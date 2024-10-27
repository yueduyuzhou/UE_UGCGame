// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayersGameMode.h"
#include "../../UGCGameInstance.h"
#include "../LobbyPlayerState.h"
#include "../LobbyGameState.h"
#include "../LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ThreadManage.h"
#include "UGCGame/Common/PlayerModule/PlayerModule.h"

ALobbyPlayersGameMode::ALobbyPlayersGameMode()
{
	GameStateClass = ALobbyGameState::StaticClass();

	PlayerStateClass = ALobbyPlayerState::StaticClass();

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/BP/Lobby/BP_LobbyPlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	//PlayerControllerClass = ALobbyPlayerController::StaticClass();

	bUseSeamlessTravel = true;
}

void ALobbyPlayersGameMode::NotifyAllPlayerUpdateList()
{
	if (UUGCGameInstance * MyGameInstance = GetGameInstance<UUGCGameInstance>())
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(It->Get()))
			{
				MyPC->ServerCallClientUpdatePlayerList(MyGameInstance->PlayerDatas);
			}
		}
	}
}

void ALobbyPlayersGameMode::NotifyAllPlayerAddMassage(const int32& InPlayerID, const FString& InMsg)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(It->Get()))
		{
			MyPC->ServerCallClientAddMassage(InPlayerID, InMsg);
		}
	}
}

void ALobbyPlayersGameMode::NotifyAllClientQuit()
{
	if (ALobbyPlayerController * LocalLobbyPC = GetLocalPlayerController())
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(It->Get()))
			{
				if (LocalLobbyPC->PlayerID != MyPC->PlayerID)
				{
					MyPC->ServerCallClientQuit();
				}
			}
		}
	}
}

void ALobbyPlayersGameMode::AddPlayerDataInInstance(const FPlayerNetData& InPlayerData)
{
	if (UUGCGameInstance * MyGameInstance = GetGameInstance<UUGCGameInstance>())
	{		
		MyGameInstance->PlayerDatas.Add(InPlayerData);
	}
}

void ALobbyPlayersGameMode::RemovePlayerDataInInstance(const FPlayerNetData& InPlayerData)
{
	if (UUGCGameInstance * MyGameInstance = GetGameInstance<UUGCGameInstance>())
	{
		TArray<int32> RemoveDatas;

		for (int32 Index = 0; Index < MyGameInstance->PlayerDatas.Num(); ++Index)
		{
			if (MyGameInstance->PlayerDatas[Index].PlayerID == InPlayerData.PlayerID)
			{
				RemoveDatas.Add(Index);
				break;
			}
		}

		// 遍历临时索引数组并删除相应的元素
		for (int32 Index : RemoveDatas)
		{
			MyGameInstance->PlayerDatas.RemoveAt(Index);
		}
	}
}

void ALobbyPlayersGameMode::ClearAllPlayer()
{
	if (UUGCGameInstance * MyGameInstance = GetGameInstance<UUGCGameInstance>())
	{
		MyGameInstance->PlayerDatas.Empty();
	}
}

ALobbyPlayerController* ALobbyPlayersGameMode::GetLocalPlayerController()
{
	if (AController * LocalPC = GetWorld()->GetFirstPlayerController())
	{
		if (ALobbyPlayerController * LocalLobbyPC = Cast<ALobbyPlayerController>(LocalPC))
		{
			return LocalLobbyPC;
		}
	}
	return nullptr;
}

void ALobbyPlayersGameMode::OnReceiveNewPlayerInfo(ALobbyPlayerController* InNewPlayer, FString InNewPlayerID)
{
	FPlayerNetData TmpPlayerData;
	TmpPlayerData.PlayerID = InNewPlayer->PlayerID;
	TmpPlayerData.Team = ETeamType::TEAM_RED;

	AddPlayerDataInInstance(TmpPlayerData);
	InNewPlayer->ServerCallClientUpdateLocalPlayerData(TmpPlayerData);

	GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
		{
			NotifyAllPlayerUpdateList();
		});
}

void ALobbyPlayersGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	ClearAllPlayer();
}

void ALobbyPlayersGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(NewPlayer))
	{
		//呼叫新加入玩家的本地客户端发送信息到服务器
		MyPC->ServerCallClientSendPlayerInfo();
	}
}

void ALobbyPlayersGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(Exiting))
	{
		FPlayerNetData TmpPlayerData;
		TmpPlayerData.PlayerID = MyPC->PlayerID;
		TmpPlayerData.Team = ETeamType::TEAM_RED;

		RemovePlayerDataInInstance(TmpPlayerData);

		NotifyAllPlayerUpdateList();
	}
}

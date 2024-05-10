// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayersGameMode.h"
#include "../../UGCGameInstance.h"
#include "../LobbyPlayerState.h"
#include "../LobbyGameState.h"
#include "../LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ThreadManage.h"

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

		// ������ʱ�������鲢ɾ����Ӧ��Ԫ��
		for (int32 Index : RemoveDatas)
		{
			MyGameInstance->PlayerDatas.RemoveAt(Index);
		}
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

void ALobbyPlayersGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(NewPlayer))
	{
		MyPC->PlayerID = (int32)FMath::RandRange(100000, 999999999);
		FPlayerNetData TmpPlayerData;
		TmpPlayerData.PlayerID = MyPC->PlayerID;
		TmpPlayerData.Team = ETeamType::TEAM_RED;

		AddPlayerDataInInstance(TmpPlayerData);
		MyPC->ServerCallClientUpdateLocalPlayerData(TmpPlayerData);

		GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
			{
				NotifyAllPlayerUpdateList();
			});
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

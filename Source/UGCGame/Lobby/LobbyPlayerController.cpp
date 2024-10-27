// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "Engine/World.h"
#include "../System/GameMapManage.h"
#include "../UI/Lobby/PlayerList/UI_PlayerList.h"
#include "../UI/Lobby/ChatFrame/UI_ChatFrame.h"
#include "LobbyPlayers/LobbyPlayersGameMode.h"
#include "../UGCGameInstance.h"
#include "../Common/PlayerModule/PlayerModule.h"

ALobbyPlayerController::ALobbyPlayerController()
	:PlayerID(INDEX_NONE)
{
	bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ALobbyPlayerController, PlayerID, COND_None);
}

void ALobbyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*if (UUGCGameInstance * MyGameInstance = GetGameInstance<UUGCGameInstance>())
	{
		if (MyGameInstance->LocalPlayerData.Team == ETeamType::TEAM_BLUE)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%d : Team Blue"), MyGameInstance->LocalPlayerData.PlayerID));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%d : Team Red"), MyGameInstance->LocalPlayerData.PlayerID));
		}
	}*/
}

void ALobbyPlayerController::ServerCallClientUpdatePlayerList_Implementation(const TArray<FPlayerNetData>& InPlayerDatas)
{
	if (PlayerList)
	{
		PlayerList->UpdatePlayerList(InPlayerDatas);
	}
}

void ALobbyPlayerController::ServerCallClientQuit_Implementation()
{
	if (PlayerList)
	{
		PlayerList->QuitMap();
	}
}

void ALobbyPlayerController::ServerCallClientUpdateLocalPlayerData_Implementation(const FPlayerNetData& InPlayerData)
{
	if (UUGCGameInstance * MyGameInstance = GetGameInstance<UUGCGameInstance>())
	{
		MyGameInstance->LocalPlayerData = InPlayerData;
	}
}

void ALobbyPlayerController::PlayerListChangeOnServer_Implementation(const FPlayerNetData& InPlayerData)
{
	if (AGameModeBase * MyGM = GetWorld()->GetAuthGameMode())
	{
		if (ALobbyPlayersGameMode * LobbyGM = Cast<ALobbyPlayersGameMode>(MyGM))
		{
			LobbyGM->RemovePlayerDataInInstance(InPlayerData);
			LobbyGM->AddPlayerDataInInstance(InPlayerData);
			LobbyGM->NotifyAllPlayerUpdateList();
		}
	}
}

void ALobbyPlayerController::AddMassageOnServer_Implementation(const int32& InPlayerID, const FString& InMsg)
{
	if (AGameModeBase * MyGM = GetWorld()->GetAuthGameMode())
	{
		if (ALobbyPlayersGameMode * LobbyGM = Cast<ALobbyPlayersGameMode>(MyGM))
		{
			LobbyGM->NotifyAllPlayerAddMassage(InPlayerID, InMsg);
		}
	}
}

void ALobbyPlayerController::ServerCallClientAddMassage_Implementation(const int32& InPlayerID, const FString& InMsg)
{
	if (ChatFrame)
	{
		ChatFrame->AddMassageToContent(InPlayerID, InMsg);
	}
}

void ALobbyPlayerController::ServerCallClientSendPlayerInfo_Implementation()
{
	ServerReceivePlayerInfo(UPlayerModule::Get()->Account);
}

void ALobbyPlayerController::ServerReceivePlayerInfo_Implementation(const FString& InNewPlayerID)
{
	PlayerID = FCString::Atoi(*InNewPlayerID);

	if (ALobbyPlayersGameMode * LPGM = GetWorld()->GetAuthGameMode<ALobbyPlayersGameMode>())
	{
		LPGM->OnReceiveNewPlayerInfo(this, InNewPlayerID);
	}
}

void ALobbyPlayerController::SetPlayerList(UUI_PlayerList* InPlayerList)
{
	PlayerList = InPlayerList;
}

void ALobbyPlayerController::SetChatFrame(UUI_ChatFrame* InChatFrame)
{
	ChatFrame = InChatFrame;
}

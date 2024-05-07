// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "Engine/World.h"
#include "../System/GameMapManage.h"
#include "../UI/Lobby/PlayerList/UI_PlayerList.h"
#include "LobbyPlayers/LobbyPlayersGameMode.h"

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

void ALobbyPlayerController::ServerCallClientUpdatePlayerList_Implementation(const TArray<FPlayerNetData>& InPlayerDatas)
{
	if (PlayerList)
	{
		PlayerList->UpdatePlayerList(InPlayerDatas);
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

void ALobbyPlayerController::SetPlayerList(UUI_PlayerList* InPlayerList)
{
	PlayerList = InPlayerList;
}
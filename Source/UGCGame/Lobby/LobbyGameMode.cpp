// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "../UGCGamePlayerState.h"
#include "../UGCGamePlayerController.h"
#include "ThreadManage.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyHUD.h"

ALobbyGameMode::ALobbyGameMode()
{
	HUDClass = ALobbyHUD::StaticClass();

	PlayerControllerClass = AUGCGamePlayerController::StaticClass();

	PlayerStateClass = AUGCGamePlayerState::StaticClass();

	bUseSeamlessTravel = true;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

}
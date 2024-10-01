// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "../UGCGamePlayerState.h"
#include "../UGCGamePlayerController.h"
#include "ThreadManage.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyHUD.h"
#include "../Common/ServerManage/ServerManage.h"

ALobbyGameMode::ALobbyGameMode()
{
	HUDClass = ALobbyHUD::StaticClass();

	PlayerControllerClass = AUGCGamePlayerController::StaticClass();

	PlayerStateClass = AUGCGamePlayerState::StaticClass();

	bUseSeamlessTravel = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void ALobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FServerManage::Get()->Tick(DeltaSeconds);
}
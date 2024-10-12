// Fill out your copyright notice in the Description page of Project Settings.


#include "HypermarketGameMode.h"
#include "HypermarketGameState.h"
#include "HypermarketPlayerController.h"
#include "HypermarketPlayerState.h"
#include "CameraPawn.h"
#include "../Common/ServerManage/ServerManage.h"

AHypermarketGameMode::AHypermarketGameMode()
{
	DefaultPawnClass = ACameraPawn::StaticClass();

	GameStateClass = AHypermarketGameState::StaticClass();

	PlayerControllerClass = AHypermarketPlayerController::StaticClass();

	PlayerStateClass = AHypermarketPlayerState::StaticClass();

	bUseSeamlessTravel = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AHypermarketGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FServerManage::Get()->Tick(DeltaSeconds);
}
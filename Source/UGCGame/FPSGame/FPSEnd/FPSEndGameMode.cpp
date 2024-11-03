// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSEndGameMode.h"
#include "FPSEndGameState.h"
#include "../../Common/ServerManage/ServerManage.h"

AFPSEndGameMode::AFPSEndGameMode()
{
	GameStateClass = AFPSEndGameState::StaticClass();

	bUseSeamlessTravel = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AFPSEndGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FServerManage::Get()->Tick(DeltaSeconds);
}

void AFPSEndGameMode::StartPlay()
{
	Super::StartPlay();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

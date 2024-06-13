// Fill out your copyright notice in the Description page of Project Settings.


#include "HypermarketGameMode.h"
#include "HypermarketGameState.h"
#include "HypermarketPlayerController.h"
#include "HypermarketPlayerState.h"
#include "CameraPawn.h"

AHypermarketGameMode::AHypermarketGameMode()
{
	DefaultPawnClass = ACameraPawn::StaticClass();

	GameStateClass = AHypermarketGameState::StaticClass();

	PlayerControllerClass = AHypermarketPlayerController::StaticClass();

	PlayerStateClass = AHypermarketPlayerState::StaticClass();
}
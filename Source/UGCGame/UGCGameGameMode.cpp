// Copyright Epic Games, Inc. All Rights Reserved.

#include "UGCGameGameMode.h"
#include "UGCGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
//#include "GameFramework/SpectatorPawn.h"
#include "UGCGameState.h"
#include "UGCGameHUD.h"
#include "UGCGamePlayerController.h"
#include "UGCGamePlayerState.h"
#include "UGCGamePawn.h"

AUGCGameGameMode::AUGCGameGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP/BP_UGCGamePawn"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = AUGCGameState::StaticClass();

	HUDClass = AUGCGameHUD::StaticClass();

	PlayerControllerClass = AUGCGamePlayerController::StaticClass();

	PlayerStateClass = AUGCGamePlayerState::StaticClass();

	bUseSeamlessTravel = true;
}

void AUGCGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AUGCGamePlayerState * NewPlayerState = NewPlayer->GetPlayerState<AUGCGamePlayerState>())
	{
		//每个新登录的玩家会在服务器上初始化PlayerID
		NewPlayerState->TPlayerID = FMath::RandRange(1000000, 888889999);
		NewPlayerState->InitPlayerData(NewPlayerState->TPlayerID);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "../UGCGamePlayerState.h"
#include "../UGCGamePlayerController.h"
#include "ThreadManage.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyHUD.h"

ALobbyGameMode::ALobbyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	HUDClass = ALobbyHUD::StaticClass();

	PlayerControllerClass = AUGCGamePlayerController::StaticClass();

	PlayerStateClass = AUGCGamePlayerState::StaticClass();

	bUseSeamlessTravel = true;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

}
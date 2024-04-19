// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameGameMode.h"
#include "ThreadManage.h"
#include "FPSGameHUD.h"
#include "FPSGamePlayerController.h"
#include "FPSGamePlayerState.h"
#include "../UGCGameInstance.h"
#include "../System/GameMapManage.h"

AFPSGameGameMode::AFPSGameGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP/FPS/BP_FPSGameCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AFPSGamePlayerController::StaticClass();

	HUDClass = AFPSGameHUD::StaticClass();

	PlayerStateClass = AFPSGamePlayerState::StaticClass();
}

void AFPSGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
	{
		UGameMapManage::Get()->LoadMapDataAndSpawn(MyGameInstance->LoadMapName, GetWorld());
	}
}
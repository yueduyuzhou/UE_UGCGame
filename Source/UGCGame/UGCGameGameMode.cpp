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
#include "ThreadManage.h"
#include "UGCGameInstance.h"
#include "System/GameMapManage.h"
#include "Common/MethodUnit.h"

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

void AUGCGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	//延时加载游戏地图
	GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
		{
			if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
			{
				if (MyGameInstance->bIsLoadMapForUGC)
				{
					UGameMapManage::Get()->LoadMapDataAndSpawnForUGC(MyGameInstance->LoadMapName, GetWorld());
				}

				if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
				{
					//TODO:读表获取初始化时应该生成的
					MyPlayerState->SpawnElementNotControl(MyPlayerState->TPlayerID, 77773);
				}
			}
		});
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

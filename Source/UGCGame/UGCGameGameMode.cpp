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
#include "UGCGame/Common/ServerManage/ServerManage.h"
#include "Common/ServerManage/Protocols/UGCGameProtocols.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

AUGCGameGameMode::AUGCGameGameMode()
	:Time(0.f)
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
	PrimaryActorTick.bCanEverTick = true;

	//ServerManage = FServerManage::Get();
}

void AUGCGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	//��ʱ������Ϸ��ͼ
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
					//TODO:�����ȡ��ʼ��ʱӦ�����ɵ�
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
		//ÿ���µ�¼����һ��ڷ������ϳ�ʼ��PlayerID
		NewPlayerState->TPlayerID = FMath::RandRange(1000000, 888889999);
		NewPlayerState->InitPlayerData(NewPlayerState->TPlayerID);
	}
}

void AUGCGameGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FServerManage::Get()->Tick(DeltaSeconds);

	/*Time += DeltaSeconds;
	if (Time > 5.f)
	{
		Time = 0.f;
		FServerManage::Get()->Send<FString, int32>(SP_C2D_DATA_REQUEST, "asd", 2325);
	}*/
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif
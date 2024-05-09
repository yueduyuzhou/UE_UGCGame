// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameGameMode.h"
#include "ThreadManage.h"
#include "FPSGameHUD.h"
#include "FPSGamePlayerController.h"
#include "FPSGamePlayerState.h"
#include "../UGCGameInstance.h"
#include "../System/GameMapManage.h"
#include "../Element/Effect/EE_SpawnPoint.h"
#include "Kismet/GameplayStatics.h"

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

	GameStateClass = AUGCGameState::StaticClass();

	bUseSeamlessTravel = true;
}

void AFPSGameGameMode::SpawnPlayerCharacters()
{
	// ��ȡ��Ϸ��ͼ�ϵĳ���������
	TArray<AActor*> SpawnPoints;
	int32 Index = 0;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEE_SpawnPoint::StaticClass(), SpawnPoints);

	// ����������ҿ����������ɽ�ɫ�����䵽����
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			//TODO���������

			// �ڶ�Ӧ�ĳ��������ɽ�ɫ
			FVector SpawnLocation = SpawnPoints[Index]->GetActorLocation();
			FRotator SpawnRotation = SpawnPoints[Index]->GetActorRotation();
			APawn * NewCharacter = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation);

			// �����ɫ����Ӧ�Ķ���
			if (NewCharacter)
			{
				PlayerController->Possess(NewCharacter);
			}
		}
	}

	for (auto& Tmp : SpawnPoints)
	{
		Tmp->Destroy();
	}
}

void AFPSGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
		{
			if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
			{
				UGameMapManage::Get()->LoadMapDataAndSpawn(MyGameInstance->LoadMapName, GetWorld());
			}
		});
}
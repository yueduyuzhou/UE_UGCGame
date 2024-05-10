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
#include "FPSGameCharacterBase.h"
#include "UGCGame/Common/UGCGameType.h"

AFPSGameGameMode::AFPSGameGameMode()
	:RedIndex(0)
	, BlueIndex(0)
{
	PlayerControllerClass = AFPSGamePlayerController::StaticClass();

	HUDClass = AFPSGameHUD::StaticClass();

	PlayerStateClass = AFPSGamePlayerState::StaticClass();

	GameStateClass = AUGCGameState::StaticClass();

	bUseSeamlessTravel = true;
}

void AFPSGameGameMode::SpawnPlayerCharacters()
{
	// 获取游戏地图上的出生点数组
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEE_SpawnPoint::StaticClass(), SpawnPoints);
	//区分出生点
	if (SpawnPoints.Num() > 0)
	{
		for (auto* Tmp : SpawnPoints)
		{
			if(AEE_SpawnPoint* CurPoint = Cast<AEE_SpawnPoint>(Tmp))
			if (CurPoint->GetTeamType() == ETeamType::TEAM_RED)
			{
				RedSpawnPoints.Add(CurPoint);
			}
			else if (CurPoint->GetTeamType() == ETeamType::TEAM_BLUE)
			{
				BlueSpawnPoints.Add(CurPoint);
			}
		}
	}

	// 遍历所有玩家控制器，生成角色并分配到队伍
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{	
		if (APlayerController * MyPC = It->Get())
		{
			if (AFPSGamePlayerController * MyFPSPC = Cast<AFPSGamePlayerController>(MyPC))
			{
				MyFPSPC->ServerCallClientSendPlayerData();
			}
		}
	}

	for (auto& Tmp : SpawnPoints)
	{
		Tmp->Destroy();
	}
}

const FTransform AFPSGameGameMode::GetNextSpawnTransform(const FPlayerNetData& InPlayerData)
{
	FTransform PlayerTransform;
	if (InPlayerData.Team == ETeamType::TEAM_RED)
	{
		PlayerTransform.SetLocation(RedSpawnPoints[RedIndex]->GetActorLocation());
		PlayerTransform.SetRotation(RedSpawnPoints[RedIndex]->GetActorRotation().Quaternion());
		RedIndex = (RedIndex + 1) % RedSpawnPoints.Num();
	}
	else if (InPlayerData.Team == ETeamType::TEAM_BLUE)
	{
		PlayerTransform.SetLocation(BlueSpawnPoints[BlueIndex]->GetActorLocation());
		PlayerTransform.SetRotation(BlueSpawnPoints[BlueIndex]->GetActorRotation().Quaternion());
		BlueIndex = (BlueIndex + 1) % BlueSpawnPoints.Num();
	}

	return PlayerTransform;
}

UClass* AFPSGameGameMode::GetCharacterClass(const ETeamType& InType)
{
	if (InType == ETeamType::TEAM_BLUE)
	{
		return BlueCharacterClass;
	}
	else if (InType == ETeamType::TEAM_RED)
	{
		return RedCharacterClass;
	}
	return nullptr;
}

void AFPSGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
		{
			if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
			{
				UGameMapManage::Get()->LoadMapDataAndSpawn(MyGameInstance->LoadMapName, GetWorld());
				SpawnPlayerCharacters();
			}
		});
}

void AFPSGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


}

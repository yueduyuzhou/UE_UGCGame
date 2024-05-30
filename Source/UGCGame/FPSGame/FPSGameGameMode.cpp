// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameGameMode.h"
#include "ThreadManage.h"
#include "FPSGameHUD.h"
#include "FPSGamePlayerController.h"
#include "FPSGamePlayerState.h"
#include "FPSGameGameState.h"
#include "../UGCGameInstance.h"
#include "../System/GameMapManage.h"
#include "../Element/Effect/EE_SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameCharacterBase.h"
#include "UGCGame/Common/UGCGameType.h"
#include "Kismet/KismetRenderingLibrary.h"

AFPSGameGameMode::AFPSGameGameMode()
	:RedIndex(0)
	, BlueIndex(0)
	, PlayerSpawnCount(0)
	, bStartDownTime(false)
	, DownTime(90.f)
{
	PlayerControllerClass = AFPSGamePlayerController::StaticClass();

	HUDClass = AFPSGameHUD::StaticClass();

	PlayerStateClass = AFPSGamePlayerState::StaticClass();

	GameStateClass = AFPSGameGameState::StaticClass();

	bUseSeamlessTravel = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AFPSGameGameMode::SpawnPlayerCharacters()
{
	if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
	{
		UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameMode]: Player Count: %d"), MyGameInstance->PlayerDatas.Num());
		
#if !WITH_EDITOR
		if (MyGameInstance->PlayerDatas.Num() == GetNumPlayers())
#else 
		if (2 == GetNumPlayers())
#endif
		{
			// 获取游戏地图上的出生点数组
			TArray<AActor*> SpawnPoints;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEE_SpawnPoint::StaticClass(), SpawnPoints);
			//区分出生点
			if (SpawnPoints.Num() > 0)
			{
				for (auto* Tmp : SpawnPoints)
				{
					if (AEE_SpawnPoint * CurPoint = Cast<AEE_SpawnPoint>(Tmp))
					{
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
		}
		else
		{
			GThread::Get()->GetCoroutines().BindLambda(0.2f, [&]()
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, TEXT("[class AFPSGameGameMode]: Repeat Call SpawnPlayerCharacters"));
					SpawnPlayerCharacters();
				});
		}
	}
	
}

void AFPSGameGameMode::GameCharacterDeath(const int32& InKillerID, const int32& InKilledID)
{
	if (UUGCGameInstance * MyGI = GetGameInstance<UUGCGameInstance>())
	{
		if (AFPSGameGameState * FPSGS = Cast<AFPSGameGameState>(GetWorld()->GetGameState()))
		{
			if (MyGI->GetTeamTypeByPlayerID(InKillerID) == ETeamType::TEAM_BLUE)
			{
				FPSGS->KillBlue(InKillerID, InKilledID);
				AllPlayerUpdateKillText(ETeamType::TEAM_BLUE);
			}
			else
			{
				FPSGS->KillRed(InKillerID, InKilledID);
				AllPlayerUpdateKillText(ETeamType::TEAM_RED);
			}
		}
	}
}

void AFPSGameGameMode::InitDownTime()
{
	bStartDownTime = true;

	//通知客户端开始模拟计时
	AllPlayerUpdateDownTime(DownTime);
}

void AFPSGameGameMode::AllPlayerUpdateDownTime(const int32& InDownTime)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController * MyPC = It->Get())
		{
			if (AFPSGamePlayerController * MyFPSPC = Cast<AFPSGamePlayerController>(MyPC))
			{
				MyFPSPC->ServerCallClientUpdateDownTime(InDownTime);
			}
		}
	}
}

void AFPSGameGameMode::EndGame()
{
	ETeamType WinTeam = ETeamType::TEAM_NONE;
	//结算（队伍人头/队伍积分）
	if (AFPSGameGameState * FPSGS = Cast<AFPSGameGameState>(GetWorld()->GetGameState()))
	{
		if (UUGCGameInstance * MyGI = GetGameInstance<UUGCGameInstance>())
		{
			//判断获胜队伍
			if (FPSGS->GetBlueTeamKillCount() < FPSGS->GetRedTeamKillCount())
			{
				WinTeam = ETeamType::TEAM_RED;
			}
			else if (FPSGS->GetBlueTeamKillCount() > FPSGS->GetRedTeamKillCount())
			{
				WinTeam = ETeamType::TEAM_BLUE;
			}

			//通知客户端显示获胜/失败页面
			if (AFPSGamePlayerController * LocalFPSPC = GetLocalPlayerController())
			{
				//保存再服务端Instance
				MyGI->WinTeam = WinTeam;
				MyGI->EndGamePlayerInfos = FPSGS->GetFPSPlayerInfos();

				LocalFPSPC->EndGame(WinTeam, FPSGS->GetFPSPlayerInfos());
				//AllPlayerEndGame(WinTeam, FPSGS->GetFPSPlayerInfos());
			}
		}
	}

	
}

void AFPSGameGameMode::AllClientEndGame(const ETeamType& InWinTeam, const TArray<FFPSPlayerInfo>& InPlayerInfos)
{
	if (AFPSGamePlayerController * LocalFPSPC = GetLocalPlayerController())
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (AFPSGamePlayerController * MyFPSPC = Cast<AFPSGamePlayerController>(It->Get()))
			{
				if (LocalFPSPC->PlayerID != MyFPSPC->PlayerID)
				{
					MyFPSPC->ServerCallClientEndGame(InWinTeam, InPlayerInfos);
				}
			}
		}
	}
}

void AFPSGameGameMode::AllPlayerUpdateMiniMap()
{
	if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController * MyPC = It->Get())
			{
				if (AFPSGamePlayerController * MyFPSPC = Cast<AFPSGamePlayerController>(MyPC))
				{
					MyFPSPC->ServerCallClientUpdateMiniMap(MyGameInstance->LoadMapName);
				}
			}
		}
	}
}

void AFPSGameGameMode::AllPlayerUpdateKillText(const ETeamType& InTeamType)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController * MyPC = It->Get())
		{
			if (AFPSGamePlayerController * MyFPSPC = Cast<AFPSGamePlayerController>(MyPC))
			{
				MyFPSPC->ServerCallClientUpdateKillText(InTeamType);
			}
		}
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

AFPSGamePlayerController* AFPSGameGameMode::GetLocalPlayerController()
{
	if (AController * LocalPC = GetWorld()->GetFirstPlayerController())
	{
		if (AFPSGamePlayerController * LocalLobbyPC = Cast<AFPSGamePlayerController>(LocalPC))
		{
			return LocalLobbyPC;
		}
	}
	return nullptr;
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

void AFPSGameGameMode::AddSpawnCount()
{
	PlayerSpawnCount++;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameGameMode]: AddSpawnCount, PlayerSpawnCount = %d"), PlayerSpawnCount));
}

void AFPSGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	//延时加载游戏地图
	GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
		{
			if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
			{
				UGameMapManage::Get()->LoadMapDataAndSpawn(MyGameInstance->LoadMapName, GetWorld());
			}
		});

	GThread::Get()->GetCoroutines().BindLambda(2.f, [&]()
		{
			//通知所有端更新MiniMap
			AllPlayerUpdateMiniMap();

			//为玩家生成游戏角色
			SpawnPlayerCharacters();

			//开始计时
			InitDownTime();
		});
}

void AFPSGameGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bStartDownTime)
	{
		DownTime -= DeltaSeconds;
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%0.2f"), DownTime));
		if (DownTime <= 0.f)
		{
			//游戏结算
			EndGame();
		}
	}
}

void AFPSGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AddSpawnCount();
}

void AFPSGameGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayerSpawnCount--;
}

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

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

AFPSGameGameMode::AFPSGameGameMode()
	:Super()
	, RedIndex(0)
	, BlueIndex(0)
	, bStartDownTime(false)
	, bCheckQuitGame(false)
	, bStartedGame(false)
	, bMapReady(false)
	, bAllPlayerReadyStartGame(false)
	, DownTime(90.f)
	, WinTeam(ETeamType::TEAM_NONE)
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
		UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameMode::SpawnPlayerCharacters]: Player Count: %d"), MyGameInstance->PlayerDatas.Num());
		
		// ��ȡ��Ϸ��ͼ�ϵĳ���������
		TArray<AActor*> SpawnPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEE_SpawnPoint::StaticClass(), SpawnPoints);
		//���ֳ�����
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

		// ����������ҿ�������������Ϣ�����ɽ�ɫ�����䵽����
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
	
}

void AFPSGameGameMode::GameCharacterDeath(const int32& InKillerID, const int32& InKilledID)
{
	UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameMode]:%d ==Kill==> %d"), InKillerID, InKilledID);
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

	//֪ͨ�ͻ��˿�ʼģ���ʱ
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

void AFPSGameGameMode::PreEndGame()
{
	bCheckQuitGame = true;
	bStartDownTime = false;

	//����������
	if (EndGameSettlement())
	{
		if (AFPSGameGameState * FPSGS = Cast<AFPSGameGameState>(GetWorld()->GetGameState()))
		{
			//LocalFPSPC->EndGame(WinTeam, FPSGS->GetFPSPlayerInfos());
			//֪ͨ���Լ������пͻ����˳��Ự�����㣬��ʾ��ʤ/ʧ��ҳ��
			AllClientEndGame(WinTeam, FPSGS->GetFPSPlayerInfos());
		}
	}
}

void AFPSGameGameMode::EndGame()
{
	//TODO�������˳�
	if (AFPSGameGameState * FPSGS = Cast<AFPSGameGameState>(GetWorld()->GetGameState()))
	{
		if (AFPSGamePlayerController * LocalFPSPC = GetLocalPlayerController())
		{
			LocalFPSPC->EndGame(WinTeam, FPSGS->GetFPSPlayerInfos());
		}
	}
}

bool AFPSGameGameMode::EndGameSettlement()
{
	//���㣨������ͷ/������֣�
	if (AFPSGameGameState * FPSGS = Cast<AFPSGameGameState>(GetWorld()->GetGameState()))
	{
		if (UUGCGameInstance * MyGI = GetGameInstance<UUGCGameInstance>())
		{
			//�жϻ�ʤ����
			if (FPSGS->GetBlueTeamKillCount() < FPSGS->GetRedTeamKillCount())
			{
				WinTeam = ETeamType::TEAM_RED;
			}
			else if (FPSGS->GetBlueTeamKillCount() > FPSGS->GetRedTeamKillCount())
			{
				WinTeam = ETeamType::TEAM_BLUE;
			}


			//�����ٷ����Instance
			MyGI->WinTeam = WinTeam;
			MyGI->EndGamePlayerInfos = FPSGS->GetFPSPlayerInfos();

			return WinTeam != ETeamType::TEAM_NONE;
		}
	}

	return false;
}

void AFPSGameGameMode::AllClientEndGame(const ETeamType& InWinTeam, const TArray<FFPSPlayerInfo>& InPlayerInfos)
{
	if (AFPSGamePlayerController * LocalFPSPC = GetLocalPlayerController())
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (AFPSGamePlayerController * MyFPSPC = Cast<AFPSGamePlayerController>(It->Get()))
			{
				//�Ƿ�����
				if (LocalFPSPC->PlayerID != MyFPSPC->PlayerID)
				{
					MyFPSPC->ServerCallClientEndGame(InWinTeam, InPlayerInfos);
				}
			}
		}
	}
}

bool AFPSGameGameMode::IsReadyStartGame()
{
	bool bAllPlayerJoined = false;
	//������Ҽ���ŻῪ��ʽ������Ϸ����
#if !WITH_EDITOR
	if (MyGameInstance->PlayerDatas.Num() == GetNumPlayers())
#else 
	if (2 == GetNumPlayers())
#endif
	{
		bAllPlayerJoined = true;
	}

	//��������Ѽ��� ��ͼ��׼���� ��Ϸδ��ʽ��ʼ
	return bAllPlayerJoined && bMapReady && !bStartedGame;
}

bool AFPSGameGameMode::IsAllPlayerReadyStartGame()
{
	return bAllPlayerReadyStartGame;
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

void AFPSGameGameMode::ChangeMapReadyState(bool InState)
{
	bMapReady = InState;
}

void AFPSGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	//������Ϸ��ͼ
	if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
	{
		UGameMapManage::Get()->LoadMapDataAndSpawnForFPS(MyGameInstance->LoadMapName, GetWorld());
	}
}

void AFPSGameGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FServerManage::Get()->Tick(DeltaSeconds);

	//��ʼ��Ϸ���
	if (IsReadyStartGame())
	{
		//��ʼ��Ϸ���
		bStartedGame = true;

		//֪ͨ���ж˸���MiniMap
		AllPlayerUpdateMiniMap();
		UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameMode::Tick] : AllPlayerUpdateMiniMap"));

		//Ϊ���������Ϸ��ɫ
		SpawnPlayerCharacters();
		UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameMode::Tick] : SpawnPlayerCharacters"));

		//��ʼ��ʱ
		InitDownTime();
		UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameMode::Tick] : InitDownTime"));

	}

	//Ԥ�˳���Ϸ���
	if (bStartDownTime && DownTime >= 0.f)
	{
		DownTime -= DeltaSeconds;
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%0.2f"), DownTime));
		if (DownTime <= 0.f)
		{
			PreEndGame();
			UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameMode::Tick] : PreEndGame"));
		}
	}

	//�˳���Ϸ���
	if (bCheckQuitGame && GetNumPlayers() == 1)
	{
		EndGame();
		UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameMode::Tick] : EndGame"));
	}
}

void AFPSGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AFPSGameGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);


}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif
// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGamePlayerController.h"
#include "FPSGameCharacterBase.h"
#include "FPSGameHUD.h"
#include "UGCGame/UGCGameInstance.h"
#include "FPSGameGameMode.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGamePlayerState.h"
#include "FPSGameGameState.h"

AFPSGamePlayerController::AFPSGamePlayerController()
	:CrosshairUI(nullptr)
	, MiniMapUI(nullptr)
	, DownTimeUI(nullptr)
	, TeamType(ETeamType::TEAM_NONE)
	, PlayerID(INDEX_NONE)
	, ReSpawnTime(3.f)
{

}

void AFPSGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AFPSGamePlayerController::OnLeftMouseButtonDown);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AFPSGamePlayerController::OnLeftMouseButtonUp);

	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AFPSGamePlayerController::OnRightMouseButtonDown);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &AFPSGamePlayerController::OnRightMouseButtonUp);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSGamePlayerController::Jump);

	InputComponent->BindAction("LowSpeedWalk", IE_Pressed, this, &AFPSGamePlayerController::OnLeftShiftButtonDown);
	InputComponent->BindAction("LowSpeedWalk", IE_Released, this, &AFPSGamePlayerController::OnLeftShiftButtonUp);

	InputComponent->BindAction("AmmoReload", IE_Pressed, this, &AFPSGamePlayerController::AmmoReload);
	InputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AFPSGamePlayerController::SwitchWeapon);
	InputComponent->BindAction("ThrowWeapon", IE_Pressed, this, &AFPSGamePlayerController::ThrowWeapon);

	InputComponent->BindAxis("MoveForward", this, &AFPSGamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSGamePlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AFPSGamePlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &AFPSGamePlayerController::LookUpAtRate);
}

void AFPSGamePlayerController::OnLeftMouseButtonDown()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->WeaponFirePressed();
	}
}

void AFPSGamePlayerController::OnLeftMouseButtonUp()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->WeaponFireReleassed();
	}
}

void AFPSGamePlayerController::OnRightMouseButtonDown()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->WeaponAimingPressed();
	}
}

void AFPSGamePlayerController::OnRightMouseButtonUp()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->WeaponEndAimingReleassed();
	}
}

void AFPSGamePlayerController::Jump()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->Jump();
	}
}

void AFPSGamePlayerController::OnLeftShiftButtonDown()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->LowSpeedWalk();
	}
}

void AFPSGamePlayerController::OnLeftShiftButtonUp()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->NormalSpeedWalk();
	}
}

void AFPSGamePlayerController::AmmoReload()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->AmmoReload();
	}
}

void AFPSGamePlayerController::SwitchWeapon()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->SwitchWeapon();
	}
}

void AFPSGamePlayerController::ThrowWeapon()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->DetachWeapon();
	}
}

void AFPSGamePlayerController::MoveForward(float Value)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->MoveForward(Value);
	}
}

void AFPSGamePlayerController::MoveRight(float Value)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->MoveRight(Value);
	}
}

void AFPSGamePlayerController::TurnAtRate(float Rate)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->TurnAtRate(Rate);
	}
}

void AFPSGamePlayerController::LookUpAtRate(float Rate)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->LookUpAtRate(Rate);
	}
}

void AFPSGamePlayerController::ServerCallClientSendPlayerData_Implementation()
{
	if (UGameInstance * MyGI = GetWorld()->GetGameInstance())
	{
		if (UUGCGameInstance * MyUGCGI = Cast<UUGCGameInstance>(MyGI))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("[class AFPSGamePlayerController] : ServerCallClientSendPlayerData, MyUGCGI->LocalPlayerData.PlayerID = %d"), MyUGCGI->LocalPlayerData.PlayerID));
			SendPlayerDataToServer(MyUGCGI->LocalPlayerData);
		}
	}
}

void AFPSGamePlayerController::ServerCallClientUpdateMiniMap_Implementation(const FString& InMapName)
{
	if (MiniMapUI)
	{
		//捕捉小地图图片
		MapCapture.Initialize(GetWorld());
		FString SavePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("MiniMapScreenshot"), InMapName);
		MapCapture.CaptureMiniMapImage(SavePath);

		if (UTexture2D * MiniMapTexture = MapCapture.LoadBMPImage(SavePath + FString(TEXT("00000.bmp"))))
		{
			//更新MiniMapUI
			MiniMapUI->UpdateMiniMapImage(MiniMapTexture);
			//延时删除图片文件 会崩溃
			/*GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
				{
					MapCapture.DeleteBMPFile(SavePath + FString(TEXT("00000.bmp")));
				});*/
			
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[class AFPSGamePlayerController] : ServerCallClientUpdateMiniMap, MiniMapTexture Is Null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class AFPSGamePlayerController] : ServerCallClientUpdateMiniMap, MiniMapUI Is Null"));
	}
}

void AFPSGamePlayerController::ServerCallClientUpdateDownTime_Implementation(const int32& InDownTime)
{
	//通知本地UI开始更新时间
	DownTimeUI->StartDownTime(InDownTime);
}

void AFPSGamePlayerController::ServerCallClientUpdateKillText_Implementation(const ETeamType& InTeamType)
{
	//通知本地UI开始更新团队击杀数
	TopInfoPanelUI->UpdateKillText(InTeamType);
}

void AFPSGamePlayerController::ServerCallClientEndGame_Implementation(const ETeamType& InWinTeam, const TArray<FFPSPlayerInfo>& InPlayerInfos)
{
	//保存结束数据
	if (UUGCGameInstance * MyGI = GetGameInstance<UUGCGameInstance>())
	{
		MyGI->WinTeam = InWinTeam;
		MyGI->EndGamePlayerInfos = InPlayerInfos;

		//禁止输入，播放过场动画

		//延时打开新关卡，显示结算UI
		EndGame(InWinTeam, InPlayerInfos);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("[class AFPSGamePlayerController] : ServerCallClientEndGame, MyGI Is Null")));
	}
}

void AFPSGamePlayerController::SendPlayerDataToServer_Implementation(const FPlayerNetData& InPlayerData)
{
	if (AGameModeBase * MyGM = GetWorld()->GetAuthGameMode())
	{
		if (AFPSGameGameMode * MyFPSGM = Cast<AFPSGameGameMode>(MyGM))
		{
			if (AFPSGameGameState * FPSGS = Cast<AFPSGameGameState>(GetWorld()->GetGameState()))
			{
				TeamType = InPlayerData.Team;
				PlayerID = InPlayerData.PlayerID;

				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, 
					FString::Printf(
						TEXT("[class AFPSGamePlayerController] : SendPlayerDataToServer, InPlayerData.PlayerID = %d, PlayerID = %d"), 
						InPlayerData.PlayerID,
						PlayerID));
				FPSGS->RegisterPlayerInfo(PlayerID);

				SpawnPlayerCharacter(
					MyFPSGM->GetCharacterClass(InPlayerData.Team),
					MyFPSGM->GetNextSpawnTransform(InPlayerData));
			}
		}
	}
}

void AFPSGamePlayerController::SpawnPlayerCharacter(UClass* InCharacterClass, const FTransform& InTransform)
{
	if (AGameModeBase * MyGM = GetWorld()->GetAuthGameMode())
	{
		if (AFPSGameGameMode * MyFPSGM = Cast<AFPSGameGameMode>(MyGM))
		{
			if (APawn * NewCharacter = GetWorld()->SpawnActor<APawn>(InCharacterClass, InTransform))
			{
				UE_LOG(LogTemp, Display, TEXT("[class AFPSGamePlayerController] : PlayerController Possess Pawn"));
				Possess(NewCharacter);
				MyFPSGM->AddSpawnCount();
			}
		}
	}
}

void AFPSGamePlayerController::ControllerCharacterDeath(AActor* InDamager)
{
	if (APawn * ControllerPawn = GetPawn())
	{
		if (AFPSGameCharacterBase * FPSCharacter = Cast<AFPSGameCharacterBase>(ControllerPawn))
		{
			if (AFPSGameCharacterBase * DamageFPSCharacter = Cast<AFPSGameCharacterBase>(InDamager))
			{
				if (AFPSGamePlayerController * DamagerController = Cast<AFPSGamePlayerController>(DamageFPSCharacter->GetOwner()))
				{
					//通知GameMode有角色死亡
					if (AGameModeBase * MyGM = GetWorld()->GetAuthGameMode())
					{
						if (AFPSGameGameMode * MyFPSGM = Cast<AFPSGameGameMode>(MyGM))
						{
							//GameMode处理死亡
							GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGamePlayerController] : Call ControllerCharacterDeath, KillerPlayerID = %d, KilledPlayerID = %d"), DamagerController->PlayerID, PlayerID));

							MyFPSGM->GameCharacterDeath(DamagerController->PlayerID, PlayerID);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("[class AFPSGamePlayerController] : ControllerCharacterDeath, MyFPSGM Is Null"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("[class AFPSGamePlayerController] : ControllerCharacterDeath, MyGM Is Null"));
					}

					//角色销毁
					FPSCharacter->Destroy();

					//禁止输入

					//计时重生
					GThread::Get()->GetCoroutines().BindLambda(ReSpawnTime, [&]()
						{
							ServerCallClientSendPlayerData();
						});
				}
			}
		}
	}
	else
	{

	}
}

void AFPSGamePlayerController::PlayerCameraShake(TSubclassOf<UCameraShake> InCameraShake)
{
	ClientPlayCameraShake(InCameraShake);
}

void AFPSGamePlayerController::CrosshairRecoil()
{
	if (CrosshairUI)
	{
		CrosshairUI->DoCrosshairRecoil();
	}
}

void AFPSGamePlayerController::UpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo)
{
	UE_LOG(LogTemp, Warning, TEXT("[class AFPSGamePlayerController] : Call UpdateAmmo"));
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGamePlayerController] : Call UpdateAmmo")));
	if (CrosshairUI)
	{
		CrosshairUI->UpdateAmmo(InCurrentClipAmmo, InCurrentAmmo);
	}
}

void AFPSGamePlayerController::UpdateHealth(const float& InHealth, const float& InMaxHealth)
{
	if (CrosshairUI)
	{
		CrosshairUI->UpdateHealth(InHealth, InMaxHealth);
	}
}

bool AFPSGamePlayerController::IsAuthority()
{
	return GetLocalRole() == ROLE_Authority;
}
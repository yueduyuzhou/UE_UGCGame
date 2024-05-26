// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPSGamePlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponBaseServer.h"
#include "WeaponBaseClient.h"
#include "Components/DecalComponent.h"
#include "FPSGamePlayerState.h"
#include "Camera/CameraComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ThreadManage.h"

AFPSGameCharacterBase::AFPSGameCharacterBase()
	:BaseTurnRate(45.f)
	, BaseLookUpRate(45.f)
	, IsFireing(false)
	, IsReloading(false)
	, IsAiming(false)
	, PsitolRecoilMin(0.f)
	, PsitolRecoilMax(0.f)
{ 
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(PlayerCamera);
	//仅自己可见
	ArmMesh->SetOnlyOwnerSee(true);

	//自己不可见，仅查询，类型为Pawn
	Mesh->SetOwnerNoSee(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	WeaponPrimaryServer = nullptr;
	WeaponPrimaryClient = nullptr;
	WeaponSecondaryServer = nullptr;
	WeaponSecondaryClient = nullptr;

}

void AFPSGameCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSGameCharacterBase, IsFireing, COND_None);
	DOREPLIFETIME_CONDITION(AFPSGameCharacterBase, IsReloading, COND_None);
	DOREPLIFETIME_CONDITION(AFPSGameCharacterBase, IsAiming, COND_None);
	DOREPLIFETIME_CONDITION(AFPSGameCharacterBase, ActiveWeapon, COND_None);
}

void AFPSGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	ClientArmAnimBP = ArmMesh->GetAnimInstance();
	ClientBodyAnimBP = Mesh->GetAnimInstance();

	OnTakePointDamage.AddDynamic(this, &AFPSGameCharacterBase::OnHit);

	StartWeapon();

	GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
		{
			if (AFPSGamePlayerState * MyPlayerState = Cast<AFPSGamePlayerState>(GetPlayerState()))
			{
				MyPlayerState->DeathResetData();
				ServerCallClientUpdateHealth(MyPlayerState->Health, MyPlayerState->MaxHealth);
			}
		});
}

void AFPSGameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (GetLocalRole() == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : Authority Current ActiveWeapon = %d, Current StartWeapon = %d"), ActiveWeapon, StartWeaponType));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : Client Current ActiveWeapon = %d, Current StartWeapon = %d"), ActiveWeapon, StartWeaponType));
	}*/
}

void AFPSGameCharacterBase::StartWeapon()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		PurchaseWeapon(StartWeaponType);
	}
}

void AFPSGameCharacterBase::PurchaseWeapon(EWeaponType InWeaponType)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	switch (InWeaponType)
	{
	case EWeaponType::AK47:
		{
			//生成
			UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/Ak47/BP_Ak47_Server.BP_Ak47_Server_c'"));
			AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);
			
			//设置当前持有武器类型
			ActiveWeapon = EWeaponType::AK47;
			
			//预处理并装备当前持有武器
			ServerWeapon->EquipWeapon();
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
	case EWeaponType::M4A1:
		{
			//生成
			UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/M4A1/BP_M4A1_Server.BP_M4A1_Server_c'"));
			AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);
			
			//设置当前持有武器类型
			ActiveWeapon = EWeaponType::M4A1;

			//预处理并装备当前持有武器
			ServerWeapon->EquipWeapon();
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
	case EWeaponType::MP7:
		{
			//生成
			UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/MP7/BP_MP7_Server.BP_MP7_Server_c'"));
			AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);

			//设置当前持有武器类型
			ActiveWeapon = EWeaponType::MP7;

			//预处理并装备当前持有武器
			ServerWeapon->EquipWeapon();
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
	case EWeaponType::DESERTEAGLE:
		{
			//生成
			UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/DesertEagle/BP_DesertEagle_Server.BP_DesertEagle_Server_c'"));
			AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);

			//设置当前持有武器类型
			ActiveWeapon = EWeaponType::DESERTEAGLE;

			//预处理并装备当前持有武器
			ServerWeapon->EquipWeapon();
			EquipSecondaryWeapon(ServerWeapon);
			break;
		}
	case EWeaponType::SNIPER:
		{
			//生成
			UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/Sniper/BP_Sniper_Server.BP_Sniper_Server_c'"));
			AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);

			//设置当前持有武器类型
			ActiveWeapon = EWeaponType::SNIPER;

			//预处理并装备当前持有武器
			ServerWeapon->EquipWeapon();
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
	}
}

AWeaponBaseClient* AFPSGameCharacterBase::GetCurrentClientWeapon()
{
	switch (ActiveWeapon)
	{
	case EWeaponType::AK47:
		{
			return WeaponPrimaryClient;
		}
	case EWeaponType::M4A1:
		{
			return WeaponPrimaryClient;
		}
	case EWeaponType::MP7:
		{
			return WeaponPrimaryClient;
		}
	case EWeaponType::DESERTEAGLE:
		{
			return WeaponSecondaryClient;
		}
	case EWeaponType::SNIPER:
		{
			return WeaponPrimaryClient;
		}
	}
	return nullptr;
}

AWeaponBaseServer* AFPSGameCharacterBase::GetCurrentServerWeapon()
{
	switch (ActiveWeapon)
	{
	case EWeaponType::AK47:
		{
			return WeaponPrimaryServer;
		}
	case EWeaponType::M4A1:
		{
			return WeaponPrimaryServer;
		}
	case EWeaponType::MP7:
		{
			return WeaponPrimaryServer;
		}
	case EWeaponType::DESERTEAGLE:
		{
			return WeaponSecondaryServer;
		}
	case EWeaponType::SNIPER:
		{
			return WeaponPrimaryServer;
		}
	}
	return nullptr;
}

void AFPSGameCharacterBase::ChangeWalkWeaponOnServer_Implementation(float InValue)
{
	CharacterMovement->MaxWalkSpeed = InValue;
}

void AFPSGameCharacterBase::RifleWeaponFireOnServer_Implementation(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing)
{
	//Server设置开火标记
	IsFireing = true;

	if (WeaponPrimaryServer)
	{
		//广播枪口火花(多播)
		WeaponPrimaryServer->MulticastFireEffect();
		WeaponPrimaryServer->ExpendAmmo();

		//第三人称身体射击动画(多播)
		MulticastFire();
		
		ServerCallClientUpdateAmmo(WeaponPrimaryServer->GetCurrentClipAmmo(), WeaponPrimaryServer->GetCurrentAmmo());
	}

	RifleLineTrace(InCamreaLocation, InCameraRotation, IsMoveing);
}

void AFPSGameCharacterBase::SniperWeaponFireOnServer_Implementation(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing)
{
	//Server设置开火标记
	IsFireing = true;

	if (WeaponPrimaryServer)
	{
		//广播枪口火花(多播)
		WeaponPrimaryServer->MulticastFireEffect();
		WeaponPrimaryServer->ExpendAmmo();

		//第三人称身体射击动画(多播)
		MulticastFire();

		ServerCallClientUpdateAmmo(WeaponPrimaryServer->GetCurrentClipAmmo(), WeaponPrimaryServer->GetCurrentAmmo());
	}

	if (WeaponPrimaryClient)
	{
		GThread::Get()->GetCoroutines().BindUObject(
			WeaponPrimaryClient->ClientArmFireMontage->GetPlayLength(),
			this,
			&AFPSGameCharacterBase::SniperFireDelayCallBack);
	}

	SniperLineTrace(InCamreaLocation, InCameraRotation, IsMoveing);
}

void AFPSGameCharacterBase::PsitolWeaponFireOnServer_Implementation(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing)
{
	//Server设置开火标记
	IsFireing = true;

	if (WeaponSecondaryServer)
	{
		//广播枪口火花(多播)
		WeaponSecondaryServer->MulticastFireEffect();
		WeaponSecondaryServer->ExpendAmmo();

		//第三人称身体射击动画(多播)
		MulticastFire();

		ServerCallClientUpdateAmmo(WeaponSecondaryServer->GetCurrentClipAmmo(), WeaponSecondaryServer->GetCurrentAmmo());
	}

	//计时重置后坐力偏移
	GThread::Get()->GetCoroutines().BindLambda(WeaponSecondaryServer->GetResetRecoilFrequency(), [&]()
		{
			PsitolRecoilMin = 0.f;
			PsitolRecoilMax = 0.f;
		});

	PistolLineTrace(InCamreaLocation, InCameraRotation, IsMoveing);
}

void AFPSGameCharacterBase::PrimaryWeaponReloadOnServer_Implementation()
{
	if (AWeaponBaseClient * ClientWeapon = GetCurrentClientWeapon())
	{
		if (WeaponPrimaryServer)
		{
			if (WeaponPrimaryServer->GetCurrentAmmo() > 0 && WeaponPrimaryServer->GetCurrentClipAmmo() < WeaponPrimaryServer->GetMaxClipAmmo())
			{
				//设置换弹标志
				IsReloading = true;

				//客户端手臂动画、服务器多播身体动画、数据更新、UI更新
				ServerCallClientReloadAnimation();
				MulticastReload();

				GThread::Get()->GetCoroutines().BindUObject(
					ClientWeapon->ClientArmReloadMontage->GetPlayLength(),
					this,
					&AFPSGameCharacterBase::ReloadDelayCallBack);
			}
		}
	}
}

void AFPSGameCharacterBase::SecondaryWeaponReloadOnServer_Implementation()
{
	if (AWeaponBaseClient * ClientWeapon = GetCurrentClientWeapon())
	{
		if (WeaponSecondaryServer)
		{
			if (WeaponSecondaryServer->GetCurrentAmmo() > 0 && WeaponSecondaryServer->GetCurrentClipAmmo() < WeaponSecondaryServer->GetMaxClipAmmo())
			{
				//设置换弹标志
				IsReloading = true;

				//客户端手臂动画、服务器多播身体动画、数据更新、UI更新
				ServerCallClientReloadAnimation();
				MulticastReload();

				GThread::Get()->GetCoroutines().BindUObject(
					ClientWeapon->ClientArmReloadMontage->GetPlayLength(),
					this,
					&AFPSGameCharacterBase::ReloadDelayCallBack);
			}
		}
	}
}

void AFPSGameCharacterBase::StopFireingOnServer_Implementation()
{
	IsFireing = false;
}

void AFPSGameCharacterBase::SetIsAimingOnServer_Implementation(bool InIsAiming)
{
	IsAiming = InIsAiming;
}

void AFPSGameCharacterBase::ServerCallClientEquipPrimaryWeapon_Implementation()
{
	if (WeaponPrimaryServer)
	{
		if (WeaponPrimaryClient)
		{

		}
		else
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			WeaponPrimaryClient = GetWorld()->SpawnActor<AWeaponBaseClient>(WeaponPrimaryServer->ClientWeaponClass, GetActorTransform(), SpawnInfo);

			GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
				{
					FName SocketName = TEXT("WeaponSocket");
					if (ActiveWeapon == EWeaponType::M4A1)
					{
						SocketName = TEXT("M4A1WeaponSocket");
					}
					else if (ActiveWeapon == EWeaponType::MP7)
					{
						SocketName = TEXT("MP7WeaponSocket");
					}
					else if (ActiveWeapon == EWeaponType::SNIPER)
					{
						SocketName = TEXT("SniperWeaponSocket");
					}

					if (WeaponPrimaryClient)
					{
						UE_LOG(LogTemp, Warning, TEXT("[class AFPSGameCharacterBase] : Client Bind Weapon To Arm, Current SocketName = %s"), *SocketName.ToString());
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : Client Bind Weapon To Arm, Current SocketName = %s"), *SocketName.ToString()));

						WeaponPrimaryClient->K2_AttachToComponent(ArmMesh, SocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

						//调整手臂动画
						UpdateFPSArmAnimBlend(WeaponPrimaryClient->ArmAnimBlendIndex);
					}
				});
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientEquipSecondaryWeapon_Implementation()
{
	if (WeaponSecondaryServer)
	{
		if (WeaponSecondaryClient)
		{

		}
		else
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			WeaponSecondaryClient = GetWorld()->SpawnActor<AWeaponBaseClient>(WeaponSecondaryServer->ClientWeaponClass, GetActorTransform(), SpawnInfo);

			FName SocketName = TEXT("DesertEagleWeaponSocket");
			if (WeaponSecondaryClient)
			{
				WeaponSecondaryClient->K2_AttachToComponent(ArmMesh, SocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

				//调整手臂动画
				GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
					{
						UpdateFPSArmAnimBlend(WeaponSecondaryClient->ArmAnimBlendIndex);
					});
			}			
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientFireWeapon_Implementation()
{
	if (AWeaponBaseClient * CurClientWeapon = GetCurrentClientWeapon())
	{
		//枪体播放动画
		CurClientWeapon->PlayShootAnimation();
		//开火效果
		CurClientWeapon->DisplayWeaponEffect();

		if (UAnimMontage * ClientArmFireMontage = CurClientWeapon->ClientArmFireMontage)
		{
			//手臂播放动画
			ClientArmAnimBP->Montage_SetPlayRate(ClientArmFireMontage, 1.f);
			ClientArmAnimBP->Montage_Play(ClientArmFireMontage);
		}

		if (FPSPlayerController)
		{
			//屏幕抖动
			FPSPlayerController->PlayerCameraShake(CurClientWeapon->CameraShakeClass);

			//准星动画
			FPSPlayerController->CrosshairRecoil();
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientWeaponRecoil_Implementation()
{
	if (WeaponPrimaryServer)
	{
		CurveXRecoilValue += 0.1;

		if (UCurveFloat * WeaponVerticalRecoilCurve = WeaponPrimaryServer->WeaponVerticalRecoilCurve)
		{
			NewVerticalRecoilValue = WeaponVerticalRecoilCurve->GetFloatValue(CurveXRecoilValue);
			VerticalRecoilDiffValue = NewVerticalRecoilValue - OldVerticalRecoilValue;
			OldVerticalRecoilValue = NewVerticalRecoilValue;
		}

		if (UCurveFloat * WeaponHorizontalRecoilCurve = WeaponPrimaryServer->WeaponHorizontalRecoilCurve)
		{
			NewHorizontalRecoilValue = WeaponHorizontalRecoilCurve->GetFloatValue(CurveXRecoilValue);
			HorizontalRecoilDiffValue = NewHorizontalRecoilValue - OldHorizontalRecoilValue;
			OldHorizontalRecoilValue = NewHorizontalRecoilValue;
		}

		if (FPSPlayerController)
		{
			FRotator OldRotataion = FPSPlayerController->GetControlRotation();
			FPSPlayerController->SetControlRotation(
				FRotator(
					OldRotataion.Pitch + VerticalRecoilDiffValue,
					OldRotataion.Yaw + HorizontalRecoilDiffValue,
					OldRotataion.Roll));
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientUpdateAmmo_Implementation(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo)
{
	
	if (GetLocalRole() == ROLE_Authority)
	{
		if (AFPSGamePlayerController * TmpFPSPlayerController = GetFPSPlayerControllerOnServer())
		{
			UE_LOG(LogTemp, Warning, TEXT("[class AFPSGameCharacterBase] : Authority CurrentClipAmmo = %d, CurrentAmmo = %d"), InCurrentClipAmmo, InCurrentAmmo);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : Authority CurrentClipAmmo = %d, CurrentAmmo = %d"), InCurrentClipAmmo, InCurrentAmmo));
			TmpFPSPlayerController->UpdateAmmo(InCurrentClipAmmo, InCurrentAmmo);
		}
		else
		{
			GThread::Get()->GetCoroutines().BindLambda(0.2f, [&](const int32 & InCurrentClipAmmo, const int32 & InCurrentAmmo)
				{
					UpdateAmmoForSuccessServer(InCurrentClipAmmo, InCurrentAmmo);
				}, InCurrentClipAmmo, InCurrentAmmo);
		}
	}
	else
	{
		FPSPlayerController = GetController<AFPSGamePlayerController>();
		if (FPSPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("[class AFPSGameCharacterBase] : Client CurrentClipAmmo = %d, CurrentAmmo = %d"), InCurrentClipAmmo, InCurrentAmmo);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : Client CurrentClipAmmo = %d, CurrentAmmo = %d"), InCurrentClipAmmo, InCurrentAmmo));
			FPSPlayerController->UpdateAmmo(InCurrentClipAmmo, InCurrentAmmo);
		}
		else
		{
			GThread::Get()->GetCoroutines().BindLambda(0.2f, [&](const int32 & InCurrentClipAmmo, const int32 & InCurrentAmmo)
				{
					UpdateAmmoForSuccessClient(InCurrentClipAmmo, InCurrentAmmo);
				}, InCurrentClipAmmo, InCurrentAmmo);
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientUpdateHealth_Implementation(const float& InHealth, const float& InMaxHealth)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		//TODO:BUG
		if (AFPSGamePlayerController * TmpFPSPlayerController = GetFPSPlayerControllerOnServer())
		{
			TmpFPSPlayerController->UpdateHealth(InHealth, InMaxHealth);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString(TEXT("[class AFPSGameCharacterBase] : ServerCallClientUpdateHealth, TmpFPSPlayerController Is Null")));
		}
	}
	else
	{
		if (FPSPlayerController)
		{
			FPSPlayerController->UpdateHealth(InHealth, InMaxHealth);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString(TEXT("[class AFPSGameCharacterBase] : ServerCallClientUpdateHealth, FPSPlayerController Is Null")));
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientReloadAnimation_Implementation()
{
	if (AWeaponBaseClient * CurClientWeapon = GetCurrentClientWeapon())
	{
		if (UAnimMontage * ClientArmReloadMontage = CurClientWeapon->ClientArmReloadMontage)
		{
			//手臂播放动画
			ClientArmAnimBP->Montage_Play(ClientArmReloadMontage);
			CurClientWeapon->PlayReloadAnimation();
		}
	}
}

void AFPSGameCharacterBase::ClientAiming_Implementation()
{
	//修改视角、添加瞄准UI、隐藏枪体和手臂
	if (ArmMesh)
	{
		ArmMesh->SetHiddenInGame(true);
	}

	if (WeaponPrimaryClient)
	{
		WeaponPrimaryClient->SetActorHiddenInGame(true);
		if (PlayerCamera)
		{
			PlayerCamera->SetFieldOfView(WeaponPrimaryClient->FieldOfView);
		}

		if (!WeaponPrimaryClient->SinperScope)
		{
			WeaponPrimaryClient->SinperScope = CreateWidget<UUserWidget>(GetWorld(), WeaponPrimaryClient->SinperScopeClass);
		}
		WeaponPrimaryClient->SinperScope->AddToViewport();
	}
}

void AFPSGameCharacterBase::ClientEndAiming_Implementation()
{
	if (ArmMesh)
	{
		ArmMesh->SetHiddenInGame(false);
	}

	if (WeaponPrimaryClient)
	{
		WeaponPrimaryClient->SetActorHiddenInGame(false);
		if (PlayerCamera)
		{
			PlayerCamera->SetFieldOfView(90);
		}

		if (WeaponPrimaryClient->SinperScope)
		{
			WeaponPrimaryClient->SinperScope->RemoveFromParent();
		}
	}
}

void AFPSGameCharacterBase::ClientCharacterDeath_Implementation()
{
	if (AWeaponBaseClient * ClientWeapon = GetCurrentClientWeapon())
	{
		ClientWeapon->Destroy();
	}
}

void AFPSGameCharacterBase::MulticastFire_Implementation()
{
	if (ClientBodyAnimBP)
	{
		if (AWeaponBaseServer * ServerWeapon = GetCurrentServerWeapon())
		{
			ClientBodyAnimBP->Montage_Play(ServerWeapon->ClientBodyFireMontage);
		}
	}
}

void AFPSGameCharacterBase::MulticastReload_Implementation()
{
	if (ClientBodyAnimBP)
	{
		if (AWeaponBaseServer * ServerWeapon = GetCurrentServerWeapon())
		{
			ClientBodyAnimBP->Montage_Play(ServerWeapon->ClientBodyReloadMontage);
		}
	}
}

void AFPSGameCharacterBase::MulticastBulletHole_Implementation(const FVector& InLocation, const FRotator& InRotation)
{
	if (AWeaponBaseServer * ServerWeapon = GetCurrentServerWeapon())
	{
		UDecalComponent* Decle = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ServerWeapon->BulletHoleDecal, FVector(8, 8, 8), InLocation, InRotation, 10.f);
		if (Decle)
		{
			//FadeScreenSize越小贴花显示的距离越远
			Decle->SetFadeScreenSize(0.001);
		}
	}
}

void AFPSGameCharacterBase::WeaponFirePressed()
{
	switch (ActiveWeapon)
	{
	case EWeaponType::AK47:
		{
			PrimaryWeaponFire();
			break;
		}
	case EWeaponType::M4A1:
		{
			PrimaryWeaponFire();
			break;
		}
	case EWeaponType::MP7:
		{
			PrimaryWeaponFire();
			break;
		}
	case EWeaponType::DESERTEAGLE:
		{
			SecondaryWeaponFire();
			break;
		}
	case EWeaponType::SNIPER:
		{
			SniperWeaponFire();
			break;
		}
	}
}

void AFPSGameCharacterBase::WeaponFireReleassed()
{
	switch (ActiveWeapon)
	{
	case EWeaponType::AK47:
		{
			PrimaryWeaponStopFire();
			break;
		}
	case EWeaponType::M4A1:
		{
			PrimaryWeaponStopFire();
			break;
		}
	case EWeaponType::MP7:
		{
			PrimaryWeaponStopFire();
			break;
		}
	case EWeaponType::DESERTEAGLE:
		{
			SecondaryWeaponStopFire();
			break;
		}
	case EWeaponType::SNIPER:
		{
			SniperWeaponStopFire();
			break;
		}
	}
}

void AFPSGameCharacterBase::WeaponAimingPressed()
{
	if (ActiveWeapon == EWeaponType::SNIPER)
	{
		//服务器修改IsAiming
		SetIsAimingOnServer(true);
		//客户端修改视角、添加瞄准UI、隐藏枪体和手臂
		ClientAiming();
	}
}

void AFPSGameCharacterBase::WeaponEndAimingReleassed()
{
	if (ActiveWeapon == EWeaponType::SNIPER)
	{
		//服务器修改IsAiming
		SetIsAimingOnServer(false);
		//客户端修改视角、移除瞄准UI、显示枪体和手臂
		ClientEndAiming();
	}
}

void AFPSGameCharacterBase::LowSpeedWalk()
{
	CharacterMovement->MaxWalkSpeed = 300.f;
	ChangeWalkWeaponOnServer(300.f);
}

void AFPSGameCharacterBase::NormalSpeedWalk()
{
	CharacterMovement->MaxWalkSpeed = 600.f;
	ChangeWalkWeaponOnServer(600.f);
}

void AFPSGameCharacterBase::AmmoReload()
{
	if (!IsFireing)
	{
		if (!IsReloading)
		{
			switch (ActiveWeapon)
			{
			case EWeaponType::AK47:
				{
					PrimaryWeaponReloadOnServer();
					break;
				}
			case EWeaponType::M4A1:
				{
					PrimaryWeaponReloadOnServer();
					break;
				}
			case EWeaponType::MP7:
				{
					PrimaryWeaponReloadOnServer();
					break;
				}
			case EWeaponType::DESERTEAGLE:
				{
					SecondaryWeaponReloadOnServer();
					break;
				}
			case EWeaponType::SNIPER:
				{
					PrimaryWeaponReloadOnServer();
					break;
				}
			}
		}
	}
}

void AFPSGameCharacterBase::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFPSGameCharacterBase::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFPSGameCharacterBase::TurnAtRate(float Rate)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : Current = %f"), Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()));
	//Standalone Game视角转动变慢
	float Tmp = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();

	AddControllerYawInput(Tmp);
}

void AFPSGameCharacterBase::LookUpAtRate(float Rate)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : Current = %f"), Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds()));
	float Tmp = Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
	//Standalone Game视角转动变慢
	AddControllerPitchInput(Tmp);
}

void AFPSGameCharacterBase::EquipPrimaryWeapon(AWeaponBaseServer* InWeaponBaseServer)
{
	if (WeaponPrimaryServer)
	{
		//切枪
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[class AFPSGameCharacterBase] : Equip Primary Weapon"));
		WeaponPrimaryServer = InWeaponBaseServer;
		WeaponPrimaryServer->SetOwner(this);

		WeaponPrimaryServer->K2_AttachToComponent(Mesh, TEXT("Weapon_Rifle"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	
		ServerCallClientEquipPrimaryWeapon();

		ServerCallClientUpdateAmmo(WeaponPrimaryServer->GetCurrentClipAmmo(), WeaponPrimaryServer->GetCurrentAmmo());
	}
}

void AFPSGameCharacterBase::EquipSecondaryWeapon(AWeaponBaseServer* InWeaponBaseServer)
{
	if (WeaponSecondaryServer)
	{
		//切枪
	}
	else
	{
		WeaponSecondaryServer = InWeaponBaseServer;
		WeaponSecondaryServer->SetOwner(this);

		WeaponSecondaryServer->K2_AttachToComponent(Mesh, TEXT("Weapon_Rifle"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

		ServerCallClientEquipSecondaryWeapon();

		ServerCallClientUpdateAmmo(WeaponSecondaryServer->GetCurrentClipAmmo(), WeaponSecondaryServer->GetCurrentAmmo());
	}
}

void AFPSGameCharacterBase::PrimaryWeaponFire()
{
	if (WeaponPrimaryServer)
	{
		//子弹
		if (WeaponPrimaryServer->GetCurrentClipAmmo() > 0 && !IsReloading)
		{
			//I.服务器：播放射击音效、枪口火花、减少弹药、射线检测、应用伤害、弹孔生成
			if (UKismetMathLibrary::VSize(GetVelocity()) > 0.1f)
			{
				RifleWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), true);
			}
			else
			{
				RifleWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), false);
			}

			//II.客户端：枪体播放动画、手臂播放动画、播放射击音效、屏幕抖动、后坐力、枪口火花
			ServerCallClientFireWeapon();
			ServerCallClientWeaponRecoil();

			if (WeaponPrimaryServer->IsAutomaticWeapon())
			{
				//III.连击
				GetWorldTimerManager().SetTimer(AutomaticFireTimerHandle, this, &AFPSGameCharacterBase::AutomaticFire, WeaponPrimaryServer->GetAutomaticFireRate(), true);
			}
		}
	}
}

void AFPSGameCharacterBase::PrimaryWeaponStopFire()
{
	//Server清除开火标记
	StopFireingOnServer();

	//清除连击计时器
	GetWorldTimerManager().ClearTimer(AutomaticFireTimerHandle);
	
	//重置后坐力
	ResetRecoil();
	
	//弹头延时销毁
}

void AFPSGameCharacterBase::RifleLineTrace(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing)
{
	FVector EndLocation;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	FHitResult HitResult;

	if (WeaponPrimaryServer)
	{
		if (IsMoveing)
		{
			EndLocation = InCamreaLocation + UKismetMathLibrary::GetForwardVector(InCameraRotation) * WeaponPrimaryServer->BulletDistance;
			float RandomX = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponPrimaryServer->GetMoveingFireRandomRange(), WeaponPrimaryServer->GetMoveingFireRandomRange());
			float RandomY = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponPrimaryServer->GetMoveingFireRandomRange(), WeaponPrimaryServer->GetMoveingFireRandomRange());
			float RandomZ = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponPrimaryServer->GetMoveingFireRandomRange(), WeaponPrimaryServer->GetMoveingFireRandomRange());
			EndLocation.X += RandomX;
			EndLocation.Y += RandomY;
			EndLocation.Z += RandomZ;
		}
		else
		{
			EndLocation = InCamreaLocation + UKismetMathLibrary::GetForwardVector(InCameraRotation) * WeaponPrimaryServer->BulletDistance;
		}
	}
	
	bool HitSuccess = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		InCamreaLocation,
		EndLocation,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::Persistent,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f);

	if (HitSuccess)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *HitResult.Actor->GetName()));
		if (AFPSGameCharacterBase * DamageCharacter = Cast<AFPSGameCharacterBase>(HitResult.Actor))
		{
			//应用伤害（Server）
			DamagePlayer(HitResult.PhysMaterial.Get(), HitResult.Actor.Get(), InCamreaLocation, HitResult);
		}
		else
		{
			//生成弹孔（Multicast）
			FRotator XRotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			MulticastBulletHole(HitResult.Location, XRotation);
		}
	}
}

void AFPSGameCharacterBase::SniperWeaponFire()
{
	if (WeaponPrimaryServer)
	{
		//子弹
		if (WeaponPrimaryServer->GetCurrentClipAmmo() > 0 && !IsReloading && !IsFireing)
		{
			//I.服务器：播放射击音效、枪口火花、减少弹药、射线检测、应用伤害、弹孔生成
			if (UKismetMathLibrary::VSize(GetVelocity()) > 0.1f)
			{
				SniperWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), true);
			}
			else
			{
				SniperWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), false);
			}

			//II.客户端：枪体播放动画、手臂播放动画、播放射击音效、屏幕抖动、后坐力、枪口火花
			ServerCallClientFireWeapon();
		}
	}
}

void AFPSGameCharacterBase::SniperWeaponStopFire()
{
}

void AFPSGameCharacterBase::SniperLineTrace(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing)
{
	FVector EndLocation;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	FHitResult HitResult;

	if (WeaponPrimaryServer)
	{
		if (IsAiming)
		{
			if (IsMoveing)
			{
				EndLocation = InCamreaLocation + UKismetMathLibrary::GetForwardVector(InCameraRotation) * WeaponPrimaryServer->BulletDistance;
				float RandomX = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponPrimaryServer->GetMoveingFireRandomRange(), WeaponPrimaryServer->GetMoveingFireRandomRange());
				float RandomY = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponPrimaryServer->GetMoveingFireRandomRange(), WeaponPrimaryServer->GetMoveingFireRandomRange());
				float RandomZ = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponPrimaryServer->GetMoveingFireRandomRange(), WeaponPrimaryServer->GetMoveingFireRandomRange());
				EndLocation.X += RandomX;
				EndLocation.Y += RandomY;
				EndLocation.Z += RandomZ;
			}
			else
			{
				EndLocation = InCamreaLocation + UKismetMathLibrary::GetForwardVector(InCameraRotation) * WeaponPrimaryServer->BulletDistance;
			}

			ClientEndAiming();
		}
		else
		{
			EndLocation = InCamreaLocation + UKismetMathLibrary::GetForwardVector(InCameraRotation) * WeaponPrimaryServer->BulletDistance;
			float RandomX = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponPrimaryServer->GetMoveingFireRandomRange(), WeaponPrimaryServer->GetMoveingFireRandomRange());
			float RandomY = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponPrimaryServer->GetMoveingFireRandomRange(), WeaponPrimaryServer->GetMoveingFireRandomRange());
			float RandomZ = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponPrimaryServer->GetMoveingFireRandomRange(), WeaponPrimaryServer->GetMoveingFireRandomRange());
			EndLocation.X += RandomX;
			EndLocation.Y += RandomY;
			EndLocation.Z += RandomZ;
		}
	}

	bool HitSuccess = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		InCamreaLocation,
		EndLocation,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::Persistent,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f);

	if (HitSuccess)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *HitResult.Actor->GetName()));
		if (AFPSGameCharacterBase * DamageCharacter = Cast<AFPSGameCharacterBase>(HitResult.Actor))
		{
			//应用伤害（Server）
			DamagePlayer(HitResult.PhysMaterial.Get(), HitResult.Actor.Get(), InCamreaLocation, HitResult);
		}
		else
		{
			//生成弹孔（Multicast）
			FRotator XRotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			MulticastBulletHole(HitResult.Location, XRotation);
		}
	}
}

void AFPSGameCharacterBase::SecondaryWeaponFire()
{
	if (WeaponSecondaryServer)
	{
		//子弹
		if (WeaponSecondaryServer->GetCurrentClipAmmo() > 0 && !IsReloading)
		{
			//I.服务器：播放射击音效、枪口火花、减少弹药、射线检测、应用伤害、弹孔生成
			if (UKismetMathLibrary::VSize(GetVelocity()) > 0.1f)
			{
				PsitolWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), true);
			}
			else
			{
				PsitolWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), false);
			}

			//II.客户端：枪体播放动画、手臂播放动画、播放射击音效、屏幕抖动、后坐力、枪口火花
			ServerCallClientFireWeapon();
		}
	}
}

void AFPSGameCharacterBase::SecondaryWeaponStopFire()
{
	//Server清除开火标记
	StopFireingOnServer();
}

void AFPSGameCharacterBase::PistolLineTrace(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing)
{
	FVector EndLocation;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	FHitResult HitResult;

	if (WeaponSecondaryServer)
	{
		if (IsMoveing)
		{
			FRotator NewRotation;
			NewRotation.Roll = InCameraRotation.Roll;
			NewRotation.Pitch = InCameraRotation.Pitch + FMath::RandRange(PsitolRecoilMin, PsitolRecoilMax);
			NewRotation.Yaw = InCameraRotation.Yaw + FMath::RandRange(PsitolRecoilMin, PsitolRecoilMax);

			EndLocation = InCamreaLocation + UKismetMathLibrary::GetForwardVector(NewRotation) * WeaponSecondaryServer->BulletDistance;
			float RandomX = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponSecondaryServer->GetMoveingFireRandomRange(), WeaponSecondaryServer->GetMoveingFireRandomRange());
			float RandomY = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponSecondaryServer->GetMoveingFireRandomRange(), WeaponSecondaryServer->GetMoveingFireRandomRange());
			float RandomZ = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponSecondaryServer->GetMoveingFireRandomRange(), WeaponSecondaryServer->GetMoveingFireRandomRange());
			EndLocation.X += RandomX;
			EndLocation.Y += RandomY;
			EndLocation.Z += RandomZ;
		}
		else
		{
			FRotator NewRotation;
			NewRotation.Roll = InCameraRotation.Roll;
			NewRotation.Pitch = InCameraRotation.Pitch + FMath::RandRange(PsitolRecoilMin, PsitolRecoilMax);
			NewRotation.Yaw = InCameraRotation.Yaw + FMath::RandRange(PsitolRecoilMin, PsitolRecoilMax);
			EndLocation = InCamreaLocation + UKismetMathLibrary::GetForwardVector(NewRotation) * WeaponSecondaryServer->BulletDistance;
		}
	}

	PsitolRecoilMin -= WeaponSecondaryServer->PsitolRecoilRate;
	PsitolRecoilMax += WeaponSecondaryServer->PsitolRecoilRate;

	bool HitSuccess = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		InCamreaLocation,
		EndLocation,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::Persistent,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f);

	if (HitSuccess)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *HitResult.Actor->GetName()));
		if (AFPSGameCharacterBase * DamageCharacter = Cast<AFPSGameCharacterBase>(HitResult.Actor))
		{
			//应用伤害（Server）
			DamagePlayer(HitResult.PhysMaterial.Get(), HitResult.Actor.Get(), InCamreaLocation, HitResult);
		}
		else
		{
			//生成弹孔（Multicast）
			FRotator XRotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			MulticastBulletHole(HitResult.Location, XRotation);
		}
	}
}

void AFPSGameCharacterBase::AutomaticFire()
{
	if (WeaponPrimaryServer)
	{
		//子弹
		if (WeaponPrimaryServer->GetCurrentClipAmmo() > 0)
		{
			//I.服务器：播放射击音效、枪口火花、减少弹药、射线检测、应用伤害、弹孔生成
			if (UKismetMathLibrary::VSize(GetVelocity()) > 0.1f)
			{
				RifleWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), true);
			}
			else
			{
				RifleWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), false);
			}

			//II.客户端：枪体播放动画、手臂播放动画、播放射击音效、屏幕抖动、后坐力、枪口火花
			ServerCallClientFireWeapon();
			ServerCallClientWeaponRecoil();
		}
		else
		{
			PrimaryWeaponStopFire();
		}
	}
}

void AFPSGameCharacterBase::ResetRecoil()
{
	NewVerticalRecoilValue = 0;
	OldVerticalRecoilValue = 0;
	VerticalRecoilDiffValue = 0;
	CurveXRecoilValue = 0;
	NewHorizontalRecoilValue = 0;
	OldHorizontalRecoilValue = 0;
	HorizontalRecoilDiffValue = 0;
}

void AFPSGameCharacterBase::ReloadDelayCallBack()
{
	if (AWeaponBaseServer * ServerWeapon = GetCurrentServerWeapon())
	{
		//换弹
		ServerWeapon->ReloadAmmo();

		//更新UI
		ServerCallClientUpdateAmmo(
			ServerWeapon->GetCurrentClipAmmo(),
			ServerWeapon->GetCurrentAmmo());

		//清除换弹标志
		IsReloading = false;
	}
}

void AFPSGameCharacterBase::SniperFireDelayCallBack()
{
	IsFireing = false;
}

void AFPSGameCharacterBase::UpdateAmmoForSuccessServer(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo)
{
	if (AFPSGamePlayerController * TmpFPSPlayerController = GetFPSPlayerControllerOnServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("[class AFPSGameCharacterBase] : Authority CurrentClipAmmo = %d, CurrentAmmo = %d"), InCurrentClipAmmo, InCurrentAmmo);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : Authority CurrentClipAmmo = %d, CurrentAmmo = %d"), InCurrentClipAmmo, InCurrentAmmo));
		TmpFPSPlayerController->UpdateAmmo(InCurrentClipAmmo, InCurrentAmmo);
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&](const int32 & InCurrentClipAmmo, const int32 & InCurrentAmmo)
			{
				UpdateAmmoForSuccessServer(InCurrentClipAmmo, InCurrentAmmo);
			}, InCurrentClipAmmo, InCurrentAmmo);
	}
}

void AFPSGameCharacterBase::UpdateAmmoForSuccessClient(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo)
{
	if (FPSPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[class AFPSGameCharacterBase] : Client CurrentClipAmmo = %d, CurrentAmmo = %d"), InCurrentClipAmmo, InCurrentAmmo);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : Client CurrentClipAmmo = %d, CurrentAmmo = %d"), InCurrentClipAmmo, InCurrentAmmo));
		FPSPlayerController->UpdateAmmo(InCurrentClipAmmo, InCurrentAmmo);
	}
	else
	{
		FPSPlayerController = Cast<AFPSGamePlayerController>(GetController());
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&](const int32 & InCurrentClipAmmo, const int32 & InCurrentAmmo)
			{
				UpdateAmmoForSuccessClient(InCurrentClipAmmo, InCurrentAmmo);
			}, InCurrentClipAmmo, InCurrentAmmo);
	}
}

void AFPSGameCharacterBase::DamagePlayer(UPhysicalMaterial* InPhysicsMaterial, AActor* InDamageActor, FVector InDamageFromDrection, FHitResult& InHitResult)
{
	if (AWeaponBaseServer * WeaponServer = GetCurrentServerWeapon())
	{
		float DamageVal = WeaponServer->BaseDamage;

		switch (InPhysicsMaterial->SurfaceType)
		{
		case EPhysicalSurface::SurfaceType1:	//Head
		{
			DamageVal *= 4;
			break;
		}
		case EPhysicalSurface::SurfaceType2:	//Body
		{
			DamageVal *= 1;
			break;
		}
		case EPhysicalSurface::SurfaceType3:	//Leg
		{
			DamageVal *= 0.7;
			break;
		}
		case EPhysicalSurface::SurfaceType4:	//Arm
		{
			DamageVal *= 0.8;
			break;
		}
		}

		UGameplayStatics::ApplyPointDamage(
			InDamageActor,
			DamageVal,
			InDamageFromDrection,
			InHitResult,
			GetController(),
			this,
			UDamageType::StaticClass());
	}
}

void AFPSGameCharacterBase::CharacterDeath()
{
	if (AWeaponBaseServer * ServerWeapon = GetCurrentServerWeapon())
	{
		ServerWeapon->Destroy();
	}
	if (AWeaponBaseClient * ClientWeapon = GetCurrentClientWeapon())
	{
		ClientWeapon->Destroy();
	}
	//销毁客户端的武器
	ClientCharacterDeath();

	if (AFPSGamePlayerState * MyPlayerState = Cast<AFPSGamePlayerState>(GetPlayerState()))
	{
		MyPlayerState->DeathResetData();
	}
}

void AFPSGameCharacterBase::OnHit(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (AFPSGamePlayerState * MyPlayerState = Cast<AFPSGamePlayerState>(GetPlayerState()))
	{
		MyPlayerState->Health = FMath::Clamp(MyPlayerState->Health - Damage, 0.f, MyPlayerState->MaxHealth);
		ServerCallClientUpdateHealth(MyPlayerState->Health, MyPlayerState->MaxHealth);
		
		if (MyPlayerState->Health <= 0)
		{
			//死亡
			if (AFPSGamePlayerController* FPSPC = Cast<AFPSGamePlayerController>(GetController()))
			{
				CharacterDeath();
				FPSPC->ControllerCharacterDeath(DamageCauser);
			}
		}
	}
}

AFPSGamePlayerController* AFPSGameCharacterBase::GetFPSPlayerControllerOnServer()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AFPSGamePlayerController * TmpPlayerController = Cast<AFPSGamePlayerController>(It->Get()))
		{
			if (this == Cast<AFPSGameCharacterBase>(TmpPlayerController->GetPawn()))
			{
				return TmpPlayerController;
			}
		}
	}
	return nullptr;
}

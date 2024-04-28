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
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(PlayerCamera);
	//���Լ��ɼ�
	ArmMesh->SetOnlyOwnerSee(true);

	//�Լ����ɼ�������ѯ������ΪPawn
	Mesh->SetOwnerNoSee(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	WeaponPrimaryServer = nullptr;
	WeaponPrimaryClient = nullptr;
	WeaponSecondServer = nullptr;
	WeaponSecondClient = nullptr;

}

void AFPSGameCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSGameCharacterBase, IsFireing, COND_None);
	DOREPLIFETIME_CONDITION(AFPSGameCharacterBase, IsReloading, COND_None);
	DOREPLIFETIME_CONDITION(AFPSGameCharacterBase, ActiveWeapon, COND_None);
}

void AFPSGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	StartWeapon();

	ClientArmAnimBP = ArmMesh->GetAnimInstance();
	ClientBodyAnimBP = Mesh->GetAnimInstance();
	FPSPlayerController = Cast<AFPSGamePlayerController>(GetController());

	OnTakePointDamage.AddDynamic(this, &AFPSGameCharacterBase::OnHit);
}

void AFPSGameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
			//����
			UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/Ak47/BP_Ak47_Server.BP_Ak47_Server_c'"));
			AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);
			
			//���õ�ǰ������������
			ActiveWeapon = EWeaponType::AK47;
			
			//Ԥ����װ����ǰ��������
			ServerWeapon->EquipWeapon();
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
	case EWeaponType::M4A1:
		{
			//����
			UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/M4A1/BP_M4A1_Server.BP_M4A1_Server_c'"));
			AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);
			
			//���õ�ǰ������������
			ActiveWeapon = EWeaponType::M4A1;

			//Ԥ����װ����ǰ��������
			ServerWeapon->EquipWeapon();
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
	case EWeaponType::MP7:
		{
			//����
			UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/MP7/BP_MP7_Server.BP_MP7_Server_c'"));
			AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);

			//���õ�ǰ������������
			ActiveWeapon = EWeaponType::MP7;

			//Ԥ����װ����ǰ��������
			ServerWeapon->EquipWeapon();
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
	case EWeaponType::DESERTEAGLE:
		{
			//����
			UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/DesertEagle/BP_DesertEagle_Server.BP_DesertEagle_Server_c'"));
			AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);

			//���õ�ǰ������������
			ActiveWeapon = EWeaponType::DESERTEAGLE;

			//Ԥ����װ����ǰ��������
			ServerWeapon->EquipWeapon();
			EquipSecondaryWeapon(ServerWeapon);
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
			return WeaponSecondClient;
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
			return WeaponSecondServer;
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
	//Server���ÿ�����
	IsFireing = true;

	if (WeaponPrimaryServer)
	{
		//�㲥ǹ�ڻ�(�ಥ)
		WeaponPrimaryServer->MulticastFireEffect();
		WeaponPrimaryServer->ExpendAmmo();

		//�����˳������������(�ಥ)
		MulticastFire();
		
		ServerCallClientUpdateAmmo(WeaponPrimaryServer->GetCurrentClipAmmo(), WeaponPrimaryServer->GetCurrentAmmo());
	}

	RifleLineTrace(InCamreaLocation, InCameraRotation, IsMoveing);
}

void AFPSGameCharacterBase::PsitolWeaponFireOnServer_Implementation(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing)
{
	//Server���ÿ�����
	IsFireing = true;

	if (WeaponSecondServer)
	{
		//�㲥ǹ�ڻ�(�ಥ)
		WeaponSecondServer->MulticastFireEffect();
		WeaponSecondServer->ExpendAmmo();

		//�����˳������������(�ಥ)
		MulticastFire();

		ServerCallClientUpdateAmmo(WeaponSecondServer->GetCurrentClipAmmo(), WeaponSecondServer->GetCurrentAmmo());
	}

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
				//���û�����־
				IsReloading = true;

				//�ͻ����ֱ۶������������ಥ���嶯�������ݸ��¡�UI����
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
		if (WeaponSecondServer)
		{
			if (WeaponSecondServer->GetCurrentAmmo() > 0 && WeaponSecondServer->GetCurrentClipAmmo() < WeaponSecondServer->GetMaxClipAmmo())
			{
				//���û�����־
				IsReloading = true;

				//�ͻ����ֱ۶������������ಥ���嶯�������ݸ��¡�UI����
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

			FName SocketName = TEXT("WeaponSocket");
			if (ActiveWeapon == EWeaponType::M4A1)
			{
				SocketName = TEXT("M4A1WeaponSocket");
			}
			else if (ActiveWeapon == EWeaponType::MP7)
			{
				SocketName = TEXT("MP7WeaponSocket");
			}

			if (WeaponPrimaryClient)
			{
				WeaponPrimaryClient->K2_AttachToComponent(ArmMesh, SocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

				//�����ֱ۶���
				GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
					{
						UpdateFPSArmAnimBlend(WeaponPrimaryClient->ArmAnimBlendIndex);
					});
			}
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientEquipSecondaryWeapon_Implementation()
{
	if (WeaponSecondServer)
	{
		if (WeaponSecondClient)
		{

		}
		else
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			WeaponSecondClient = GetWorld()->SpawnActor<AWeaponBaseClient>(WeaponSecondServer->ClientWeaponClass, GetActorTransform(), SpawnInfo);

			FName SocketName = TEXT("DesertEagleWeaponSocket");
			if (WeaponSecondClient)
			{
				WeaponSecondClient->K2_AttachToComponent(ArmMesh, SocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

				//�����ֱ۶���
				GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
					{
						UpdateFPSArmAnimBlend(WeaponSecondClient->ArmAnimBlendIndex);
					});
			}			
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientFireWeapon_Implementation()
{
	if (AWeaponBaseClient * CurClientWeapon = GetCurrentClientWeapon())
	{
		//ǹ�岥�Ŷ���
		CurClientWeapon->PlayShootAnimation();
		//����Ч��
		CurClientWeapon->DisplayWeaponEffect();

		if (UAnimMontage * ClientArmFireMontage = CurClientWeapon->ClientArmFireMontage)
		{
			//�ֱ۲��Ŷ���
			ClientArmAnimBP->Montage_SetPlayRate(ClientArmFireMontage, 1.f);
			ClientArmAnimBP->Montage_Play(ClientArmFireMontage);
		}

		if (FPSPlayerController)
		{
			//��Ļ����
			FPSPlayerController->PlayerCameraShake(CurClientWeapon->CameraShakeClass);

			//׼�Ƕ���
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
			TmpFPSPlayerController->UpdateAmmo(InCurrentClipAmmo, InCurrentAmmo);
		}
	}
	else
	{
		if (FPSPlayerController)
		{
			FPSPlayerController->UpdateAmmo(InCurrentClipAmmo, InCurrentAmmo);
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientUpdateHealth_Implementation(const float& InHealth, const float& InMaxHealth)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (AFPSGamePlayerController * TmpFPSPlayerController = GetFPSPlayerControllerOnServer())
		{
			TmpFPSPlayerController->UpdateHealth(InHealth, InMaxHealth);
		}
	}
	else
	{
		if (FPSPlayerController)
		{
			FPSPlayerController->UpdateHealth(InHealth, InMaxHealth);
		}
	}
}

void AFPSGameCharacterBase::ServerCallClientReloadAnimation_Implementation()
{
	if (AWeaponBaseClient * CurClientWeapon = GetCurrentClientWeapon())
	{
		if (UAnimMontage * ClientArmReloadMontage = CurClientWeapon->ClientArmReloadMontage)
		{
			//�ֱ۲��Ŷ���
			ClientArmAnimBP->Montage_Play(ClientArmReloadMontage);
			CurClientWeapon->PlayReloadAnimation();
		}
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
			//FadeScreenSizeԽС������ʾ�ľ���ԽԶ
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
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSGameCharacterBase::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFPSGameCharacterBase::EquipPrimaryWeapon(AWeaponBaseServer* InWeaponBaseServer)
{
	if (WeaponPrimaryServer)
	{
		//��ǹ
	}
	else
	{
		WeaponPrimaryServer = InWeaponBaseServer;
		WeaponPrimaryServer->SetOwner(this);

		WeaponPrimaryServer->K2_AttachToComponent(Mesh, TEXT("Weapon_Rifle"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	
		ServerCallClientEquipPrimaryWeapon();

		ServerCallClientUpdateAmmo(WeaponPrimaryServer->GetCurrentClipAmmo(), WeaponPrimaryServer->GetCurrentAmmo());
	}
}

void AFPSGameCharacterBase::EquipSecondaryWeapon(AWeaponBaseServer* InWeaponBaseServer)
{
	if (WeaponSecondServer)
	{
		//��ǹ
	}
	else
	{
		WeaponSecondServer = InWeaponBaseServer;
		WeaponSecondServer->SetOwner(this);

		WeaponSecondServer->K2_AttachToComponent(Mesh, TEXT("Weapon_Rifle"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

		ServerCallClientEquipSecondaryWeapon();

		ServerCallClientUpdateAmmo(WeaponSecondServer->GetCurrentClipAmmo(), WeaponSecondServer->GetCurrentAmmo());
	}
}

void AFPSGameCharacterBase::PrimaryWeaponFire()
{
	if (WeaponPrimaryServer)
	{
		//�ӵ�
		if (WeaponPrimaryServer->GetCurrentClipAmmo() > 0 && !IsReloading)
		{
			//I.�����������������Ч��ǹ�ڻ𻨡����ٵ�ҩ�����߼�⡢Ӧ���˺�����������
			if (UKismetMathLibrary::VSize(GetVelocity()) > 0.1f)
			{
				RifleWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), true);
			}
			else
			{
				RifleWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), false);
			}

			//II.�ͻ��ˣ�ǹ�岥�Ŷ������ֱ۲��Ŷ��������������Ч����Ļ��������������ǹ�ڻ�
			ServerCallClientFireWeapon();
			ServerCallClientWeaponRecoil();

			if (WeaponPrimaryServer->IsAutomaticWeapon())
			{
				//III.����
				GetWorldTimerManager().SetTimer(AutomaticFireTimerHandle, this, &AFPSGameCharacterBase::AutomaticFire, WeaponPrimaryServer->GetAutomaticFireRate(), true);
			}
		}
	}
}

void AFPSGameCharacterBase::PrimaryWeaponStopFire()
{
	//Server���������
	StopFireingOnServer();

	//���������ʱ��
	GetWorldTimerManager().ClearTimer(AutomaticFireTimerHandle);
	
	//���ú�����
	ResetRecoil();
	
	//��ͷ��ʱ����
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
			//Ӧ���˺���Server��
			DamagePlayer(HitResult.PhysMaterial.Get(), HitResult.Actor.Get(), InCamreaLocation, HitResult);
		}
		else
		{
			//���ɵ��ף�Multicast��
			FRotator XRotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			MulticastBulletHole(HitResult.Location, XRotation);
		}
	}
}

void AFPSGameCharacterBase::SecondaryWeaponFire()
{
	if (WeaponSecondServer)
	{
		//�ӵ�
		if (WeaponSecondServer->GetCurrentClipAmmo() > 0 && !IsReloading)
		{
			//I.�����������������Ч��ǹ�ڻ𻨡����ٵ�ҩ�����߼�⡢Ӧ���˺�����������
			if (UKismetMathLibrary::VSize(GetVelocity()) > 0.1f)
			{
				PsitolWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), true);
			}
			else
			{
				PsitolWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), false);
			}

			//II.�ͻ��ˣ�ǹ�岥�Ŷ������ֱ۲��Ŷ��������������Ч����Ļ��������������ǹ�ڻ�
			ServerCallClientFireWeapon();
		}
	}
}

void AFPSGameCharacterBase::SecondaryWeaponStopFire()
{
	//Server���������
	StopFireingOnServer();
}

void AFPSGameCharacterBase::PistolLineTrace(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing)
{
	FVector EndLocation;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	FHitResult HitResult;

	if (WeaponSecondServer)
	{
		if (IsMoveing)
		{
			EndLocation = InCamreaLocation + UKismetMathLibrary::GetForwardVector(InCameraRotation) * WeaponSecondServer->BulletDistance;
			float RandomX = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponSecondServer->GetMoveingFireRandomRange(), WeaponSecondServer->GetMoveingFireRandomRange());
			float RandomY = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponSecondServer->GetMoveingFireRandomRange(), WeaponSecondServer->GetMoveingFireRandomRange());
			float RandomZ = UKismetMathLibrary::RandomFloatInRange(-1 * WeaponSecondServer->GetMoveingFireRandomRange(), WeaponSecondServer->GetMoveingFireRandomRange());
			EndLocation.X += RandomX;
			EndLocation.Y += RandomY;
			EndLocation.Z += RandomZ;
		}
		else
		{
			EndLocation = InCamreaLocation + UKismetMathLibrary::GetForwardVector(InCameraRotation) * WeaponSecondServer->BulletDistance;
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
			//Ӧ���˺���Server��
			DamagePlayer(HitResult.PhysMaterial.Get(), HitResult.Actor.Get(), InCamreaLocation, HitResult);
		}
		else
		{
			//���ɵ��ף�Multicast��
			FRotator XRotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			MulticastBulletHole(HitResult.Location, XRotation);
		}
	}
}

void AFPSGameCharacterBase::AutomaticFire()
{
	if (WeaponPrimaryServer)
	{
		//�ӵ�
		if (WeaponPrimaryServer->GetCurrentClipAmmo() > 0)
		{
			//I.�����������������Ч��ǹ�ڻ𻨡����ٵ�ҩ�����߼�⡢Ӧ���˺�����������
			if (UKismetMathLibrary::VSize(GetVelocity()) > 0.1f)
			{
				RifleWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), true);
			}
			else
			{
				RifleWeaponFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), false);
			}

			//II.�ͻ��ˣ�ǹ�岥�Ŷ������ֱ۲��Ŷ��������������Ч����Ļ��������������ǹ�ڻ�
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
		//����
		ServerWeapon->ReloadAmmo();

		//����UI
		ServerCallClientUpdateAmmo(
			ServerWeapon->GetCurrentClipAmmo(),
			ServerWeapon->GetCurrentAmmo());

		//���������־
		IsReloading = false;
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

void AFPSGameCharacterBase::OnHit(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (AFPSGamePlayerState * MyPlayerState = Cast<AFPSGamePlayerState>(GetPlayerState()))
	{
		MyPlayerState->Health = FMath::Clamp(MyPlayerState->Health - Damage, 0.f, MyPlayerState->MaxHealth);
		ServerCallClientUpdateHealth(MyPlayerState->Health, MyPlayerState->MaxHealth);
		if (MyPlayerState->Health <= 0)
		{
			//����
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

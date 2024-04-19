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

AFPSGameCharacterBase::AFPSGameCharacterBase()
	:BaseTurnRate(45.f)
	,BaseLookUpRate(45.f)
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
		PurchaseWeapon(EWeaponType::AK47);
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
		UClass* WeaponClass = StaticLoadClass(AWeaponBaseServer::StaticClass(), nullptr, TEXT("Blueprint'/Game/BP/FPS/Weapon/Ak47/BP_Ak47_Server.BP_Ak47_Server_c'"));
		AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(WeaponClass, GetActorTransform(), SpawnInfo);
		ServerWeapon->EquipWeapon();
		EquipPrimaryWeapon(ServerWeapon);
		break;
	}
	case EWeaponType::DESERTEAGLE:
	{

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
	}
	return nullptr;
}

void AFPSGameCharacterBase::ChangeWalkWpeedOnServer_Implementation(float InValue)
{
	CharacterMovement->MaxWalkSpeed = InValue;
}

void AFPSGameCharacterBase::RifleWpeedFireOnServer_Implementation(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing)
{
	if (WeaponPrimaryServer)
	{
		//广播枪口火花(多播)
		WeaponPrimaryServer->MulticastFireEffect();
		WeaponPrimaryServer->ExpendAmmo();

		//第三人称身体射击动画(多播)
		MulticastFire();
		
		ServerCallClientUpdateAmmo(WeaponPrimaryServer->GetCurrentClipAmmo(), WeaponPrimaryServer->GetCurrentAmmo());

		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Server CurrentClipAmmo : %d"), WeaponPrimaryServer->GetCurrentClipAmmo()));
	}

	RifleLineTrace(InCamreaLocation, InCameraRotation, IsMoveing);
}

void AFPSGameCharacterBase::ServerCallClientEquipPrimaryWeapon_Implementation()
{
	if (WeaponPrimaryServer)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		WeaponPrimaryClient = GetWorld()->SpawnActor<AWeaponBaseClient>(WeaponPrimaryServer->ClientWeaponClass, GetActorTransform(), SpawnInfo);
		
		WeaponPrimaryClient->K2_AttachToComponent(ArmMesh, TEXT("WeaponSocket"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

		//调整手臂动画
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

void AFPSGameCharacterBase::MulticastFire_Implementation()
{
	if (ClientBodyAnimBP)
	{
		if (WeaponPrimaryServer)
		{
			ClientBodyAnimBP->Montage_Play(WeaponPrimaryServer->ClientBodyFireMontage);
		}
	}
}

void AFPSGameCharacterBase::MulticastBulletHole_Implementation(const FVector& InLocation, const FRotator& InRotation)
{
	if (WeaponPrimaryServer)
	{
		UDecalComponent* Decle = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), WeaponPrimaryServer->BulletHoleDecal, FVector(8, 8, 8), InLocation, InRotation, 10.f);
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
	case EWeaponType::DESERTEAGLE:
	{
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
	case EWeaponType::DESERTEAGLE:
	{
		break;
	}
	}
}

void AFPSGameCharacterBase::LowSpeedWalk()
{
	CharacterMovement->MaxWalkSpeed = 300.f;
	ChangeWalkWpeedOnServer(300.f);
}

void AFPSGameCharacterBase::NormalSpeedWalk()
{
	CharacterMovement->MaxWalkSpeed = 600.f;
	ChangeWalkWpeedOnServer(600.f);
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
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("%f"), Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()));
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

void AFPSGameCharacterBase::PrimaryWeaponFire()
{
	//子弹
	if (WeaponPrimaryServer->GetCurrentClipAmmo() > 0)
	{
		//I.服务器：播放射击音效、枪口火花、减少弹药、射线检测、应用伤害、弹孔生成
		RifleWpeedFireOnServer(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentRotation(), false);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Client CurrentClipAmmo : %d"), WeaponPrimaryServer->GetCurrentClipAmmo()));

		//II.客户端：枪体播放动画、手臂播放动画、播放射击音效、屏幕抖动、后坐力、枪口火花
		ServerCallClientFireWeapon();

		//III.连击
	}
}

void AFPSGameCharacterBase::PrimaryWeaponStopFire()
{
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
		FLinearColor::Green,
		FLinearColor::Red,
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

void AFPSGameCharacterBase::DamagePlayer(UPhysicalMaterial* InPhysicsMaterial, AActor* InDamageActor, FVector InDamageFromDrection, FHitResult& InHitResult)
{
	if (WeaponPrimaryServer)
	{
		float DamageVal = WeaponPrimaryServer->BaseDamage;

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
		//MyPlayerState->Health = FMath::Clamp(MyPlayerState->Health - Damage, 0.f, MyPlayerState->MaxHealth);
		MyPlayerState->Health -= Damage;
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Health : %f"), MyPlayerState->Health));
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

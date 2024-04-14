// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponBaseServer.h"
#include "WeaponBaseClient.h"
#include "Camera/CameraComponent.h"

AFPSGameCharacterBase::AFPSGameCharacterBase()
	:BaseTurnRate(45.f)
	,BaseLookUpRate(45.f)
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
}

void AFPSGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	StartWeapon();

	ClientArmAnimBP = ArmMesh->GetAnimInstance();
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

void AFPSGameCharacterBase::ServerCallClientEquipPrimaryWeapon_Implementation()
{
	if (WeaponPrimaryServer)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		WeaponPrimaryClient = GetWorld()->SpawnActor<AWeaponBaseClient>(WeaponPrimaryServer->ClientWeaponClass, GetActorTransform(), SpawnInfo);
		
		WeaponPrimaryClient->K2_AttachToComponent(ArmMesh, TEXT("WeaponSocket"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		
		//�����ֱ۶���
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
	}
}

void AFPSGameCharacterBase::PrimaryWeaponFire()
{
	//UE_LOG(LogTemp, Warning, TEXT("Call PrimaryWeaponFire"));
	//I.�����������ٵ�ҩ�����߼�⡢Ӧ���˺�����������

	//II.�ͻ��ˣ�ǹ�岥�Ŷ������ֱ۲��Ŷ��������������Ч����Ļ��������������ǹ�ڻ�
	ServerCallClientFireWeapon();
}

void AFPSGameCharacterBase::PrimaryWeaponStopFire()
{
	//��ͷ��ʱ����
}

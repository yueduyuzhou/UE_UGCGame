// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBaseServer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FPSGameCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "ThreadManage.h"

AWeaponBaseServer::AWeaponBaseServer()
	:WeaponType(EWeaponType::AK47)
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetOwnerNoSee(true); 

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetSimulatePhysics(true);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBaseServer::OnAttackerBeginOverlap);

	//生成时自动同步
	SetReplicates(true);
}

void AWeaponBaseServer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeaponBaseServer, CurrentClipAmmo, COND_None);
	DOREPLIFETIME_CONDITION(AWeaponBaseServer, ReplicatedLocation, COND_None);
	DOREPLIFETIME_CONDITION(AWeaponBaseServer, ReplicatedRotation, COND_None);

}

void AWeaponBaseServer::OnAttackerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFPSGameCharacterBase * FPSCharacter = Cast<AFPSGameCharacterBase>(OtherActor))
	{
		//if (!GetOwner())
		{
			EquipWeapon();

			if (WeaponType == EWeaponType::DESERTEAGLE)
			{
				FPSCharacter->EquipSecondaryWeapon(this);
			}
			else
			{
				FPSCharacter->EquipPrimaryWeapon(this);
			}
		}
	}
}

void AWeaponBaseServer::EquipWeapon()
{
	if (WeaponMesh)
	{
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
			
	if (SphereCollision)
	{
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//void AWeaponBaseServer::DetachWeapon()
//{
//	if (WeaponMesh)
//	{
//		WeaponMesh->SetEnableGravity(true);
//		WeaponMesh->SetSimulatePhysics(true);
//		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
//	
//		GThread::Get()->GetCoroutines().BindLambda(1.5f, [&]()
//			{
//				if (SphereCollision)
//				{
//					SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//				}
//			});
//	}
//}

void AWeaponBaseServer::ThrowWeapon(FVector InForwardDirection)
{
	float LaunchStrength = 200.0f;
	FVector LaunchVelocity = InForwardDirection * LaunchStrength;

	//DetachWeapon();
	if (WeaponMesh)
	{
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->AddImpulse(LaunchVelocity, NAME_None, true);

		GThread::Get()->GetCoroutines().BindLambda(1.5f, [&]()
		{
			if (SphereCollision)
			{
				SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
		});
	}
}

void AWeaponBaseServer::ReloadAmmo()
{
	if (CurrentAmmo >= MaxClipAmmo - CurrentClipAmmo)
	{
		CurrentAmmo -= MaxClipAmmo - CurrentClipAmmo;
		CurrentClipAmmo = MaxClipAmmo;
	}
	else
	{
		CurrentClipAmmo += CurrentAmmo;
		CurrentAmmo = 0;
	}
}

void AWeaponBaseServer::MulticastFireEffect_Implementation()
{
	if (GetOwner() != UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		UGameplayStatics::SpawnEmitterAttached(
			ServerWeaponMuzzleFlash,
			WeaponMesh,
			TEXT("Fire_FX_Soccket"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true,
			EPSCPoolMethod::None,
			true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ServerWeaponFireSound, GetActorLocation());
	}
}

void AWeaponBaseServer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBaseServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_Authority)
	{
		ReplicatedLocation = GetActorLocation();
		ReplicatedRotation = GetActorRotation();
	}
}

void AWeaponBaseServer::OnRep_ReplicatedLocation()
{
	SetActorLocation(ReplicatedLocation);
}

void AWeaponBaseServer::OnRep_ReplicatedRotation()
{
	SetActorRotation(ReplicatedRotation);
}


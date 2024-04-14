// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBaseClient.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AWeaponBaseClient::AWeaponBaseClient()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetOnlyOwnerSee(true);

	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponBaseClient::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBaseClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBaseClient::DisplayWeaponEffect()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClientWeaponFireSound);
	UGameplayStatics::SpawnEmitterAttached(
		ClientWeaponMuzzleFlash, 
		WeaponMesh, 
		TEXT("Fire_FX_Soccket"), 
		FVector::ZeroVector, 
		FRotator::ZeroRotator, 
		EAttachLocation::KeepRelativeOffset, 
		true, 
		EPSCPoolMethod::None, 
		true);
	
}


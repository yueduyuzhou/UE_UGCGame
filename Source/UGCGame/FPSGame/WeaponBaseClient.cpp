// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBaseClient.h"
#include "Components/SkeletalMeshComponent.h"

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


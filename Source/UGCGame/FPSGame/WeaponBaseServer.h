// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBaseServer.generated.h"

class USkeletalMeshComponent;
class USphereComponent;

UENUM()
enum class EWeaponType : uint8
{
	AK47			UMETA(DisplayName = "Ak47"),
	DESERTEAGLE		UMETA(DisplayName = "DesertEagle"),
};

UCLASS()
class UGCGAME_API AWeaponBaseServer : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
		EWeaponType WeapType;

public:	
	AWeaponBaseServer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};

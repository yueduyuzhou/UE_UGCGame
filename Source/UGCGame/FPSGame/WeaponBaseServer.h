// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonData/FPSGameType.h"
#include "WeaponBaseServer.generated.h"

class USkeletalMeshComponent;
class USphereComponent;
class AWeaponBaseClient;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AWeaponBaseClient> ClientWeaponClass;

public:	
	AWeaponBaseServer();

	UFUNCTION()
		void OnAttackerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*装备武器时处理*/
	UFUNCTION()
		void EquipWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};

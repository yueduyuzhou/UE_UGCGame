// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/FPSGameType.h"
#include "WeaponBaseClient.generated.h"

class USkeletalMeshComponent;

UCLASS()
class UGCGAME_API AWeaponBaseClient : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
	UAnimMontage* ClientArmFireMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* ClientArmReloadMontage;

	UPROPERTY(EditAnywhere)
	USoundBase* ClientWeaponFireSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ClientWeaponMuzzleFlash;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> CameraShakeClass;
	
	UPROPERTY(EditAnywhere)
	int32 ArmAnimBlendIndex;

	UPROPERTY(EditAnywhere)
	float FieldOfView;

	UPROPERTY(VisibleAnywhere, Category = "SinperUI")
	UUserWidget* SinperScope;

	UPROPERTY(EditAnywhere, Category = "SinperUI")
	TSubclassOf<UUserWidget> SinperScopeClass;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

public:	
	AWeaponBaseClient();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "FPSAnim")
		void PlayShootAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "FPSAnim")
		void PlayReloadAnimation();

	void DisplayWeaponEffect();
};

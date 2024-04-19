// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonData/FPSGameType.h"
#include "WeaponBaseServer.generated.h"

class USkeletalMeshComponent;
class USphereComponent;
class AWeaponBaseClient;
class UParticleSystem;

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

	UPROPERTY(EditAnywhere)
		UParticleSystem* ServerWeaponMuzzleFlash;

	UPROPERTY(EditAnywhere)
		USoundBase* ServerWeaponFireSound;

	UPROPERTY(EditAnywhere)
		UAnimMontage* ClientBodyFireMontage;

	UPROPERTY(EditAnywhere)
		float BulletDistance;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* BulletHoleDecal;

	UPROPERTY(EditAnywhere)
		float BaseDamage;

public:	
	AWeaponBaseServer();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION()
		void OnAttackerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*装备武器时处理*/
	UFUNCTION()
		void EquipWeapon();

public:
	UFUNCTION(NetMulticast, unreliable)
		void MulticastFireEffect();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE const int32& GetCurrentClipAmmo() { return CurrentClipAmmo; }
	FORCEINLINE void ExpendAmmo() { CurrentClipAmmo--; }
	FORCEINLINE const int32& GetCurrentAmmo() { return CurrentAmmo; }

private:
	UPROPERTY(EditAnywhere)
		int32 CurrentAmmo;	//当前的弹药量

	UPROPERTY(EditAnywhere, Replicated)
		int32 CurrentClipAmmo;	//当前弹夹的弹药量

	UPROPERTY(EditAnywhere)
		int32 MaxClipAmmo;	//弹夹最大弹药量
};

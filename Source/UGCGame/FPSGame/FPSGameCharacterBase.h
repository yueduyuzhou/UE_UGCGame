// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSGameCharacterBase.generated.h"

class AWeaponBaseServer;
class AWeaponBaseClient;
class AFPSGamePlayerController;
enum class EWeaponType : uint8;

UCLASS()
class UGCGAME_API AFPSGameCharacterBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ArmMesh;

	

public:
	AFPSGameCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	/*玩家初始武器*/
	void StartWeapon();

	/*根据类型生成武器并装备*/
	void PurchaseWeapon(EWeaponType InWeaponType);

	AWeaponBaseClient* GetCurrentClientWeapon();

	//RPG
private:
	UFUNCTION(server, reliable)
		void ChangeWalkWpeedOnServer(float InValue);

	UFUNCTION(server, reliable)
		void RifleWpeedFireOnServer(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing);

	UFUNCTION(client, reliable)
		void ServerCallClientEquipPrimaryWeapon();

	UFUNCTION(client, reliable)
		void ServerCallClientFireWeapon();

	UFUNCTION(client, reliable)
		void ServerCallClientUpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo);

	UFUNCTION(NetMulticast, unreliable)
		void MulticastFire();

	UFUNCTION(NetMulticast, unreliable)
		void MulticastBulletHole(const FVector& InLocation, const FRotator& InRotation);

public:
	void WeaponFirePressed();
	void WeaponFireReleassed();

	void LowSpeedWalk();
	void NormalSpeedWalk();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

public:
	/*准备主武器*/
	void EquipPrimaryWeapon(AWeaponBaseServer* InWeaponBaseServer);

	/*主武器（步枪）开/停火*/
	void PrimaryWeaponFire();
	void PrimaryWeaponStopFire();
	void RifleLineTrace(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing);

public:
	void DamagePlayer(UPhysicalMaterial* InPhysicsMaterial, AActor* InDamageActor, FVector InDamageFromDrection, FHitResult& InHitResult);
	
	UFUNCTION()
	void OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

public:
	AFPSGamePlayerController* GetFPSPlayerControllerOnServer();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		EWeaponType ActiveWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		AWeaponBaseServer * WeaponPrimaryServer;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		AWeaponBaseClient * WeaponPrimaryClient;

	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UAnimInstance* ClientArmAnimBP;

	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UAnimInstance* ClientBodyAnimBP;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		AFPSGamePlayerController* FPSPlayerController;
};

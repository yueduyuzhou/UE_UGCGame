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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	AWeaponBaseServer* GetCurrentServerWeapon();

	//RPG
private:
	UFUNCTION(server, reliable)
		void ChangeWalkWeaponOnServer(float InValue);

	UFUNCTION(server, reliable)
		void RifleWeaponFireOnServer(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing);
	
	UFUNCTION(server, reliable)
		void SniperWeaponFireOnServer(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing);

	UFUNCTION(server, reliable)
		void PsitolWeaponFireOnServer(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing);

	UFUNCTION(server, reliable)
		void PrimaryWeaponReloadOnServer();

	UFUNCTION(server, reliable)
		void SecondaryWeaponReloadOnServer();

	UFUNCTION(server, reliable)
		void StopFireingOnServer();

	UFUNCTION(server, reliable)
		void SetIsAimingOnServer(bool InIsAiming);

	UFUNCTION(client, reliable)
		void ServerCallClientEquipPrimaryWeapon();

	UFUNCTION(client, reliable)
		void ServerCallClientEquipSecondaryWeapon();

	UFUNCTION(client, reliable)
		void ServerCallClientFireWeapon();

	UFUNCTION(client, reliable)
		void ServerCallClientWeaponRecoil();

	UFUNCTION(client, reliable)
		void ServerCallClientUpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo);

	UFUNCTION(client, reliable)
		void ServerCallClientUpdateHealth(const float& InHealth, const float& InMaxHealth);

	UFUNCTION(client, reliable)
		void ServerCallClientReloadAnimation();

	UFUNCTION(client, reliable)
		void ClientAiming();

	UFUNCTION(client, reliable)
		void ClientEndAiming();

	UFUNCTION(NetMulticast, unreliable)
		void MulticastFire();

	UFUNCTION(NetMulticast, unreliable)
		void MulticastReload();

	UFUNCTION(NetMulticast, unreliable)
		void MulticastBulletHole(const FVector& InLocation, const FRotator& InRotation);

public:
	void WeaponFirePressed();
	void WeaponFireReleassed();

	void WeaponAimingPressed();
	void WeaponEndAimingReleassed();

	void LowSpeedWalk();
	void NormalSpeedWalk();

	void AmmoReload();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

public:
	/*装备主武器*/
	void EquipPrimaryWeapon(AWeaponBaseServer* InWeaponBaseServer);

	/*装备副武器*/
	void EquipSecondaryWeapon(AWeaponBaseServer* InWeaponBaseServer);

	/*主武器（步枪）开/停火*/
	void PrimaryWeaponFire();
	void PrimaryWeaponStopFire();
	void RifleLineTrace(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing);

	/*主武器（狙击枪）开/停火*/
	void SniperWeaponFire();
	void SniperWeaponStopFire();
	void SniperLineTrace(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing);

	/*副武器（手枪）开/停火*/
	void SecondaryWeaponFire();
	void SecondaryWeaponStopFire();
	void PistolLineTrace(FVector InCamreaLocation, FRotator InCameraRotation, bool IsMoveing);

	/*武器连击*/
	void AutomaticFire();

	/*重置后坐力*/
	void ResetRecoil();

	/*延时装载*/
	void ReloadDelayCallBack();

	/*狙击枪开火间隔*/
	void SniperFireDelayCallBack();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateFPSArmAnimBlend(const int32& InBlendIndex);

	void UpdateAmmoForSuccessServer(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo);
	void UpdateAmmoForSuccessClient(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo);

public:
	void DamagePlayer(UPhysicalMaterial* InPhysicsMaterial, AActor* InDamageActor, FVector InDamageFromDrection, FHitResult& InHitResult);
	
	UFUNCTION()
	void OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

public:
	AFPSGamePlayerController* GetFPSPlayerControllerOnServer();

private:
	UPROPERTY(Replicated, meta = (AllowPrivateAccess = "true"))
		EWeaponType ActiveWeapon;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		EWeaponType StartWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	/*************
	*	Weapon
	**************/
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		AWeaponBaseServer * WeaponPrimaryServer;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		AWeaponBaseClient * WeaponPrimaryClient;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		AWeaponBaseServer * WeaponSecondaryServer;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		AWeaponBaseClient * WeaponSecondaryClient;

	/*************
	*	Anim
	**************/
	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UAnimInstance* ClientArmAnimBP;

	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UAnimInstance* ClientBodyAnimBP;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		AFPSGamePlayerController* FPSPlayerController;
	
	UPROPERTY(Replicated)
		bool IsFireing;

	UPROPERTY(Replicated)
		bool IsReloading;

	UPROPERTY(Replicated)
		bool IsAiming;

private:
	FTimerHandle AutomaticFireTimerHandle;

	/*后坐力*/
	float NewVerticalRecoilValue;
	float OldVerticalRecoilValue;
	float VerticalRecoilDiffValue;
	float CurveXRecoilValue;

	float NewHorizontalRecoilValue;
	float OldHorizontalRecoilValue;
	float HorizontalRecoilDiffValue;

	float PsitolRecoilMin;
	float PsitolRecoilMax;
};

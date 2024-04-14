// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSGameCharacterBase.generated.h"

class AWeaponBaseServer;
class AWeaponBaseClient;
enum class EWeaponType : uint8;

UCLASS()
class UGCGAME_API AFPSGameCharacterBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ArmMesh;

	UAnimInstance* ClientArmAnimBP;

public:
	AFPSGameCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	/*��ҳ�ʼ����*/
	void StartWeapon();

	/*������������������װ��*/
	void PurchaseWeapon(EWeaponType InWeaponType);

	AWeaponBaseClient* GetCurrentClientWeapon();

	//RPG
private:
	UFUNCTION(server, reliable)
		void ChangeWalkWpeedOnServer(float InValue);

	UFUNCTION(client, reliable)
		void ServerCallClientEquipPrimaryWeapon();

	UFUNCTION(client, reliable)
		void ServerCallClientFireWeapon();

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
	/*׼��������*/
	void EquipPrimaryWeapon(AWeaponBaseServer* InWeaponBaseServer);

	/*����������ǹ����/ͣ��*/
	void PrimaryWeaponFire();
	void PrimaryWeaponStopFire();

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
};

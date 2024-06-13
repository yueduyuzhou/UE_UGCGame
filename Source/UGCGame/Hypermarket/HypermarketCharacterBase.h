// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HypermarketCharacterBase.generated.h"

class AWeaponBaseServer;
struct FHypermarketTable;

UCLASS()
class UGCGAME_API AHypermarketCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHypermarketCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/*����*/
	void EquipItem(const int32& InTableID);

private:
	/*������������������װ��*/
	void PurchaseWeapon(const FHypermarketTable* InHTable);

	/*װ��������*/
	void EquipPrimaryWeapon(AWeaponBaseServer* InWeaponBaseServer);

	/*װ��������*/
	void EquipSecondaryWeapon(AWeaponBaseServer* InWeaponBaseServer);

private:
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		EWeaponType ActiveWeapon;

	/*************
	*	Weapon
	**************/
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		AWeaponBaseServer * WeaponPrimaryServer;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		AWeaponBaseServer * WeaponSecondaryServer;
};

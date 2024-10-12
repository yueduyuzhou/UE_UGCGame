// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UGCGameTableBase.h"
#include "../Common/UGCGameType.h"
#include "../FPSGame/WeaponBaseServer.h"
#include "HypermarketTable.generated.h"


class UTexture2D;

USTRUCT(BlueprintType)
struct FHypermarketTable : public FUGCGameTableBase
{
	GENERATED_USTRUCT_BODY()

public:
	FHypermarketTable();

	bool ContainsType(const EHypermarkType& InType);

public:
	/*����*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FName ItemName;

	/*ͼ��*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		UTexture2D* ItemIcon;

	/*�۸�*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		int32 ItemGold;

	/*��Ӧ������*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		TSubclassOf<AWeaponBaseServer> ItemClass;

	/*��������*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		EWeaponType WeaponType;

	/*Item������*/
	UPROPERTY(EditDefaultsOnly, Category = "SlotTypes")
		TArray<EHypermarkType> Types;

	/*Item����*/
	UPROPERTY(EditDefaultsOnly, Category = "MainClass")
		EItemMainClass MainClass;
};
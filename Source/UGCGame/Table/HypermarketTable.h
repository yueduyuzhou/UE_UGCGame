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
	/*名字*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FName ItemName;

	/*图标*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		UTexture2D* ItemIcon;

	/*价格*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		int32 ItemGold;

	/*对应武器类*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		TSubclassOf<AWeaponBaseServer> ItemClass;

	/*武器类型*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		EWeaponType WeaponType;

	/*Item所属栏*/
	UPROPERTY(EditDefaultsOnly, Category = "SlotTypes")
		TArray<EHypermarkType> Types;

	/*Item大类*/
	UPROPERTY(EditDefaultsOnly, Category = "MainClass")
		EItemMainClass MainClass;
};
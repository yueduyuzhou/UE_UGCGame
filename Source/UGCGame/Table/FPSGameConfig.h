// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UGCGameTableBase.h"
#include "FPSGameConfig.generated.h"

USTRUCT(BlueprintType)
struct FFPSGameConfig : public FUGCGameTableBase
{
	GENERATED_USTRUCT_BODY()

public:
	FFPSGameConfig();

public:
	/* 模式 */

	/* 持续时间，单位秒 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameConfig")
		float Duration;

	/* 必要的Element [ElementID, 数量] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameConfig")
		TMap<int32, int32> EssentialEle;

	/* 默认初始装备，对应位置没有装备的情况下使用 [PrimaryWeaponID SecondaryWeaponID CloseRangeWeaponID GrenadeWeaponID] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameConfig")
		TArray<int32> DefaultWeapon;
};
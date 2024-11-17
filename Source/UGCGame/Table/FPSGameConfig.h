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
	/* ģʽ */

	/* ����ʱ�䣬��λ�� */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameConfig")
		float Duration;

	/* ��Ҫ��Element [ElementID, ����] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameConfig")
		TMap<int32, int32> EssentialEle;

	/* Ĭ�ϳ�ʼװ������Ӧλ��û��װ���������ʹ�� [PrimaryWeaponID SecondaryWeaponID CloseRangeWeaponID GrenadeWeaponID] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameConfig")
		TArray<int32> DefaultWeapon;
};
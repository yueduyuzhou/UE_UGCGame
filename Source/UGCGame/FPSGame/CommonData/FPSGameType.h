// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FPSGameType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AK47			UMETA(DisplayName = "Ak47"),
	M4A1			UMETA(DisplayName = "M4A1"),
	MP7				UMETA(DisplayName = "MP7"),
	SNIPER			UMETA(DisplayName = "Sniper"),
	DESERTEAGLE		UMETA(DisplayName = "DesertEagle")
	
};

USTRUCT(BlueprintType)
struct FFPSPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	FFPSPlayerInfo()
		:PlayerID(INDEX_NONE)
		, KillCount(0)
		, DeathCount(0)
	{}

	UPROPERTY()
	int32 PlayerID;
	
	UPROPERTY()
	int32 KillCount;
	
	UPROPERTY()
	int32 DeathCount;
};
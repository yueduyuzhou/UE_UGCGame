// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AK47			UMETA(DisplayName = "Ak47"),
	DESERTEAGLE		UMETA(DisplayName = "DesertEagle"),
	M4A1			UMETA(DisplayName = "M4A1"),
	MP7				UMETA(DisplayName = "MP7"),
	SNIPER			UMETA(DisplayName = "Sniper")
};
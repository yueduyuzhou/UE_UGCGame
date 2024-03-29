// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EElementModifyType : uint8
{
	MODIFY_LOCATION UMETA(DisplayName = "ModifyLocation"),
	MODIFY_ROTATION UMETA(DisplayName = "ModifyRotation")
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	ALL					UMETA(DisplayName = "All"),
	STATIC_PROP			UMETA(DisplayName = "Static Prop"),
	DYNAMIC_PROP		UMETA(DisplayName = "Dynamic Prop"),
	EFFECT_PROP			UMETA(DisplayName = "Effect Prop")
};


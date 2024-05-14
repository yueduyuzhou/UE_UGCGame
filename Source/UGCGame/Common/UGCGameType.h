// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UGCGameType.generated.h"

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

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	TEAM_RED,
	TEAM_BLUE,
};

USTRUCT(BlueprintType)
struct FPlayerNetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 PlayerID;

	UPROPERTY()
	ETeamType Team;
};

FORCEINLINE bool operator==(const FPlayerNetData& InA, const FPlayerNetData& InB)
{
	return InA.PlayerID == InB.PlayerID;
}

UENUM(BlueprintType)
enum class EEditDetailType : uint8
{
	DETAIL_LOCATION		UMETA(DisplayName = "DetailLocation"),
	DETAIL_ROTATION		UMETA(DisplayName = "DetailRotation"),
	DETAIL_SCALE		UMETA(DisplayName = "DetailScale"),

};

UENUM(BlueprintType)
enum class EVectorDime : uint8
{
	X,
	Y,
	Z
};

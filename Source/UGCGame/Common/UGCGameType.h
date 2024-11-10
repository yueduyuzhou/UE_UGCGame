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
enum class EHypermarkType : uint8
{
	ALL					UMETA(DisplayName = "All"),
	PRIMARYWEAPON		UMETA(DisplayName = "PrimaryWeapon"),
	SECONDARYWEAPON		UMETA(DisplayName = "SecondaryWeapon"),
	CLOSERANGEWEAPON	UMETA(DisplayName = "CloseRangeWeapon"),
	DEFAULT				UMETA(DiaplayName = "Default")
};

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	TEAM_RED = 0,
	TEAM_BLUE,
	TEAM_NONE
};

USTRUCT(BlueprintType)
struct FPlayerNetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 PlayerID;

	UPROPERTY()
	ETeamType Team;

	UPROPERTY()
	TArray<int32> EquipedWeapons;
};

FORCEINLINE bool operator==(const FPlayerNetData& InA, const FPlayerNetData& InB)
{
	return InA.PlayerID == InB.PlayerID;
}

UENUM(BlueprintType)
enum class EEditDetailType : uint8
{
	DETAIL_LOCATION			UMETA(DisplayName = "DetailLocation"),
	DETAIL_ROTATION			UMETA(DisplayName = "DetailRotation"),
	DETAIL_SCALE			UMETA(DisplayName = "DetailScale"),
	
	DETAIL_ENUM_TEAMTYPE	UMETA(DisplayName = "DetailTeamType"),

	DETAIL_COLOR			UMETA(DisplayName = "DetailColor"),
};

UENUM(BlueprintType)
enum class EVectorDime : uint8
{
	X,
	Y,
	Z
};

UENUM(BlueprintType)
enum class EHSVType : uint8
{
	HUE						UMETA(DisplayName = "Hue"),
	SATURATION				UMETA(DisplayName = "Saturation"),
	VALUE					UMETA(DisplayName = "Value"),
};

UENUM(BlueprintType)
enum class ELobbyPanelType : uint8
{
	LOBBY_UIPANEL_PERMANENT					UMETA(DisplayName = "PermanentPanel"),
	LOBBY_UIPANEL_LOBBY						UMETA(DisplayName = "LobbyPanel"),
	LOBBY_UIPANEL_HYPERMARKET				UMETA(DisplayName = "HypermarketPanel"),
	LOBBY_UIPANEL_CREATIVEWORKSHOP			UMETA(DisplayName = "CreativeWorkshop"),
	LOBBY_UIPANEL_QUITPANEL					UMETA(DisplayName = "QuitPanel"),
};

UENUM(BlueprintType)
enum class EItemMainClass : uint8
{
	DEFAULT = 0						UMETA(DisplayName = "Default"),
	CURRENCY = 1					UMETA(DisplayName = "Currency"),
	WEAPON = 2						UMETA(DisplayName = "Weapon"),
	OTHER = 3						UMETA(DisplayName = "Other"),
};

/* 通用物品id，数量 键值对*/
USTRUCT(BlueprintType)
struct FIDCountPair
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 Count;
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

	FItemInfo()
		:ItemID(INDEX_NONE)
	{}

	FItemInfo(int32 InID)
		:ItemID(InID)
	{}

	UPROPERTY()
		int32 ItemID;
};

FORCEINLINE bool operator==(const FItemInfo& InA, const FItemInfo& InB)
{
	return InA.ItemID == InB.ItemID;
}

UCLASS()
class UGCGAME_API UPlayerSaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	bool ItemContains(const int32& InID);

public:
	//玩家拥有的金币
	UPROPERTY()
		int32 Gold;

	//玩家拥有的道具、武器
	UPROPERTY()
		TArray<FItemInfo> Items;
};

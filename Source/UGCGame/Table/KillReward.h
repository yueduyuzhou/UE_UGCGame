// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UGCGameTableBase.h"
#include "../Common/UGCGameType.h"
#include "KillReward.generated.h"

USTRUCT(BlueprintType)
struct FKillReward : public FUGCGameTableBase
{
	GENERATED_USTRUCT_BODY()

public:
	FKillReward();

public:
	/* »÷É±Êý */
	/*UPROPERTY(EditDefaultsOnly, Category = "KillReward")
		int32 ID;*/

	/* ½±Àø [[item1,count],[item2,count]...] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KillReward")
		TArray<FIDCountPair> Items;
};
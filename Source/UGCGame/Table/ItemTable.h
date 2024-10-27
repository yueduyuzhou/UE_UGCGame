// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UGCGameTableBase.h"
#include "../Common/UGCGameType.h"
#include "ItemTable.generated.h"


class UTexture2D;

USTRUCT(BlueprintType)
struct FItemTable : public FUGCGameTableBase
{
	GENERATED_USTRUCT_BODY()

public:
	FItemTable();

public:
	/*名字*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FName ItemName;

	/*图标*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		UTexture2D* ItemIcon;

	/*Item大类*/
	UPROPERTY(EditDefaultsOnly, Category = "MainClass")
		EItemMainClass MainClass;
};
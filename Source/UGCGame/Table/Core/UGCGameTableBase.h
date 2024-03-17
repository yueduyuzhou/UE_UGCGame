// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UGCGameTableBase.generated.h"

USTRUCT(BlueprintType)
struct FUGCGameTableBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FUGCGameTableBase();

	UPROPERTY(EditDefaultsOnly, Category = "UGC Game Table")
	int32 ID;
};
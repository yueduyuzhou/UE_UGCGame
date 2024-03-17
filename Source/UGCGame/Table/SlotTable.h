// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UGCGameTableBase.h"
#include "SlotTable.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FSlotTable : public FUGCGameTableBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSlotTable();

	/*Ãû×Ö*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FName SlotName;

	/*Í¼±ê*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		UTexture2D* SlotIcon;
	
};
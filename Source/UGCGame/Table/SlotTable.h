// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UGCGameTableBase.h"
#include "../Common/UGCGameType.h"
#include "SlotTable.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FSlotTable : public FUGCGameTableBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSlotTable();

	bool ContainsType(const ESlotType& InType);

public:
	/*����*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FName SlotName;

	/*ͼ��*/
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		UTexture2D* SlotIcon;

	/*���*/	UPROPERTY(EditDefaultsOnly, Category = "SlotTypes")		TArray<ESlotType> Types;
};
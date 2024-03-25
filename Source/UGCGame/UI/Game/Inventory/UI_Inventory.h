// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_Inventory.generated.h"

class UUniformGridPanel;
class UUI_InventorySlot;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Inventory : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UUniformGridPanel* SlotArray;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_InventorySlot> InventorySlotClass;

protected:
	virtual void NativeConstruct();

protected:
	//≥ı ºªØInventorySlot
	void LayoutSlot(const TArray<int32>& InKeys);

	void BindDelegate();
};

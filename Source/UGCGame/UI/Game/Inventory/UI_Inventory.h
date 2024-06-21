// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_Inventory.generated.h"

class UUniformGridPanel;
class UUI_InventorySlot;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Inventory : public UUI_Base
{
	GENERATED_BODY()

	friend class UUI_TypeSlot;

	UPROPERTY(meta = (BindWidget))
		UUniformGridPanel* SlotArray;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* TypesPanel;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_InventorySlot> InventorySlotClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_TypeSlot> TypesPanelSlotClass;

protected:
	virtual void NativeConstruct();

protected:
	//≥ı ºªØInventorySlot
	void LayoutSlot(const TArray<int32>& InKeys);
	void LayoutTypesPanel();
};

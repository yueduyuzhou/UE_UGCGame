// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_ItemPanel.generated.h"

class UUniformGridPanel;
class UUI_Item;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_ItemPanel : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UUniformGridPanel* SlotArray;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_Item> InventorySlotClass;

protected:
	virtual void NativeConstruct();
};

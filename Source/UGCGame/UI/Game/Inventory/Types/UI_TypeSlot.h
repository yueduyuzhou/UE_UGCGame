// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Slot.h"
#include "UI_TypeSlot.generated.h"

class UTextBlock;
class UUI_Inventory;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_TypeSlot : public UUI_Slot
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UTextBlock* TypeText;

	ESlotType SlotType;

public:
	void SetTypeText(const FText& InText);
	void SetType(const ESlotType& InType);
	void SetParentInventory(UUI_Inventory* InInventory);

protected:
	virtual void NativeConstruct() override;

public:
	virtual void OnClickedWidget() override;

private:
	UUI_Inventory* ParentInventory;
};

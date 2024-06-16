// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Common/UGCGameType.h"
#include "UI_Backpack.generated.h"

class UButton;
class UGridPanel;
class UComboBoxString;
class UUI_HypermarketSlot;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Backpack : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UComboBoxString* Selecter;

	UPROPERTY(meta = (BindWidget))
		UButton* XButton;

	UPROPERTY(meta = (BindWidget))
		UGridPanel* ItemGrid;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_HypermarketSlot> ItemClass;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		virtual void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

protected:
	void UpdateItem(EHypermarkType InType);

protected:
	UFUNCTION()
		void OnXButtonClicked();
};

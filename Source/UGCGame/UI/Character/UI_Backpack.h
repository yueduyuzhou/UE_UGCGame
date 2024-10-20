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
class UUI_EquippedSlot;
class UPlayerModule;
struct FHypermarketTable;

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

	UPROPERTY(meta = (BindWidget))
		UUI_EquippedSlot* EquippedPrimary;

	UPROPERTY(meta = (BindWidget))
		UUI_EquippedSlot* EquippedSecondary;

	UPROPERTY(meta = (BindWidget))
		UUI_EquippedSlot* EquippedCloseRange;

	UPROPERTY(meta = (BindWidget))
		UUI_EquippedSlot* EquippedGrenade;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_EquippedSlot> ItemClass;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

protected:
	void UpdateItem(EHypermarkType InType = EHypermarkType::DEFAULT);
	void UpdateItem_Inner();

	UFUNCTION()
	void OnXButtonClicked();

public:
	/*更新Equipped信息*/
	void UpdateEquippedSlot(const int32& InID);
	void InitEquippedSlots();

	void SaveEquippedItems();

private:
	UPlayerModule* PMod = nullptr;

	FDelegateHandle UpdateItemsHandle;
	EHypermarkType CurMarkType;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Common/UGCGameType.h"
#include "UI_Hypermarket.generated.h"

class UButton;
class UCheckBox;
class UUniformGridPanel;
class UUI_HypermarketSlot;
class UPlayerModule;
struct FHypermarketTable;

/**
 * 
 */

UCLASS()
class UGCGAME_API UUI_Hypermarket : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UCheckBox* All;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* PrimaryWeapon;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* SecondaryWeapon;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* CloseRangeWeapon;

	UPROPERTY(meta = (BindWidget))
		UUniformGridPanel* ItemGrid;

	UPROPERTY(meta = (BindWidget))
		UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
		UButton* BuyButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
		UButton* GoBackButton;

	UPROPERTY(meta = (BindWidget))
		UButton* BackpackButton;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* GoldText;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_HypermarketSlot> ItemClass;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void UpdateItem(EHypermarkType InType);
	void UpdateItemImage(const int32& InID);
	void UpdateItemImage();

protected:
	UFUNCTION()
		void CheckBoxAll(bool bIsChecked);

	UFUNCTION()
		void CheckBoxPrimaryWeapon(bool bIsChecked);

	UFUNCTION()
		void CheckBoxSecondaryWeapon(bool bIsChecked);

	UFUNCTION()
		void CheckBoxCloseRangeWeapon(bool bIsChecked);

	UFUNCTION()
		void OnBuyButtonClicked();

	UFUNCTION()
		void OnCancelButtonClicked();

	UFUNCTION()
		void OnGoBackButtonClicked();

	UFUNCTION()
		void OnBackpackButtonClicked();

protected:
	void SetCheckBoxArray(ECheckBoxState CheckBoxState);
	void UpdateGold();

private:
	/*所有可选项*/
	TArray<UCheckBox*> CheckBoxArray;

	int32 CurTableID = INDEX_NONE;

	FDelegateHandle UpdateGoldHandle;
	FDelegateHandle UpdateItemsHandle;

	UPlayerModule* PMod;

	int32 PreBuyItemID = INDEX_NONE;
};

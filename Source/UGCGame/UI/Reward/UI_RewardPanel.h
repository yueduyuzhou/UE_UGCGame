// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "UI_RewardPanel.generated.h"

class UButton;
class UHorizontalBox;
class UUI_HypermarketSlot;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_RewardPanel : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UButton* OKBtn;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* ItemList;
	
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_HypermarketSlot> ItemClass;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetItemsData(const TMap<int32, int32>& InData);

private:
	UFUNCTION()
		void OnOKBtnClick();

	void RenderItemList();

private:
	TMap<int32, int32> Data;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_FPSBase.h"
#include "UI_FinalPanel.generated.h"

class UVerticalBox;
class UTextBlock;
class UUI_FinalPanelSlot;
class UUI_RewardPanel;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_FinalPanel : public UUI_FPSBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UVerticalBox* PlayerList;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Result;

	UPROPERTY(meta = (BindWidget))
		UUI_RewardPanel* RewardPanel;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_FinalPanelSlot> FinalPanelSlotClass;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void InitPlayerList();
	void InitRewardPanel();

	void SettlementReward();
};

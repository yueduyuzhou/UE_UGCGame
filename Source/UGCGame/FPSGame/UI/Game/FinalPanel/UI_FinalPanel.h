// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_FPSBase.h"
#include "UI_FinalPanel.generated.h"

class UVerticalBox;
class UTextBlock;
class UUI_FinalPanelSlot;

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

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_FinalPanelSlot> FinalPanelSlotClass;

protected:
	virtual void NativeConstruct() override;

};

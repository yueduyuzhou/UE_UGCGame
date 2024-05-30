// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_FPSBase.h"
#include "UI_FinalPanelSlot.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_FinalPanelSlot : public UUI_FPSBase
{
	friend class UUI_FinalPanel;

	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Rank;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Kill;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Death;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Score;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayerID;
};

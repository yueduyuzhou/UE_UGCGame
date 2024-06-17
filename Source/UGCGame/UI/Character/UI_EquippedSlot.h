// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_HypermarketSlot.h"
#include "UI_EquippedSlot.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_EquippedSlot : public UUI_HypermarketSlot
{
	friend class UUI_Hypermarket;
	friend class UUI_Backpack;

	GENERATED_BODY()

	UUI_EquippedSlot();

protected:
	virtual void OnClickedWidget();

protected:
	FHypermarketDelegate EquippedtDelegate;
};

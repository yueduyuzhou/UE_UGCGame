// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Slot.h"
#include "UI_Settings.generated.h"

class UCheckBox;
class UButton;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Settings : public UUI_Slot
{
	GENERATED_BODY()

public:
	virtual void OnClickedWidget();

protected:
	virtual void NativeConstruct();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void DestorySessionBP();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_Settings.generated.h"

class UCheckBox;
class UButton;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Settings : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;
};

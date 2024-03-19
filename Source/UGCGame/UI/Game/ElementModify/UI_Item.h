// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_Item.generated.h"

class UImage;
class UEditableText;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Item : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* Icon;

	UPROPERTY(meta = (BindWidget))
		UEditableText* InputText;
};

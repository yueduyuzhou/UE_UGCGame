// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "UI_MapList.generated.h"

class UVerticalBox;
class UButton;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_MapList : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* MapList;

	UPROPERTY(meta = (BindWidget))
		UButton* CreateMap;

protected:
	virtual void NativeConstruct() override;

	void OnCreateMapClick();
};

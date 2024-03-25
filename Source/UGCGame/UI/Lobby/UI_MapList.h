// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "UI_MapSlot.h"
#include "UI_MapList.generated.h"

class UVerticalBox;
class UButton;
class UEditableText;

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

	UPROPERTY(meta = (BindWidget))
		UEditableText* CreateMapName;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_MapSlot> MapSlotClass;

protected:
	virtual void NativeConstruct() override;

private:
	void UpdateMapList();

protected:
	UFUNCTION(BlueprintCallable)
	void OnCreateMapClick();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void CreateSessionBP(int32 InMax, bool InUseLAN);
};

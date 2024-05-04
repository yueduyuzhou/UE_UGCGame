// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "FindSessionsCallbackProxy.h"
#include "UI_SearchPanel.generated.h"

class UButton;
class UVerticalBox;
class UUI_MapSearchSlot;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_SearchPanel : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UButton* FindMap;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* MapList;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_MapSearchSlot> MapSearchSlotClass;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnFindMapClick();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		TArray<FBlueprintSessionResult> FindSessionBP(int32 InMax, bool InUseLAN);

};

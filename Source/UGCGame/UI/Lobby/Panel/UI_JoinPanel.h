// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_JoinPanel.generated.h"

class UUI_NetSetting;
class UUI_SearchPanel;
class UButton;
class UVerticalBox;
class UUI_MapSearchSlot;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_JoinPanel : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		UUI_NetSetting* Join_NetSetting_Domain;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		UButton* FindMap;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		UVerticalBox* MapList;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = UI)
		TSubclassOf<UUI_MapSearchSlot> MapSearchSlotClass;

};

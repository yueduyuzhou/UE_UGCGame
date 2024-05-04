// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_CreatePanel.generated.h"

class UUI_NetSetting;
class UButton;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_CreatePanel : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		UUI_NetSetting* Create_NetSetting_Number;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		UUI_NetSetting* Create_NetSetting_Domain;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		UButton* Create_Button;

protected:
	virtual void NativeConstruct();
};

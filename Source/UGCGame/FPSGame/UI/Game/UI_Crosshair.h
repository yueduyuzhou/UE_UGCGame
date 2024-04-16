// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_FPSBase.h"
#include "UI_Crosshair.generated.h"

class UImage;
class UOverlay;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Crosshair : public UUI_FPSBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UOverlay* CrosshairOverlay;

	UPROPERTY(meta = (BindWidget))
		UImage* Top;

	UPROPERTY(meta = (BindWidget))
		UImage* Button;

	UPROPERTY(meta = (BindWidget))
		UImage* Left;

	UPROPERTY(meta = (BindWidget))
		UImage* Right;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void DoCrosshairRecoil();

protected:
	virtual void NativeConstruct();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_FPSBase.h"
#include "UI_Crosshair.generated.h"

class UImage;
class UOverlay;
class UTextBlock;

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

	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentClipAmmo;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentAmmo;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void DoCrosshairRecoil();

	void UpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo);

protected:
	virtual void NativeConstruct();

};

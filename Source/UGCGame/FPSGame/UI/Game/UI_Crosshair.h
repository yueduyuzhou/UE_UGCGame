// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_FPSBase.h"
#include "UI_Crosshair.generated.h"

class UImage;
class UOverlay;
class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Crosshair : public UUI_FPSBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UOverlay* CrosshairOverlay;

	/*×¼ÐÇ*/
	UPROPERTY(meta = (BindWidget))
		UImage* Top;

	UPROPERTY(meta = (BindWidget))
		UImage* Button;

	UPROPERTY(meta = (BindWidget))
		UImage* Left;

	UPROPERTY(meta = (BindWidget))
		UImage* Right;

	/*×Óµ¯*/
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentClipAmmo;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentAmmo;

	/*ÑªÌõ*/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* HealthVal;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* HealthIcon;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void DoCrosshairRecoil();

	UFUNCTION(BlueprintImplementableEvent)
		void DoHealthAnim(const int32& InHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void StrongState();

	UFUNCTION(BlueprintImplementableEvent)
		void InjureState();

	UFUNCTION(BlueprintImplementableEvent)
		void AgonalState();

public:
	void UpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo);
	void UpdateHealth(const float& InHealth, const float& InMaxHealth);

private:
	void RegisterToPlayerController();

protected:
	virtual void NativeConstruct();

};

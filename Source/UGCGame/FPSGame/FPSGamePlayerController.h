// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/Game/UI_Crosshair.h"
#include "FPSGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	AFPSGamePlayerController();

	virtual void SetupInputComponent() override;

private:
	void OnLeftMouseButtonDown();
	void OnLeftMouseButtonUp();
	void Jump();
	void OnLeftShiftButtonDown();
	void OnLeftShiftButtonUp();
	void AmmoReload();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

public:
	void PlayerCameraShake(TSubclassOf<UCameraShake> InCameraShake);
	void CrosshairRecoil();

	void UpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo);
	void UpdateHealth(const float& InHealth, const float& InMaxHealth);

public:
	UUI_Crosshair* CrosshairUI;
};

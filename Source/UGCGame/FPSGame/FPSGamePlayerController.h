// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Jump();
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
};

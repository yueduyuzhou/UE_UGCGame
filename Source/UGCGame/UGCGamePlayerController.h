// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UGCGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API AUGCGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	AUGCGamePlayerController();

	virtual void SetupInputComponent() override;


private:
	void OnRightMouseButtonDown();
	void OnRightMouseButtonUp();

	void OnDeleteButtonUp();

public:
	void GetMouseLocationAndDrection(FVector& OutWorldPosition, FVector& OutWorldDirection);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HypermarketPlayerController.generated.h"

class AHypermarketCharacterBase;

/**
 * 
 */
UCLASS()
class UGCGAME_API AHypermarketPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AHypermarketPlayerController();

	virtual void SetupInputComponent() override;

	void OnMouseLeftButtonDown();
	void OnMouseLeftButtonUp();
	void OnMouseMoveX(float Value);

//protected:
//	virtual void BeginPlay() override;

private:
	bool bIsRotatingCharacter;
	FVector2D LastMousePosition;

	AHypermarketCharacterBase* ControlledActor;


};

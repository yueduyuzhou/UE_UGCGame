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
	void OnLeftMouseButtonDown();
	void OnLeftMouseButtonUp();
	void OnDeleteButtonUp();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void TransformationForTranslation();
	void TransformationForRotation();
	void TransformationForScale();

	void MultiSelect();
	void DeMultiSelect();

	void MouseWheelCameraView(float Value);

public:
	void GetMouseLocationAndDrection(FVector& OutWorldPosition, FVector& OutWorldDirection);

	FORCEINLINE bool IsEditing() { return bIsEditing; }

private:
	bool bIsEditing;
};

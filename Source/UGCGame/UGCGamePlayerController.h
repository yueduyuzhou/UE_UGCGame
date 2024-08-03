// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UGCGame/Common/RenderingUtils.h"
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

protected:
	virtual void BeginPlay() override;

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

	void EnableUIOnlyInput(UUserWidget* InWidgetToFocus);
	void EnableGameAndUIInput();

	FORCEINLINE bool IsEditing() { return bIsEditing; }

	FORCEINLINE bool IsEditingDetail() { return bIsEditingDetail; }
	void SetEditingDetail(bool inbIsEditingDetail);

	/***********************************************************************
	*	Capture
	***********************************************************************/
	void CaptureLobbyTexture(const FString& InMapName);

private:
	bool bIsEditing;
	bool bIsEditingDetail;

	float AngleRotationRate;

	FMinimapCapture LobbyTextureCapture;
};

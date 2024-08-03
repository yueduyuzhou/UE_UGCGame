// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TransformerPawn.h"
#include "UGCGamePawn.generated.h"

class UUI_DetailsPanel;
class AElementBase;

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FCommitChangeMulticastDelegate)

UCLASS()
class UGCGAME_API AUGCGamePawn : public ATransformerPawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

public:
	AUGCGamePawn();

	virtual void OnComponentSelectionChange_Implementation(class USceneComponent* Component
		, bool bSelected, bool bImplementsUFocusable) override;

protected:
	virtual void BeginPlay() override;

public:
	void OnLeftMousePressed();
	void OnLeftMouseReleassed();

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
	void SetDetailsPanel(UUI_DetailsPanel* InDetailsPanel);
	TArray<AElementBase*> GetSelectedElement();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float ZoomSpeed;

	FCommitChangeMulticastDelegate CommitChangeMulticastDelegate;

private:
	bool bMulitSelect;

	UUI_DetailsPanel* DetailsPanel;
};

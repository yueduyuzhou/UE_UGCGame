// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TransformerPawn.h"
#include "UGCGamePawn.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API AUGCGamePawn : public ATransformerPawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UFloatingPawnMovement* FloatingPawnMovement;



public:
	AUGCGamePawn();

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

	//void Set

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
};

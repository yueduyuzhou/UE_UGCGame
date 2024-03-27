// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGamePawn.h"
#include "GameFramework/FloatingPawnMovement.h"

AUGCGamePawn::AUGCGamePawn()
{
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void AUGCGamePawn::OnLeftMousePressed()
{
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;
	CollisionChannels.Add(TEnumAsByte<ECollisionChannel>::EnumType::ECC_WorldDynamic);
	CollisionChannels.Add(TEnumAsByte<ECollisionChannel>::EnumType::ECC_WorldStatic);
	ReplicatedMouseTraceByObjectTypes(5000000.f, CollisionChannels);
}

void AUGCGamePawn::OnLeftMouseReleassed()
{
	ReplicateFinishTransform();
}

void AUGCGamePawn::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUGCGamePawn::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUGCGamePawn::MoveUp(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get up vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUGCGamePawn::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUGCGamePawn::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUGCGamePawn::TransformationForTranslation()
{
	SetTransformationType(ETransformationType::TT_Translation);
}

void AUGCGamePawn::TransformationForRotation()
{
	SetTransformationType(ETransformationType::TT_Rotation);
}

void AUGCGamePawn::TransformationForScale()
{
	SetTransformationType(ETransformationType::TT_Scale);
}

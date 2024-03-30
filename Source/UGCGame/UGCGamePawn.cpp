// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGamePawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "UGCGamePlayerState.h"
#include "Common/MethodUnit.h"

AUGCGamePawn::AUGCGamePawn()
	:bMulitSelect(false)
	, ZoomSpeed(50.f)
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("UCameraComponent"));
	Camera->SetupAttachment(RootComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void AUGCGamePawn::BeginPlay()
{
	Super::BeginPlay();

	SetSnappingEnabled(ETransformationType::TT_Translation, true);
	SetSnappingEnabled(ETransformationType::TT_Rotation, true);
	SetSnappingEnabled(ETransformationType::TT_Scale, true);
	SetSnappingValue(ETransformationType::TT_Translation, 10.f);
	SetSnappingValue(ETransformationType::TT_Rotation, 10.f);
	SetSnappingValue(ETransformationType::TT_Scale, 10.f);
}

void AUGCGamePawn::OnLeftMousePressed()
{
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;
	CollisionChannels.Add(TEnumAsByte<ECollisionChannel>::EnumType::ECC_WorldDynamic);
	CollisionChannels.Add(TEnumAsByte<ECollisionChannel>::EnumType::ECC_WorldStatic);
	ReplicatedMouseTraceByObjectTypes(5000000.f, CollisionChannels, bMulitSelect);
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
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		SetTransformationType(ETransformationType::TT_Translation);
		MyPlayerState->ChangeModifyTypeDelegate.Broadcast(ETransformationType::TT_Translation);
	}
}

void AUGCGamePawn::TransformationForRotation()
{
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		SetTransformationType(ETransformationType::TT_Rotation);
		MyPlayerState->ChangeModifyTypeDelegate.Broadcast(ETransformationType::TT_Rotation);
	}
}

void AUGCGamePawn::TransformationForScale()
{
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		SetTransformationType(ETransformationType::TT_Scale);
		MyPlayerState->ChangeModifyTypeDelegate.Broadcast(ETransformationType::TT_Scale);
	}
}

void AUGCGamePawn::MultiSelect()
{
	bMulitSelect = true;
}

void AUGCGamePawn::DeMultiSelect()
{
	bMulitSelect = false;
}

void AUGCGamePawn::MouseWheelCameraView(float Value)
{
	FVector CurrentLocation = Camera->GetComponentLocation();
	FVector NewLocation = CurrentLocation + (Camera->GetForwardVector() * Value * ZoomSpeed);
	Camera->SetWorldLocation(NewLocation);
}
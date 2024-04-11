// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGamePlayerController.h"
#include "FPSGameCharacterBase.h"


AFPSGamePlayerController::AFPSGamePlayerController()
{

}

void AFPSGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AFPSGamePlayerController::OnLeftMouseButtonDown);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AFPSGamePlayerController::OnLeftMouseButtonUp);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSGamePlayerController::Jump);
	InputComponent->BindAction("LowSpeedWalk", IE_Pressed, this, &AFPSGamePlayerController::OnLeftShiftButtonDown);
	InputComponent->BindAction("LowSpeedWalk", IE_Released, this, &AFPSGamePlayerController::OnLeftShiftButtonUp);
	

	InputComponent->BindAxis("MoveForward", this, &AFPSGamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSGamePlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AFPSGamePlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &AFPSGamePlayerController::LookUpAtRate);
}

void AFPSGamePlayerController::OnLeftMouseButtonDown()
{
	
}

void AFPSGamePlayerController::OnLeftMouseButtonUp()
{
	
}

void AFPSGamePlayerController::Jump()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->Jump();
	}
}

void AFPSGamePlayerController::OnLeftShiftButtonDown()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->LowSpeedWalk();
	}
}

void AFPSGamePlayerController::OnLeftShiftButtonUp()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->NormalSpeedWalk();
	}
}

void AFPSGamePlayerController::MoveForward(float Value)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->MoveForward(Value);
	}
}

void AFPSGamePlayerController::MoveRight(float Value)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->MoveRight(Value);
	}
}

void AFPSGamePlayerController::TurnAtRate(float Rate)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->TurnAtRate(Rate);
	}
}

void AFPSGamePlayerController::LookUpAtRate(float Rate)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->LookUpAtRate(Rate);
	}
}

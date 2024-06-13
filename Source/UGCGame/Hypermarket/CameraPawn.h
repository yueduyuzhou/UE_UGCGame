// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class USceneComponent;
class UCameraComponent;

UCLASS()
class UGCGAME_API ACameraPawn : public APawn
{
	GENERATED_BODY()

	USceneComponent* RootSceneComponent;

	UCameraComponent* CameraComponent;

public:
	ACameraPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"

ACameraPawn::ACameraPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootSceneComponent);
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

}


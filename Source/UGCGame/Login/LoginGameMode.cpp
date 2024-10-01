// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameMode.h"
#include "../Common/ServerManage/ServerManage.h"

ALoginGameMode::ALoginGameMode()
{
	//bUseSeamlessTravel = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ALoginGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World && World->GetFirstPlayerController())
	{
		World->GetFirstPlayerController()->bShowMouseCursor = true;
	}
}

void ALoginGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FServerManage::Get()->Tick(DeltaSeconds);
}

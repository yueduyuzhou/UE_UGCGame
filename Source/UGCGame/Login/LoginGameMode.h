// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoginGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API ALoginGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ALoginGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};

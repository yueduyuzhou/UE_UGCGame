// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSEndGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSEndGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFPSEndGameMode();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void StartPlay() override;
};

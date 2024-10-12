// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HypermarketGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API AHypermarketGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHypermarketGameMode();

protected:
	virtual void Tick(float DeltaSeconds) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSGamePlayerState : public APlayerState
{
	GENERATED_BODY()

	AFPSGamePlayerState();

public:
	void DeathResetData();

public:
	float Health;
	float MaxHealth;

	//分数（人头数）
};

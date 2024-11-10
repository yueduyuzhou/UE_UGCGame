// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	AFPSGameModeBase();

public:
	FORCEINLINE const int32& GetFPSMode() { return FPSMode; }

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 FPSMode;
};

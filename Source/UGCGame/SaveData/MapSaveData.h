// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MapSaveData.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API UMapSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	// ElementID : {FV,FR}[]
	TMap<int32, TArray<TPair<FVector, FRotator>>> SaveMapData;
};

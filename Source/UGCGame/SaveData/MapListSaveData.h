// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MapListSaveData.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API UMapListSaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FString> Maps;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UGCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API UUGCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FString LoadMapName;
};

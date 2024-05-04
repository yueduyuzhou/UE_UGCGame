// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EffectElement.h"
#include "EE_SpawnPoint.generated.h"

UENUM()
enum class ETeamType : uint8
{
	TEAM_RED,
	TEAM_BLUE,
};

/**
 * 
 */
UCLASS()
class UGCGAME_API AEE_SpawnPoint : public AEffectElement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "EffectData")
	ETeamType TeamType;
};

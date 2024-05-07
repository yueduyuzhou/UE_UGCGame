// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EffectElement.h"
#include "EE_SpawnPoint.generated.h"

enum class ETeamType : uint8;

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

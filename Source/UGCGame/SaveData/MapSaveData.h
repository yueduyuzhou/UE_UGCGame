// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MapSaveData.generated.h"

USTRUCT(BlueprintType)
struct FElemInfo
{
	GENERATED_BODY()

	FElemInfo()
		:ElementID(INDEX_NONE)
		, Location(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, Scale(FVector::ZeroVector)
		, TeamType(ETeamType::TEAM_NONE)
		, Color(FLinearColor::White)
	{}

	FElemInfo(int32 InID, FVector InLoc, FRotator InRot, FVector InScale, ETeamType InTeamType, FLinearColor InColor)
		:ElementID(InID)
		,Location(InLoc)
		,Rotation(InRot)
		,Scale(InScale)
		,TeamType(InTeamType)
		,Color(InColor)
	{}

	UPROPERTY()
		int32 ElementID;

	UPROPERTY()
		FVector Location;

	UPROPERTY()
		FRotator Rotation;

	UPROPERTY()
		FVector Scale;

	UPROPERTY()
		ETeamType TeamType;

	UPROPERTY()
		FLinearColor Color;
};

/**
 * 
 */
UCLASS()
class UGCGAME_API UMapSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	// ElementID : {FV,FR}[]
	UPROPERTY()
		TArray<FElemInfo> Elements;

};

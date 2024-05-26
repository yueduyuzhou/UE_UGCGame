// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Common/UGCGameType.h"
#include "UGCGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class UGCGAME_API UUGCGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UUGCGameInstance();

public:
	const ETeamType GetTeamTypeByPlayerID(const int32& InPlayerID);

public:
	FString LoadMapName;
	TArray<FPlayerNetData> PlayerDatas;
	FPlayerNetData LocalPlayerData;
};

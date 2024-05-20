// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../UGCGamePawn.h"
#include "../UGCGameState.h"
#include "../UGCGamePlayerState.h"
#include "../UGCGamePlayerController.h"

namespace MethodUnit
{
	AUGCGameState* GetGameState(UWorld* InWorld);

	AUGCGamePlayerState* GetPlayerState(UWorld* InWorld);

	AUGCGamePawn* GetUGCPlayerPawn(UWorld* InWorld);

	AUGCGamePlayerController* GetPlayerControllerByPlayerID(UWorld* InWorld, int32& InPlayerID);

	const FString EditDetailTypeToString(const EEditDetailType& InType);

	const FString TeamTypeToString(const ETeamType& InType);
}


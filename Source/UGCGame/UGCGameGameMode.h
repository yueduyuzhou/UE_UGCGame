// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UGCGameGameMode.generated.h"

UCLASS(minimalapi)
class AUGCGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUGCGameGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer);
};




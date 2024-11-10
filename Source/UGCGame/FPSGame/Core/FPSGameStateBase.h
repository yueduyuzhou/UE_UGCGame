// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../UGCGameState.h"
#include "FPSGameStateBase.generated.h"

struct FFPSGameConfig;

/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSGameStateBase : public AUGCGameState
{
	GENERATED_BODY()
	
protected:
	AFPSGameStateBase();

public:
	/*获取表数据*/
	FFPSGameConfig* GetGameConfigTemplate(const int32& InID);
	FFPSGameConfig* GetGameConfigTemplate();
	TArray<FFPSGameConfig*>* GetGameConfigsTemplate();

protected:
	UPROPERTY()
		UDataTable* GameCfgPtr;

	TArray<FFPSGameConfig*> CacheGameCfgs;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../Table/HypermarketTable.h"
#include "HypermarketGameState.generated.h"


/**
 * 
 */
UCLASS()
class UGCGAME_API AHypermarketGameState : public AGameStateBase
{
	GENERATED_BODY()

	AHypermarketGameState();

private:
	virtual void BeginPlay() override;

public:
	/*获取表数据*/
	FHypermarketTable* GetWeaponTableTemplate(const int32& InID);
	TArray<FHypermarketTable*>* GetWeaponTablesTemplate();

protected:
	UPROPERTY()
		UDataTable* WeaponTablePtr;

private:
	TArray<FHypermarketTable*> CacheWeaponTables;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../../Table/HypermarketTable.h"
#include "../../Table/ItemTable.h"
#include "FPSEndGameState.generated.h"

//struct FHypermarketTable;

/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSEndGameState : public AGameStateBase
{
	GENERATED_BODY()

		AFPSEndGameState();

private:
	virtual void BeginPlay() override;

public:
	/*获取表数据*/
	FHypermarketTable* GetWeaponTableTemplate(const int32& InID);
	TArray<FHypermarketTable*>* GetWeaponTablesTemplate();
	FItemTable* GetItemTableTemplate(const int32& InID);
	TArray<FItemTable*>* GetItemTablesTemplate();

protected:
	UPROPERTY()
		UDataTable* WeaponTablePtr;

	UPROPERTY()
		UDataTable* ItemTablePtr;
private:
	TArray<FHypermarketTable*> CacheWeaponTables;
	TArray<FItemTable*> CacheItemTables;
};

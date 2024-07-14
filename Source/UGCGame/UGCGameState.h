// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Table/SlotTable.h"
#include "Table/ElementAttribute.h"
#include "System/ELementGenerater.h"
#include "UGCGameState.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS()
class UGCGAME_API AUGCGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AUGCGameState();

private:
	virtual void BeginPlay() override;

public:
	/*获取表数据*/
	const FSlotTable* GetSlotTableTemplate(const int32& InID);
	const FElementAttribute* GetElementAttributeTemplate(const int32& InID);
	const TArray<FSlotTable*>* GetSlotTablesTemplate();
	const TArray<FElementAttribute*>* GetElementAttributesTemplate();

	/*Element Spawn Count*/
	void AddSpawnData(const int32& InID);
	bool SubSpawnData(const int32& InID);
	const int32& GetSpawnCountByID(const int32& InID);

protected:
	UPROPERTY()
		UDataTable* SlotTablePtr;

	UPROPERTY()
		UDataTable* ElementAttributePtr;

private:
	TArray<FSlotTable*> CacheSlotTables;
	TArray<FElementAttribute*> CacheElementAttributes;

	/*key:ElementID, value:当前生成数*/
	TMap<int32, int32> SpanwCounts;
};

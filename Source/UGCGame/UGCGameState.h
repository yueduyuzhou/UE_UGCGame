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

	AUGCGameState();

	virtual void BeginPlay() override;

public:
	/*��ȡ������*/
	const FSlotTable* GetSlotTableTemplate(const int32& InID);
	const FElementAttribute* GetElementAttributeTemplate(const int32& InID);
	const TArray<FSlotTable*>* GetSlotTablesTemplate();
	const TArray<FElementAttribute*>* GetElementAttributesTemplate();

//public:
//	AElementGenerater* ElementGenerater;

protected:
	UPROPERTY()
		UDataTable* SlotTablePtr;

	UPROPERTY()
		UDataTable* ElementAttributePtr;

private:
	TArray<FSlotTable*> CacheSlotTables;
	TArray<FElementAttribute*> CacheElementAttributes;
};
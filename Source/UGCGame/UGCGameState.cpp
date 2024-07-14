// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGameState.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Element/ElementBase.h"

AUGCGameState::AUGCGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Slot_Table(TEXT("/Game/Table/SlotTable"));
	SlotTablePtr = Slot_Table.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> Element_Attribute(TEXT("/Game/Table/ElementAttribute"));
	ElementAttributePtr = Element_Attribute.Object;
}

void AUGCGameState::BeginPlay()
{
	Super::BeginPlay();

	SpanwCounts.Add(TTuple<int32, int32>(-1, -1));

	/*if (GetLocalRole() == ROLE_Authority)
	{
		ElementGenerater = GetWorld()->SpawnActor<AElementGenerater>(AElementGenerater::StaticClass());

		ElementGenerater->SetReplicates(true);
	}*/
}

const FSlotTable* AUGCGameState::GetSlotTableTemplate(const int32& InID)
{
	if (const TArray<FSlotTable*> * SlotData = GetSlotTablesTemplate())
	{
		for (auto* Tmp : (*SlotData))
		{
			if (Tmp->ID == InID)
			{
				return Tmp;
			}
		}
	}
	return nullptr;
}

const FElementAttribute* AUGCGameState::GetElementAttributeTemplate(const int32& InID)
{
	if (const TArray<FElementAttribute*> * ElementAttr = GetElementAttributesTemplate())
	{
		for (auto* Tmp : (*ElementAttr))
		{
			if (Tmp->ID == InID)
			{
				return Tmp;
			}
		}
	}
	return nullptr;
}

const TArray<FSlotTable*>* AUGCGameState::GetSlotTablesTemplate()
{
	if (!CacheSlotTables.Num())
	{
		if (SlotTablePtr)
		{
			SlotTablePtr->GetAllRows(TEXT("Slot Table"), CacheSlotTables);
		}
	}
	return &CacheSlotTables;
}

const TArray<FElementAttribute*>* AUGCGameState::GetElementAttributesTemplate()
{
	if (!CacheElementAttributes.Num())
	{
		if (ElementAttributePtr)
		{
			ElementAttributePtr->GetAllRows(TEXT("Element Attribute"), CacheElementAttributes);
		}
	}
	return &CacheElementAttributes;
}

void AUGCGameState::AddSpawnData(const int32& InID)
{
	if (InID != INDEX_NONE)
	{
		if (SpanwCounts.Contains(InID))
		{
			SpanwCounts[InID]++;
		}
		else
		{
			SpanwCounts.Add(TTuple<int32, int32>(InID, 1));
		}
	}
}

bool AUGCGameState::SubSpawnData(const int32& InID)
{
	if (InID == INDEX_NONE)
	{
		return false;
	}

	if (SpanwCounts.Contains(InID))
	{
		SpanwCounts[InID]--;
	}
	else
	{
		return false;
	}
	return true;
}

const int32& AUGCGameState::GetSpawnCountByID(const int32& InID)
{
	if (InID != INDEX_NONE && SpanwCounts.Contains(InID))
	{
		return SpanwCounts[InID];
	}
	return SpanwCounts[-1];
}


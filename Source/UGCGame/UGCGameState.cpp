// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGameState.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"

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
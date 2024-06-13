// Fill out your copyright notice in the Description page of Project Settings.


#include "HypermarketGameState.h"
#include "Engine/DataTable.h"

AHypermarketGameState::AHypermarketGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Hypermarket_Table(TEXT("/Game/Table/HypermarketTable"));
	WeaponTablePtr = Hypermarket_Table.Object;
}

void AHypermarketGameState::BeginPlay()
{
	Super::BeginPlay();
}

FHypermarketTable* AHypermarketGameState::GetWeaponTableTemplate(const int32& InID)
{
	if (const TArray<FHypermarketTable*> * SlotData = GetWeaponTablesTemplate())
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

TArray<FHypermarketTable*>* AHypermarketGameState::GetWeaponTablesTemplate()
{
	if (!CacheWeaponTables.Num())
	{
		if (WeaponTablePtr)
		{
			WeaponTablePtr->GetAllRows(TEXT("Weapon Table"), CacheWeaponTables);
		}
	}
	return &CacheWeaponTables;
}
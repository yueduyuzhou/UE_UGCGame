// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSEndGameState.h"
#include "Engine/DataTable.h"

AFPSEndGameState::AFPSEndGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Hypermarket_Table(TEXT("/Game/Table/HypermarketTable"));
	WeaponTablePtr = Hypermarket_Table.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> Item_Table(TEXT("/Game/Table/ItemTable"));
	ItemTablePtr = Item_Table.Object;
}

void AFPSEndGameState::BeginPlay()
{
	Super::BeginPlay();
}

FHypermarketTable* AFPSEndGameState::GetWeaponTableTemplate(const int32& InID)
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

TArray<FHypermarketTable*>* AFPSEndGameState::GetWeaponTablesTemplate()
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

FItemTable* AFPSEndGameState::GetItemTableTemplate(const int32& InID)
{
	if (const TArray<FItemTable*> * SlotData = GetItemTablesTemplate())
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

TArray<FItemTable*>* AFPSEndGameState::GetItemTablesTemplate()
{
	if (!CacheItemTables.Num())
	{
		if (ItemTablePtr)
		{
			ItemTablePtr->GetAllRows(TEXT("Item Table"), CacheItemTables);
		}
		else
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("[class AFPSEndGameState]£ºItemTablePtr Is Null"));
		}
	}
	return &CacheItemTables;
}
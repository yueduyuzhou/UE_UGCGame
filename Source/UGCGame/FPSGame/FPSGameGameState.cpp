// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameGameState.h"
#include "FPSGameGameMode.h"
#include "CommonData/FPSGameType.h"
#include "../Table/HypermarketTable.h"

AFPSGameGameState::AFPSGameGameState()
	:BlueTeamKillCount(0)
	, RedTeamKillCount(0)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Hypermarket_Table(TEXT("/Game/Table/HypermarketTable"));
	WeaponTablePtr = Hypermarket_Table.Object;
}

FHypermarketTable* AFPSGameGameState::GetWeaponTableTemplate(const int32& InID)
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

TArray<FHypermarketTable*>* AFPSGameGameState::GetWeaponTablesTemplate()
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

void AFPSGameGameState::KillBlue(const int32& InKillerID, const int32& InKilledID)
{
	BlueTeamKillCount++;
	if (ExistPlayer(InKillerID))
	{
		GetInfoByID(InKillerID).KillCount++;
	}
	if (ExistPlayer(InKilledID))
	{
		GetInfoByID(InKilledID).DeathCount++;
	}
}

void AFPSGameGameState::KillRed(const int32& InKillerID, const int32& InKilledID)
{
	RedTeamKillCount++;
	
	if (ExistPlayer(InKillerID))
	{
		GetInfoByID(InKillerID).KillCount++;
	}
	if (ExistPlayer(InKilledID))
	{
		GetInfoByID(InKilledID).DeathCount++;
	}
}

FFPSPlayerInfo& AFPSGameGameState::GetInfoByID(const int32& InPlayerID)
{
	for (auto& Tmp : FPSPlayerInfos)
	{
		if (Tmp.PlayerID == InPlayerID)
		{
			return Tmp;
		}
	}
	return FPSPlayerInfos[0];
}

bool AFPSGameGameState::ExistPlayer(const int32& InPlayerID)
{
	for (auto& Tmp : FPSPlayerInfos)
	{
		if (Tmp.PlayerID == InPlayerID)
		{
			return true;
		}
	}
	return false;
}

void AFPSGameGameState::RegisterPlayerInfo(const int32& InPlayerID)
{
	if (!ExistPlayer(InPlayerID))
	{
		FFPSPlayerInfo TmpInfo;
		TmpInfo.PlayerID = InPlayerID;
		FPSPlayerInfos.Add(TmpInfo);
	}
	else
	{

	}
}

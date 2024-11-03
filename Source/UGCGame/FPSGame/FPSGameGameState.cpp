// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameGameState.h"
#include "FPSGameGameMode.h"
#include "Common/FPSGameType.h"
#include "../Table/HypermarketTable.h"
#include "../Table/KillReward.h"

AFPSGameGameState::AFPSGameGameState()
	:BlueTeamKillCount(0)
	, RedTeamKillCount(0)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Hypermarket_Table(TEXT("/Game/Table/HypermarketTable"));
	WeaponTablePtr = Hypermarket_Table.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> KillReward_Table(TEXT("/Game/Table/KillReward"));
	KillRewardTablePtr = KillReward_Table.Object;
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

FKillReward* AFPSGameGameState::GetKillRewardTableTemplate(const int32& InID)
{
	UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameState]:Find ID=%d FKillReward "), InID);
	if (const TArray<FKillReward*> * SlotData = GetKillRewardTablesTemplate())
	{
		for (auto* Tmp : (*SlotData))
		{
			if (Tmp->ID == InID)
			{
				UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameState]:return ID Is %d FKillReward "), Tmp->ID);
				return Tmp;
			}
		}
	}
	return nullptr;
}

TArray<FKillReward*>* AFPSGameGameState::GetKillRewardTablesTemplate()
{
	if (!CacheKillRewardTables.Num())
	{
		if (KillRewardTablePtr)
		{
			KillRewardTablePtr->GetAllRows(TEXT("Kill Reward Table"), CacheKillRewardTables);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameState]:KillRewardTablePtr Is Null"));
		}
	}
	return &CacheKillRewardTables;
}

void AFPSGameGameState::KillBlue(const int32& InKillerID, const int32& InKilledID)
{
	//记录队伍击杀数
	BlueTeamKillCount++;
	SettlementDeath(InKillerID, InKilledID);
}

void AFPSGameGameState::KillRed(const int32& InKillerID, const int32& InKilledID)
{
	//记录队伍击杀数
	RedTeamKillCount++;
	SettlementDeath(InKillerID, InKilledID);
}

void AFPSGameGameState::Attack(const int32& InAttackerID, const int32& InAttackedID)
{
	FFPSPlayerInfo& Attacked = GetInfoByID(InAttackedID);
	Attacked.AssisterQueue.AddAssister(InAttackerID);

	//UE_LOG(LogTemp, Display, TEXT("%d ==Attack==> %d"), InAttackerID, InAttackedID);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("%d ==Attack==> %d"), InAttackerID, InAttackedID));
}

void AFPSGameGameState::SettlementDeath(const int32& InKillerPlayerID, const int32& InKilledPlayerID)
{
	FFPSPlayerInfo& Killer = GetInfoByID(InKillerPlayerID);
	FFPSPlayerInfo& Killed = GetInfoByID(InKilledPlayerID);

	//记录个人数据
	Killer.KillCount++;
	Killer.TmpKillCount++;
	Killed.DeathCount++;

	FKillReward* KRData = GetKillRewardTableTemplate(Killed.TmpKillCount);
	if (!KRData) 
	{
		UE_LOG(LogTemp, Error, TEXT("[class AFPSGameGameState]:KRData Is Null"));
		return;
	}

	TArray<FIDCountPair> Items = KRData->Items;
	

	//击杀者奖励
	//TMap<int32, int32>& RewItems = Killer.Items;
	//for (FIDCountPair& Tmp : Items)
	//{
	//	if (RewItems.Contains(Tmp.ItemID)) { RewItems[Tmp.ItemID] += Tmp.Count; }
	//	else { RewItems.Add(Tmp.ItemID, Tmp.Count); }
	//}

	//击杀者和助攻者奖励叠加(BUG)
	int32 Cur = 0;
	while (!Killed.AssisterQueue.IsEmpty())
	{
		int32 AssisterID = Killed.AssisterQueue.Tail();
		Killed.AssisterQueue.PopTail();
		FFPSPlayerInfo& Assister = GetInfoByID(AssisterID);
		//TMap<int32, int32>& AssisRewItems = Assister.Items;
		TArray<int32>& Items_ID = Assister.Items_ID;
		TArray<int32>& Items_Count = Assister.Items_Count;
		int32 Index = INDEX_NONE;

		for (FIDCountPair& Tmp : Items)
		{
			//奖励衰减
			Tmp.Count >>= Cur;

			Index = Items_ID.Find(Tmp.ItemID);
			if (Index != INDEX_NONE) { Items_Count[Index] += Tmp.Count; }
			else 
			{
				Items_ID.Add(Tmp.ItemID);
				Items_Count.Add(Tmp.Count);
			}
			//UE_LOG(LogTemp, Display, TEXT("AssisterID=%d RewItems ItemID=%d Count=%d"), AssisterID, Tmp.ItemID, AssisRewItems[Tmp.ItemID]);
		}

		Cur++;
	}

	Killed.AssisterQueue.ResetQueue();
}

FFPSPlayerInfo& AFPSGameGameState::GetInfoByID(const int32& InPlayerID)
{
	UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameState]:FPSPlayerInfos Length %d"), FPSPlayerInfos.Num());
	for (auto& Tmp : FPSPlayerInfos)
	{
		if (Tmp.PlayerID == InPlayerID)
		{
			UE_LOG(LogTemp, Display, TEXT("[class AFPSGameGameState]:return %d Info"), InPlayerID);
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/FPSGameStateBase.h"
#include "FPSGameGameState.generated.h"

class FPSGameGameMode;
struct FFPSPlayerInfo;
struct FHypermarketTable;
struct FKillReward;

/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSGameGameState : public AFPSGameStateBase
{
	GENERATED_BODY()

	friend class FPSGameGameMode;

	AFPSGameGameState();

public:
	/*获取表数据*/
	FHypermarketTable* GetWeaponTableTemplate(const int32& InID);
	TArray<FHypermarketTable*>* GetWeaponTablesTemplate();

	FKillReward* GetKillRewardTableTemplate(const int32& InID);
	TArray<FKillReward*>* GetKillRewardTablesTemplate();

public:
	FORCEINLINE const int32& GetBlueTeamKillCount() { return BlueTeamKillCount; }
	FORCEINLINE const int32& GetRedTeamKillCount() { return RedTeamKillCount; }
	FORCEINLINE const TArray<FFPSPlayerInfo>& GetFPSPlayerInfos() { return FPSPlayerInfos; }

	/* 每次有玩家被击杀时调用，会进行最终奖励叠加和记录击杀信息 */
	void KillBlue(const int32& InKillerID, const int32& InKilledID);
	void KillRed(const int32& InKillerID, const int32& InKilledID);
	
	/* 每次有玩家被攻击时调用，记录助攻信息 */
	void Attack(const int32& InAttackerID, const int32& InAttackedID);

	/*死亡结算奖励*/
	void SettlementDeath(const int32& InKillerPlayerID, const int32& InKilledPlayerID);


	//获取玩家信息
	FFPSPlayerInfo& GetInfoByID(const int32& InPlayerID);

	bool ExistPlayer(const int32& InPlayerID);
	//注册玩家信息
	void RegisterPlayerInfo(const  int32& InPlayerID);

private:
	int32 BlueTeamKillCount;
	int32 RedTeamKillCount;

	TArray<FFPSPlayerInfo> FPSPlayerInfos;

private:
	UPROPERTY()
	UDataTable* WeaponTablePtr;

	UPROPERTY()
	UDataTable* KillRewardTablePtr;

	TArray<FHypermarketTable*> CacheWeaponTables;
	TArray<FKillReward*> CacheKillRewardTables;
};

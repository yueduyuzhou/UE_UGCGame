// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UGCGameState.h"
#include "FPSGameGameState.generated.h"

class FPSGameGameMode;
struct FFPSPlayerInfo;
struct FHypermarketTable;
/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSGameGameState : public AUGCGameState
{
	GENERATED_BODY()

	friend class FPSGameGameMode;

	AFPSGameGameState();

public:
	/*获取表数据*/
	FHypermarketTable* GetWeaponTableTemplate(const int32& InID);
	TArray<FHypermarketTable*>* GetWeaponTablesTemplate();

public:
	FORCEINLINE const int32& GetBlueTeamKillCount() { return BlueTeamKillCount; }
	FORCEINLINE const int32& GetRedTeamKillCount() { return RedTeamKillCount; }
	FORCEINLINE const TArray<FFPSPlayerInfo>& GetFPSPlayerInfos() { return FPSPlayerInfos; }

	void KillBlue(const int32& InKillerID, const int32& InKilledID);
	void KillRed(const int32& InKillerID, const int32& InKilledID);

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

	TArray<FHypermarketTable*> CacheWeaponTables;
};

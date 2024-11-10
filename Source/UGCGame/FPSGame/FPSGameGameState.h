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
	/*��ȡ������*/
	FHypermarketTable* GetWeaponTableTemplate(const int32& InID);
	TArray<FHypermarketTable*>* GetWeaponTablesTemplate();

	FKillReward* GetKillRewardTableTemplate(const int32& InID);
	TArray<FKillReward*>* GetKillRewardTablesTemplate();

public:
	FORCEINLINE const int32& GetBlueTeamKillCount() { return BlueTeamKillCount; }
	FORCEINLINE const int32& GetRedTeamKillCount() { return RedTeamKillCount; }
	FORCEINLINE const TArray<FFPSPlayerInfo>& GetFPSPlayerInfos() { return FPSPlayerInfos; }

	/* ÿ������ұ���ɱʱ���ã���������ս������Ӻͼ�¼��ɱ��Ϣ */
	void KillBlue(const int32& InKillerID, const int32& InKilledID);
	void KillRed(const int32& InKillerID, const int32& InKilledID);
	
	/* ÿ������ұ�����ʱ���ã���¼������Ϣ */
	void Attack(const int32& InAttackerID, const int32& InAttackedID);

	/*�������㽱��*/
	void SettlementDeath(const int32& InKillerPlayerID, const int32& InKilledPlayerID);


	//��ȡ�����Ϣ
	FFPSPlayerInfo& GetInfoByID(const int32& InPlayerID);

	bool ExistPlayer(const int32& InPlayerID);
	//ע�������Ϣ
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

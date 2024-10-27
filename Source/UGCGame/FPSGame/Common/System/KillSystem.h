// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/*********************************************************************************
	击杀系统
**********************************************************************************/

struct FKillSystem
{

	typedef TFunction<void(const int32&, const int32&)> TKillFunction;

	/*玩家登入的时候执行*/
	void AddKiller(const int32& InPlayerID);

	/*判断玩家时候已添加*/
	bool IsExistPlayer(const int32& InPlayerID);

	void Tick(float DeltaSeconds);

	void Kill(const int32& InKillerPlayerID, const int32& InKilledPlayerID);

	void Death(const int32& InKilledPlayerID);

private:
	void AddKiller(const int32& InKillerPlayerID, const int32& InKilledPlayerID);

public:
	FKillSystem();

	/*和 无人能挡、超神 有关*/
	struct FKillMode
	{
		FKillMode(const int32& InKiller);
		int32 KillNumber;
		const int32 Killer;
		int32 Killed;
	};

	/*和 连续击杀 有关*/
	struct FKill
	{
		FKill(const int32& InKillerPlayerID, const int32& InKilledPlayerID);

		void ResetKillTime();

		int32 CurrentKiller;
		int32 CurrentKilled;
		int32 KillNumber;
		float KillTime;
	};

public:
	TArray<FKillMode> Players;
	TArray<FKill> CurrentKillState;

	/*记录成就*/
public:
	int32 FirstBlood;
	TArray<int32> DoubleKills;
	TArray<int32> TripleKills;
	TArray<int32> QuadraKills;
	TArray<int32> PentaKills;
	TArray<int32> Invincible;
	TArray<int32> Legendary;

	/*代理函数*/
public:
	TKillFunction AceFunc;
	TKillFunction FirstBloodFunc;
	TKillFunction KillFunc;
	TKillFunction DoubkeKillsFunc;
	TKillFunction TripleKillsFunc;
	TKillFunction QuadraKillsFunc;
	TKillFunction PentaKillsFunc;
	TKillFunction InvincibleFunc;
	TKillFunction LegendaryFunc;

};

FORCEINLINE bool operator==(const FKillSystem::FKill& InL, const FKillSystem::FKill& InR)
{
	return InL.CurrentKiller == InR.CurrentKiller;
}

FORCEINLINE bool operator==(const FKillSystem::FKill& InL, const int32& InR)
{
	return InL.CurrentKiller == InR;
}

FORCEINLINE bool operator==(const FKillSystem::FKillMode& InL, const FKillSystem::FKillMode& InR)
{
	return InL.Killer == InR.Killer;
}
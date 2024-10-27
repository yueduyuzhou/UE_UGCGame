// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/*********************************************************************************
	��ɱϵͳ
**********************************************************************************/

struct FKillSystem
{

	typedef TFunction<void(const int32&, const int32&)> TKillFunction;

	/*��ҵ����ʱ��ִ��*/
	void AddKiller(const int32& InPlayerID);

	/*�ж����ʱ�������*/
	bool IsExistPlayer(const int32& InPlayerID);

	void Tick(float DeltaSeconds);

	void Kill(const int32& InKillerPlayerID, const int32& InKilledPlayerID);

	void Death(const int32& InKilledPlayerID);

private:
	void AddKiller(const int32& InKillerPlayerID, const int32& InKilledPlayerID);

public:
	FKillSystem();

	/*�� �����ܵ������� �й�*/
	struct FKillMode
	{
		FKillMode(const int32& InKiller);
		int32 KillNumber;
		const int32 Killer;
		int32 Killed;
	};

	/*�� ������ɱ �й�*/
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

	/*��¼�ɾ�*/
public:
	int32 FirstBlood;
	TArray<int32> DoubleKills;
	TArray<int32> TripleKills;
	TArray<int32> QuadraKills;
	TArray<int32> PentaKills;
	TArray<int32> Invincible;
	TArray<int32> Legendary;

	/*������*/
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
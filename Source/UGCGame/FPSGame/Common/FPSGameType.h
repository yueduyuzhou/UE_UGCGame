// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FPSGameType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	/*PrimaryWeapon*/
	AK47			UMETA(DisplayName = "Ak47"),
	M4A1			UMETA(DisplayName = "M4A1"),
	MP7				UMETA(DisplayName = "MP7"),
	SNIPER			UMETA(DisplayName = "Sniper"),

	/*SecondaryWeapon*/
	DESERTEAGLE		UMETA(DisplayName = "DesertEagle")
	
};

USTRUCT(BlueprintType)
struct FAssisterQueue
{
	GENERATED_USTRUCT_BODY()

	FAssisterQueue()
		:LimitLength(1)
		, HeadIndex(0)
		, TailIndex(0)
		, CurLength(0)
	{
		for (int32 i = 0; i < LimitLength; i++)
		{
			Assisters.Add(INDEX_NONE);
		}
	}

	/* ���������� */
	UPROPERTY()
		TArray<int32> Assisters;

private:
	UPROPERTY()
		int32 LimitLength;

	UPROPERTY()
		int32 HeadIndex;

	UPROPERTY()
		int32 TailIndex;

	UPROPERTY()
		int32 CurLength;

public:
	void AddAssister(int32 InAssisterID);
	void PopFront();
	void PopTail();
	int32 Front();
	int32 Tail();
	void ResetQueue();
	bool IsMax();
	bool IsEmpty();
	int32 GetLength();
	void PrintQueue();
};

USTRUCT(BlueprintType)
struct FFPSPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	FFPSPlayerInfo()
		:PlayerID(INDEX_NONE)
		, KillCount(0)
		, TmpKillCount(0)
		, DeathCount(0)
	{}

public:
	UPROPERTY()
	int32 PlayerID;
	
	/* ���ջ�ɱ�� */
	UPROPERTY()
	int32 KillCount;

	/* ��ʱ��ɱ��Ŀ */
	UPROPERTY()
	int32 TmpKillCount;

	/* ���������� */
	UPROPERTY()
	int32 DeathCount;

	/* ���ս��� */
	UPROPERTY()
	TMap<int32, int32> Items;

	/* �������� */
	UPROPERTY()
	FAssisterQueue AssisterQueue;
};
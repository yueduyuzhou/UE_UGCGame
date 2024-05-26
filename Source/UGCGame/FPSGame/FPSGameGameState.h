// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UGCGameState.h"
#include "FPSGameGameState.generated.h"

class FPSGameGameMode;
struct FFPSPlayerInfo;

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
	void KillBlue(const int32& InKillerID, const int32& InKilledID);
	void KillRed(const int32& InKillerID, const int32& InKilledID);

	//��ȡ�����Ϣ
	bool GetInfoByID(const int32& InPlayerID, FFPSPlayerInfo& OutInfo);

	//ע�������Ϣ
	void RegisterPlayerInfo(const  int32& InPlayerID);

private:
	int32 BlueTeamKillCount;
	int32 RedTeamKillCount;

	TArray<FFPSPlayerInfo> FPSPlayerInfos;
};

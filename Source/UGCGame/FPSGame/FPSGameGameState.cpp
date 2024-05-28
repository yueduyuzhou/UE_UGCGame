// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameGameState.h"
#include "FPSGameGameMode.h"
#include "CommonData/FPSGameType.h"

AFPSGameGameState::AFPSGameGameState()
	:BlueTeamKillCount(0)
	, RedTeamKillCount(0)
{

}

void AFPSGameGameState::KillBlue(const int32& InKillerID, const int32& InKilledID)
{
	BlueTeamKillCount++;
	FFPSPlayerInfo TmpInfo;
	if (GetInfoByID(InKillerID, TmpInfo))
	{
		TmpInfo.KillCount++;
	}
	if (GetInfoByID(InKilledID, TmpInfo))
	{
		TmpInfo.DeathCount++;
	}
}

void AFPSGameGameState::KillRed(const int32& InKillerID, const int32& InKilledID)
{
	RedTeamKillCount++;
	FFPSPlayerInfo TmpInfo;
	if (GetInfoByID(InKillerID, TmpInfo))
	{
		TmpInfo.KillCount++;
	}
	if (GetInfoByID(InKilledID, TmpInfo))
	{
		TmpInfo.DeathCount++;
	}
}

bool AFPSGameGameState::GetInfoByID(const int32& InPlayerID, FFPSPlayerInfo& OutInfo)
{
	for (auto& Tmp : FPSPlayerInfos)
	{
		if (Tmp.PlayerID == InPlayerID)
		{
			OutInfo = Tmp;
			return true;
		}
	}
	return false;
}

void AFPSGameGameState::RegisterPlayerInfo(const int32& InPlayerID)
{
	FFPSPlayerInfo TmpInfo;
	if (!GetInfoByID(InPlayerID, TmpInfo))
	{
		FPSPlayerInfos.Add(TmpInfo);
		FPSPlayerInfos.Last().PlayerID = InPlayerID;
	}
}

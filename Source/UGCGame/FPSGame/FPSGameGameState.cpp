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

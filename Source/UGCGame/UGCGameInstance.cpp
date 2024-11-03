// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGameInstance.h"
#include "Common/PlayerModule/PlayerModule.h"

UUGCGameInstance::UUGCGameInstance()
	:bIsLoadMapForUGC(false)
	,LoadMapName(TEXT("asd"))
{

}

const ETeamType UUGCGameInstance::GetTeamTypeByPlayerID(const int32& InPlayerID)
{
	for (auto& Tmp : PlayerDatas)
	{
		if (Tmp.PlayerID == InPlayerID)
		{
			return Tmp.Team;
		}
	}
	return ETeamType::TEAM_NONE;
}

const FFPSPlayerInfo UUGCGameInstance::GetLocalPlayerFPSInfo()
{
	int32 Len = EndGamePlayerInfos.Num();
	for (int32 i = 0; i < Len; i++)
	{
		if (FString::FromInt(EndGamePlayerInfos[i].PlayerID) == UPlayerModule::Get()->Account)
		{
			return EndGamePlayerInfos[i];
		}
	}
	return FFPSPlayerInfo();
}

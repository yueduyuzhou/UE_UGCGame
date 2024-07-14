// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGameInstance.h"

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

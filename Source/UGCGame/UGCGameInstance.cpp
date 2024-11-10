// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGameInstance.h"
#include "Common/PlayerModule/PlayerModule.h"

UUGCGameInstance::UUGCGameInstance()
	:bIsLoadMapForUGC(false)
	,LoadMapName(TEXT("asd"))
{

}

const FPlayerNetData* UUGCGameInstance::GetNetDataByPlayerID(const int32& InPlayerID)
{
	for (auto& Tmp : PlayerDatas)
	{
		if (Tmp.PlayerID == InPlayerID)
		{
			UE_LOG(LogTemp, Display, TEXT("[class UUGCGameInstance::GetNetDataByPlayerID]£ºPlayer %d"), Tmp.PlayerID);
			return &Tmp;
		}
	}
	return nullptr;
}

const ETeamType UUGCGameInstance::GetTeamTypeByPlayerID(const int32& InPlayerID)
{
	if (const FPlayerNetData * NetData = GetNetDataByPlayerID(InPlayerID))
	{
		return NetData->Team;
	}
	return ETeamType::TEAM_NONE;
}

const TArray<int32> UUGCGameInstance::GetEquipedWeaponByPlayerID(const int32& InPlayerID)
{
	if (const FPlayerNetData * NetData = GetNetDataByPlayerID(InPlayerID))
	{
		for(auto& Tmp : NetData->EquipedWeapons)
			UE_LOG(LogTemp, Display, TEXT("[class UUGCGameInstance::GetEquipedWeaponByPlayerID]£ºWeaponId = %d"), Tmp);
		return NetData->EquipedWeapons;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[class UUGCGameInstance::GetEquipedWeaponByPlayerID]£ºNetData Is Null"));
	}
	return TArray<int32>();
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

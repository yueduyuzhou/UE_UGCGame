// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UGCGame/SaveData/MapListSaveData.h"
#include "../System/GameMapManage.h"

ALobbyPlayerState::ALobbyPlayerState()
{
}

const TArray<FString>& ALobbyPlayerState::GetMapList()
{
	/*UMapListSaveData* SaveMapData = Cast<UMapListSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("MapList"), 0));
	if (SaveMapData)
	{
		return SaveMapData->Maps;
	}*/

	return UGameMapManage::Get()->GetMapNameArray();
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Common/UGCGameType.h"
#include "FPSGame/CommonData/FPSGameType.h"
#include "UGCGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class UGCGAME_API UUGCGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UUGCGameInstance();

public:
	const ETeamType GetTeamTypeByPlayerID(const int32& InPlayerID);

	bool IsLocalTeamWin() { return WinTeam == LocalPlayerData.Team; }

public:
	//加载的地图名字
	FString LoadMapName;

	//所有玩家的ID和队伍类型
	TArray<FPlayerNetData> PlayerDatas;
	//自己的ID和队伍类型
	FPlayerNetData LocalPlayerData;

	//FPS游戏结束后保存玩家的信息
	TArray<FFPSPlayerInfo> EndGamePlayerInfos;
	//FPS游戏获胜队伍
	ETeamType WinTeam;
};

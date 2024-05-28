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
	//���صĵ�ͼ����
	FString LoadMapName;

	//������ҵ�ID�Ͷ�������
	TArray<FPlayerNetData> PlayerDatas;
	//�Լ���ID�Ͷ�������
	FPlayerNetData LocalPlayerData;

	//FPS��Ϸ�����󱣴���ҵ���Ϣ
	TArray<FFPSPlayerInfo> EndGamePlayerInfos;
	//FPS��Ϸ��ʤ����
	ETeamType WinTeam;
};

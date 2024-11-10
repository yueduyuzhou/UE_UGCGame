// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Common/UGCGameType.h"
#include "FPSGame/Common/FPSGameType.h"
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
	const FPlayerNetData* GetNetDataByPlayerID(const int32& InPlayerID);
	const ETeamType GetTeamTypeByPlayerID(const int32& InPlayerID);
	const TArray<int32> GetEquipedWeaponByPlayerID(const int32& InPlayerID);
	const FFPSPlayerInfo GetLocalPlayerFPSInfo();

	bool IsLocalTeamWin() { return WinTeam == LocalPlayerData.Team; }

public:
	/*********************************UGC*******************************/
	/*�Ƿ���ص�ͼ*/
	bool bIsLoadMapForUGC;
	//���صĵ�ͼ����
	FString LoadMapName;

	/*********************************FPS*******************************/
	//������ҵ�ID���������ͣ�װ��
	TArray<FPlayerNetData> PlayerDatas;
	//�Լ���ID�Ͷ�������
	FPlayerNetData LocalPlayerData;

	//FPS��Ϸ�����󱣴���ҵ���Ϣ
	TArray<FFPSPlayerInfo> EndGamePlayerInfos;
	//FPS��Ϸ��ʤ����
	ETeamType WinTeam;
};

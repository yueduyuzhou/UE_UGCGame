// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Common/MethodUnit.h"

class AUGCGameGameMode;
struct FUGC_MAP_ELEMENT_INFO_RESPONSE;
struct FUGC_MAP_INFO_RESPONSE;
struct FElemInfo;

class UGCGAME_API UGameMapManage : public TSharedFromThis<UGameMapManage>
{
public:
	UGameMapManage();
	~UGameMapManage() {}

	static TSharedRef<UGameMapManage> Get();

	static void Destroy();

public:
	/*�򿪵�ͼ��ΪFPS*/
	void OpenMapForFPS(UWorld* InWorld, FString InOpenMapName);

	/*�򿪵�ͼ��ΪUGC*/
	void OpenMapForUGC(UWorld* InWorld, FString InOpenMapName);

	/*����ģ���ͼ*/
	void CreateGameMap(UWorld* InWorld);

	/*�˳���ͼ*/
	void QuitGameMap(UWorld* InWorld);

	/*�˳��������ͼ*/
	void QuitAndSaveMap(UWorld* InWorld);

	/*�����ͼ*/
	void SaveGameMap(UWorld* InWorld);

	/*ΪFPS���ص�ͼ���ݲ�����*/
	void LoadMapDataAndSpawnForFPS(const FString& InSlotName, UWorld* InWorld);

	/*ΪUGC���ص�ͼ���ݲ�����*/
	void LoadMapDataAndSpawnForUGC(const FString& InSlotName, UWorld* InWorld);

	/*-------------------------------------------------------------------------*/
	static void OnUGCElementInfo(FUGC_MAP_ELEMENT_INFO_RESPONSE InData);
	static void OnUGCMapInfo(FUGC_MAP_INFO_RESPONSE InData);

private:
	/*���ص�ͼ���ݲ�����*/
	void LoadMapDataAndSpawn(const FString& InSlotName, UWorld* InWorld, bool InbShowEffectMesh);


private:
	static TSharedPtr<UGameMapManage> GameMapManage;

	static TMap<FString, int32> NameToMapID;

	static TArray<FElemInfo> TmpElements;
};
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
	/*打开地图作为FPS*/
	void OpenMapForFPS(UWorld* InWorld, FString InOpenMapName);

	/*打开地图作为UGC*/
	void OpenMapForUGC(UWorld* InWorld, FString InOpenMapName);

	/*创建模板地图*/
	void CreateGameMap(UWorld* InWorld);

	/*退出地图*/
	void QuitGameMap(UWorld* InWorld);

	/*退出并保存地图*/
	void QuitAndSaveMap(UWorld* InWorld);

	/*保存地图*/
	void SaveGameMap(UWorld* InWorld);

	/*为FPS加载地图数据并生成*/
	void LoadMapDataAndSpawnForFPS(const FString& InSlotName, UWorld* InWorld);

	/*为UGC加载地图数据并生成*/
	void LoadMapDataAndSpawnForUGC(const FString& InSlotName, UWorld* InWorld);

	/*-------------------------------------------------------------------------*/
	static void OnUGCElementInfo(FUGC_MAP_ELEMENT_INFO_RESPONSE InData);
	static void OnUGCMapInfo(FUGC_MAP_INFO_RESPONSE InData);

private:
	/*加载地图数据并生成*/
	void LoadMapDataAndSpawn(const FString& InSlotName, UWorld* InWorld, bool InbShowEffectMesh);


private:
	static TSharedPtr<UGameMapManage> GameMapManage;

	static TMap<FString, int32> NameToMapID;

	static TArray<FElemInfo> TmpElements;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


class FGameMapManage : public AActor
{
	FGameMapManage();

public:
	/*打开地图*/
	static void OpenMap(UWorld* InWorld, FName InOpenMapName);

	/*创建模板地图*/
	static void CreateGameMap(UWorld* InWorld);
	
	/*退出地图*/
	static void QuitGameMap(UWorld* InWorld);

	/*退出并保存地图*/
	static void QuitAndSaveMap(UWorld* InWorld);

private:
	/*保存地图*/
	static void SaveGameMap(UWorld* InWorld);
	
	/*加载地图数据并生成*/
	static void LoadMapDataAndSpawn(const FString& InSlotName, UWorld* InWorld);
};
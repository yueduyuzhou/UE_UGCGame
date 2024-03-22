// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


class FGameMapManage : public AActor
{
	FGameMapManage();

public:
	/*�򿪵�ͼ*/
	static void OpenMap(UWorld* InWorld, FName InOpenMapName);

	/*����ģ���ͼ*/
	static void CreateGameMap(UWorld* InWorld);
	
	/*�˳���ͼ*/
	static void QuitGameMap(UWorld* InWorld);

	/*�˳��������ͼ*/
	static void QuitAndSaveMap(UWorld* InWorld);

private:
	/*�����ͼ*/
	static void SaveGameMap(UWorld* InWorld);
	
	/*���ص�ͼ���ݲ�����*/
	static void LoadMapDataAndSpawn(const FString& InSlotName, UWorld* InWorld);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Common/MethodUnit.h"

class UGCGAME_API UGameMapManage : public TSharedFromThis<UGameMapManage>
{
public:
	~UGameMapManage() {}

	static TSharedRef<UGameMapManage> Get();

	static void Destroy();

public:
	/*�򿪵�ͼ*/
	void OpenMap(UWorld* InWorld, FString InOpenMapName);

	/*����ģ���ͼ*/
	void CreateGameMap(UWorld* InWorld);

	/*�˳���ͼ*/
	void QuitGameMap(UWorld* InWorld);

	/*�˳��������ͼ*/
	void QuitAndSaveMap(UWorld* InWorld);

	/*�����ͼ*/
	void SaveGameMap(UWorld* InWorld);

	/*���ص�ͼ���ݲ�����*/
	void LoadMapDataAndSpawn(const FString& InSlotName, UWorld* InWorld);

private:
	static TSharedPtr<UGameMapManage> GameMapManage;
};
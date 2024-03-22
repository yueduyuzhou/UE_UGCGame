// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MapList.h"
#include "Components/Button.h"
#include "../../Common/MethodUnit.h"
#include "../../System/GameMapManage.h"

void UUI_MapList::NativeConstruct()
{
	Super::NativeConstruct();

	CreateMap->OnClicked.AddDynamic(this, &UUI_MapList::OnCreateMapClick);
}

void UUI_MapList::OnCreateMapClick()
{
	//֪ͨ�����������µ�ͼ
	FGameMapManage::CreateGameMap(GetWorld());

	//CreateSessionBP(3, true);
}

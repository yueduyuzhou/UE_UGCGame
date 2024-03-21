// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MapList.h"
#include "Components/Button.h"

void UUI_MapList::NativeConstruct()
{
	Super::NativeConstruct();

	CreateMap->OnClicked.AddDynamic(this, &UUI_MapList::OnCreateMapClick);
}

void UUI_MapList::OnCreateMapClick()
{
	//TODO:通知服务器创建新地图
}

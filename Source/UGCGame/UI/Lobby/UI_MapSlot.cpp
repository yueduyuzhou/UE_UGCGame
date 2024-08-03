// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MapSlot.h"
#include "UI_MapList.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../../System/GameMapManage.h"
#include "../../UGCGameInstance.h"
#include "UI_MapList.h"

void UUI_MapSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_MapSlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);

	if (UPanelWidget * Parent = GetParent())
	{
		if (MapList)
		{
			//刷新背景图片
			MapList->UpdateBackGround(MapName->GetText().ToString());
		}
	}
}

void UUI_MapSlot::SetMapList(UUI_MapList* InMapList)
{
	MapList = InMapList;
}

void UUI_MapSlot::OnClickedWidget()
{
	if (MapName)
	{
		if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
		{
			MyGameInstance->bIsLoadMapForUGC = true;
			//在创意工坊打开该地图
			UGameMapManage::Get()->OpenMapForUGC(GetWorld(), MapName->GetText().ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_MapSlot] : OnClickedWidget, MapName Is Null"));
	}
}
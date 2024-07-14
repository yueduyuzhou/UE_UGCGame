// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MapSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../../System/GameMapManage.h"
#include "../../UGCGameInstance.h"

void UUI_MapSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_MapSlot::OnClickedWidget()
{
	if (MapName)
	{
		if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(GetGameInstance()))
		{
			MyGameInstance->bIsLoadMapForUGC = true;
			//�ڴ��⹤���򿪸õ�ͼ
			UGameMapManage::Get()->OpenMapForUGC(GetWorld(), MapName->GetText().ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_MapSlot] : OnClickedWidget, MapName Is Null"));
	}
}
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
	UGameMapManage::Get()->OpenMap(GetWorld(), MapName->GetText().ToString());
}
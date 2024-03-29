// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TypeSlot.h"
#include "../UI_Inventory.h"
#include "Components/TextBlock.h"
#include "UGCGame/Common/MethodUnit.h"

void UUI_TypeSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_TypeSlot::OnClickedWidget()
{
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		if (ParentInventory)
		{
			TArray<int32> SlotKeys;
			MyPlayerState->GetInventorySlotNetPackage(SlotType, SlotKeys);
			ParentInventory->LayoutSlot(SlotKeys);
		}
	}
}

void UUI_TypeSlot::SetTypeText(const FText& InText)
{
	TypeText->SetText(InText);
}

void UUI_TypeSlot::SetType(const ESlotType& InType)
{
	SlotType = InType;
}

void UUI_TypeSlot::SetParentInventory(UUI_Inventory* InInventory)
{
	ParentInventory = InInventory;
}

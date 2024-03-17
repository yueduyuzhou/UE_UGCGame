// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InventorySlot.h"
#include "../../../Table/SlotTable.h"
#include "UGCGame/UGCGameState.h"
#include "UGCGame/UGCGamePlayerState.h"
#include "Components/Image.h"
#include "Components/Textblock.h"

FReply UUI_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		FReply Reply = FReply::Handled();
		TSharedPtr<SWidget> SlateWidgetDrag = GetCachedWidget();
		if (SlateWidgetDrag.IsValid())
		{
			Reply.DetectDrag(SlateWidgetDrag.ToSharedRef(), EKeys::RightMouseButton);
			return Reply;
		}
	}

	return FReply::Handled();
}

void UUI_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	//通知生成Element
	if (AUGCGameState * MyGameState = GetGameState())
	{
		if (AUGCGamePlayerState * MyPlayerState = GetPlayerState())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("开始拖动"));

			//呼叫服务器生成Element
			MyPlayerState->RequestSpawnElementOnServer(MyPlayerState->TPlayerID, GetElementID());
		}
	}
}

bool UUI_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UUI_InventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
}

void UUI_InventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UUI_InventorySlot::UpdateSlot()
{
	if (AUGCGameState * MyGameState = GetGameState())
	{
		if (const FSlotTable * SlotData = MyGameState->GetSlotTableTemplate(GetElementID()))
		{
			SlotIcon->SetBrushFromTexture(SlotData->SlotIcon);
			SlotName->SetText(FText::FromName(SlotData->SlotName));
		}
	}
}



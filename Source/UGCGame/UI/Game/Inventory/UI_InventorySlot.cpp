// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InventorySlot.h"
#include "../../../Table/SlotTable.h"
#include "UGCGame/UGCGameState.h"
#include "UGCGame/UGCGamePlayerState.h"
#include "Components/Image.h"
#include "Components/Textblock.h"
#include "UGCGame/Common/ServerManage/ServerManage.h"
#include "UGCGame/Common/ServerManage/Protocols/UGCGameProtocols.h"
#include "UGCGame/Common/ServerManage/Macro/ServerType.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

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

		//FUGC_MAP_ELEMENT_INFO_REQUEST args(321);
		//FServerManage::Get()->Send<FUGC_MAP_ELEMENT_INFO_REQUEST>(SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, &args);
		//FDATA_REQUEST args1("akshd", 123);
		//FServerManage::Get()->Send<FDATA_REQUEST>(SP_C2D_DATA_REQUEST, &args1);

		/*FServerManage::Get()->AddCallback<FUGC_MAP_ELEMENT_INFO_RESPONSE>(SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, UUI_InventorySlot::OnUGCElementInfo);
		FUGC_MAP_ELEMENT_INFO_REQUEST args(0);
		FServerManage::Get()->Send<FUGC_MAP_ELEMENT_INFO_REQUEST>(SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, &args);*/
	}


	return FReply::Handled();
}

void UUI_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	//通知生成Element
	//if (AUGCGameState * MyGameState = GetGameState())
	{
		if (AUGCGamePlayerState * MyPlayerState = GetPlayerState())
		{
			MyPlayerState->SpawnElement(MyPlayerState->TPlayerID, GetElementID());
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


#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif
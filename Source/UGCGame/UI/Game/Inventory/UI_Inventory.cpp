// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "../../../UGCGamePlayerState.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "UI_InventorySlot.h"
#include "ThreadManage.h"

void UUI_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	BindDelegate();
}

void UUI_Inventory::LayoutSlot(const TArray<int32>& InKeys)
{
	if (!SlotArray->GetChildrenCount())
	{
		if (InventorySlotClass)
		{
			//if (AMobaGamePlayerState * PlayerState = GetPlayerState())
			{
				for (int32 MyColum = 0; MyColum < InKeys.Num(); MyColum++)
				{
					if (UUI_InventorySlot * SlotWidget = CreateWidget<UUI_InventorySlot>(GetWorld(), InventorySlotClass))
					{
						if (UUniformGridSlot * GridSlot = SlotArray->AddChildToUniformGrid(SlotWidget))
						{
							GridSlot->SetColumn(MyColum);
							//GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
							//GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
						}
						SlotWidget->SetElementID(InKeys[MyColum]);
						
						//Update：更新Slot数据
						SlotWidget->UpdateSlot();
					}
				}
			}
		}
	}
}

void UUI_Inventory::BindDelegate()
{
	if (AUGCGamePlayerState * MyPlayerState = GetPlayerState())
	{
		MyPlayerState->InitSlotDelegate.AddLambda([&](const TArray<int32>& InKeys)
			{
				LayoutSlot(InKeys);
			});
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.3f, [&]()
			{
				BindDelegate();
			});
	}
}

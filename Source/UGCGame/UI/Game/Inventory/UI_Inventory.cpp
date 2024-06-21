// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "UGCGame/Common/UGCGameType.h"
#include "../../../UGCGamePlayerState.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/HorizontalBox.h"
#include "Types/UI_TypeSlot.h"
#include "UI_InventorySlot.h"
#include "ThreadManage.h"

void UUI_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	if (AUGCGamePlayerState * MyUGCPS = GetPlayerState())
	{
		//获取InventorySlotID
		TArray<int32> SlotKeys;
		MyUGCPS->GetInventorySlotNetPackage(ESlotType::ALL, SlotKeys);

		LayoutSlot(SlotKeys);
		LayoutTypesPanel();
	}
}

void UUI_Inventory::LayoutSlot(const TArray<int32>& InKeys)
{
	SlotArray->ClearChildren();
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

void UUI_Inventory::LayoutTypesPanel()
{
	TypesPanel->ClearChildren();

	UEnum* SlotTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESlotType"), true);
	for (int32 i = 0; i <= (uint8)ESlotType::EFFECT_PROP; i++)
	{
		if (UUI_TypeSlot * SlotWidget = CreateWidget<UUI_TypeSlot>(GetWorld(), TypesPanelSlotClass))
		{
			if (UPanelSlot * BoxSlot = TypesPanel->AddChild(SlotWidget))
			{
				SlotWidget->SetType((ESlotType)i);
				SlotWidget->SetTypeText(SlotTypeEnum->GetDisplayNameTextByIndex(i));
				SlotWidget->SetParentInventory(this);
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_RewardPanel.h"
#include "../Character/UI_EquippedSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "../../FPSGame/FPSEnd/FPSEndGameState.h"

void UUI_RewardPanel::NativeConstruct()
{
	Super::NativeConstruct();

	OKBtn->OnClicked.AddDynamic(this, &UUI_RewardPanel::OnOKBtnClick);
}

void UUI_RewardPanel::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_RewardPanel::SetItemsData(const TMap<int32, int32>& InData)
{
	Data = InData;
	RenderItemList();
}

void UUI_RewardPanel::RenderItemList()
{

	//��ȡPlayerState
	if (AFPSEndGameState * MyGS = GetWorld()->GetGameState<AFPSEndGameState>())
	{
		//��ȡItemTable
		if (const TArray<FItemTable*> * SlotTables = MyGS->GetItemTablesTemplate())
		{
			//��ȡ����InType����Ŀ
			TArray<const FItemTable*> SlotsByID;
			for (const auto& Tmp : *SlotTables)
			{
				if (Data.Contains(Tmp->ID))
				{
					SlotsByID.Add(Tmp);
				}
			}

			ItemList->ClearChildren();
			for (auto* Tmp : SlotsByID)
			{
				if (UUI_HypermarketSlot * InfoSlot = CreateWidget<UUI_HypermarketSlot>(GetWorld(), ItemClass))
				{
					if (UHorizontalBoxSlot * HorSlot = ItemList->AddChildToHorizontalBox(InfoSlot))
					{
						//����Item��Ϣ
						InfoSlot->UpdateSlot(Tmp, false);
						InfoSlot->SetSlotGoldText(Data[Tmp->ID], false);
						UE_LOG(
							LogTemp,
							Warning,
							TEXT("[class UUI_RewardPanel] : Call UpdateSlot ItemID = %d"), Tmp->ID);

					}
				}
			}
		}
	}

}

void UUI_RewardPanel::OnOKBtnClick()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

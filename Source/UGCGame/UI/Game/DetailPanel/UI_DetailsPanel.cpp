// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DetailsPanel.h"
#include "UGCGame/Element/ElementBase.h"
#include "UGCGame/Common/MethodUnit.h"
#include "UI_DetailPanelSlot.h"
#include "Components/VerticalBox.h"

void UUI_DetailsPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (AUGCGamePawn * MyUGCPawn = MethodUnit::GetUGCPlayerPawn(GetWorld()))
	{
		MyUGCPawn->SetDetailsPanel(this);
	}
}

void UUI_DetailsPanel::UpdateDetailsPanel()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Update Details Panel"));

	//��ȡSelectElement�Ŀɱ༭���ԣ�����������������Slot��ӵ�Details
	TArray<EEditDetailType> EditDetails = SelectElement->GetEditDetails();
	if (EditDetails.Num() > 0)
	{
		Details->ClearChildren();

		for (auto& Tmp : EditDetails)
		{
			if (UUI_DetailPanelSlot * DetailSlot = CreateWidget<UUI_DetailPanelSlot>(GetWorld(), DetailPanelSlotClass))
			{
				DetailSlot->UpdateDetailChild(Tmp, SelectElement);
				if (UVerticalBoxSlot * VerSlot = Details->AddChildToVerticalBox(DetailSlot))
				{
					FMargin SlotPadding(0, 2, 0, 2); 
					VerSlot->SetPadding(SlotPadding);
				}
			}
		}
	}
}

void UUI_DetailsPanel::SetSelectElement(AElementBase* InElement)
{
	SelectElement = InElement;

	//����Detail
	UpdateDetailsPanel();
}
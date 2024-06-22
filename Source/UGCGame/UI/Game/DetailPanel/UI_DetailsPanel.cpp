// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DetailsPanel.h"
#include "UGCGame/Element/ElementBase.h"
#include "UGCGame/Common/MethodUnit.h"
#include "UI_DetailPanelSlot.h"
#include "Components/VerticalBox.h"

UUI_DetailsPanel::UUI_DetailsPanel()
	:SelectElement(nullptr)
{}

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

	if (SelectElement && Details)
	{
		Details->ClearChildren();

		//获取SelectElement的可编辑属性，根据属性类型生成Slot添加到Details
		TArray<EEditDetailType> EditDetails = SelectElement->GetEditDetails();
		if (EditDetails.Num() > 0)
		{
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
}

void UUI_DetailsPanel::SetSelectElement(AElementBase* InElement)
{
	//销毁上一个SelectElement关联的细节模板UI
	if (SelectElement)
	{
		SelectElement->DestoryVectorUI();
	}

	SelectElement = InElement;

	//更新Detail
	UpdateDetailsPanel();
}
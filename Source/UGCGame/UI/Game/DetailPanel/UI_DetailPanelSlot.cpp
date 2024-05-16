// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DetailPanelSlot.h"
#include "../../../Common/UGCGameType.h"
#include "../../../Element/ElementBase.h"
#include "Details/UI_DetailVector.h"
#include "Details/DetailEnum/UI_DetailEnum.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"

void UUI_DetailPanelSlot::UpdateDetailChild(const EEditDetailType& InType, AElementBase* InElement)
{
	if (InElement)
	{
		SelectElement = InElement;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_DetailPanelSlot]: UpdateDetailChild, InElement Is Null"));
		return;
	}

	if (InType >= EEditDetailType::DETAIL_LOCATION && InType <= EEditDetailType::DETAIL_SCALE)
	{
		if (UUI_DetailVector * VectorUI = CreateWidget<UUI_DetailVector>(GetWorld(), DetailVectorClass))
		{
			if (UCanvasPanelSlot * CanvasSlot = Canvas->AddChildToCanvas(VectorUI))
			{
				CanvasSlot->SetSize(FVector2D(600, 45));

				if (InType == EEditDetailType::DETAIL_LOCATION)
				{
					VectorUI->SetVectorName(FString(TEXT("Location")));
				}
				else if (InType == EEditDetailType::DETAIL_ROTATION)
				{
					VectorUI->SetVectorName(FString(TEXT("Rotation")));
				}
				else if (InType == EEditDetailType::DETAIL_SCALE)
				{
					VectorUI->SetVectorName(FString(TEXT("Scale")));
				}
				VectorUI->InitXYZ();
				//°ó¶¨ÐÞ¸ÄÊôÐÔ
				VectorUI->BindElementProperty(InType, InElement);
			}
		}
	}
	else if (InType == EEditDetailType::DETAIL_ENUM_TEAMTYPE)
	{
		if (UUI_DetailEnum * EnumUI = CreateWidget<UUI_DetailEnum>(GetWorld(), DetailEnumClass))
		{
			if (UCanvasPanelSlot * CanvasSlot = Canvas->AddChildToCanvas(EnumUI))
			{
				CanvasSlot->SetSize(FVector2D(600, 45));
				EnumUI->SetEnumName();
				EnumUI->BindElementProperty(InType, InElement);
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DetailPanelSlot.h"
#include "../../../Common/UGCGameType.h"
#include "../../../Element/ElementBase.h"
#include "Details/UI_DetailVector.h"
#include "Components/CanvasPanel.h"

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
	//UUI_DetailVector::StaticClass()换成蓝图类
	if (UUI_DetailVector * VectorUI = CreateWidget<UUI_DetailVector>(GetWorld(), DetailVectorClass))
	{
		Canvas->AddChild(VectorUI);
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
		//绑定修改属性
		VectorUI->BindElementProperty(InType, InElement);
	}
}

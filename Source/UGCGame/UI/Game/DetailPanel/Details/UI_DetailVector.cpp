// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DetailVector.h"
#include "../../ElementModify/UI_Item.h"
#include "../../../../Element/ElementBase.h"
#include "Components/TextBlock.h"
#include "ThreadManage.h"

void UUI_DetailVector::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_DetailVector::BindElementProperty(const EEditDetailType& InType, AElementBase* InElement)
{
	RegisterElement(InType, InElement);

	if (X && Y && Z)
	{
		X->BindProperty(InType, InElement);
		Y->BindProperty(InType, InElement);
		Z->BindProperty(InType, InElement);
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&]()
			{
				BindElementProperty(InType, InElement);
			});
	}
}

void UUI_DetailVector::UpdateVector(const EVectorDime& InDime, const float InValue)
{
	if (DetailType == EEditDetailType::DETAIL_LOCATION)
	{
		if (InDime == EVectorDime::X)
		{
			BindElementLocationXDelegate.Execute(InValue, SelectElement);
		}
		else if (InDime == EVectorDime::Y)
		{
			BindElementLocationYDelegate.Execute(InValue, SelectElement);
		}
		else if (InDime == EVectorDime::Z)
		{
			BindElementLocationZDelegate.Execute(InValue, SelectElement);
		}
	}
	else if (DetailType == EEditDetailType::DETAIL_ROTATION)
	{
		if (InDime == EVectorDime::X)
		{
			BindElementRotationXDelegate.Execute(InValue, SelectElement);
		}
		else if (InDime == EVectorDime::Y)
		{
			BindElementRotationYDelegate.Execute(InValue, SelectElement);
		}
		else if (InDime == EVectorDime::Z)
		{
			BindElementRotationZDelegate.Execute(InValue, SelectElement);
		}
	}
	else if (DetailType == EEditDetailType::DETAIL_SCALE)
	{
		if (InDime == EVectorDime::X)
		{
			BindElementScaleXDelegate.Execute(InValue, SelectElement);
		}
		else if (InDime == EVectorDime::Y)
		{
			BindElementScaleYDelegate.Execute(InValue, SelectElement);
		}
		else if (InDime == EVectorDime::Z)
		{
			BindElementScaleZDelegate.Execute(InValue, SelectElement);
		}
	}
}

void UUI_DetailVector::UpdateVectorText(const FVector& InVector)
{
	X->SetVectorText(InVector.X);
	Y->SetVectorText(InVector.Y);
	Z->SetVectorText(InVector.Z);
}

void UUI_DetailVector::SetVectorName(const FString& InName)
{
	if (Name)
	{
		Name->SetText(FText::FromString(InName));
	}
}

void UUI_DetailVector::InitXYZ()
{
	if (X && Y && Z)
	{
		X->SetIsSnappingValue(false);
		Y->SetIsSnappingValue(false);
		Z->SetIsSnappingValue(false);

		X->RegisterParentDetailVector(this);
		Y->RegisterParentDetailVector(this);
		Z->RegisterParentDetailVector(this);
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&]()
			{
				InitXYZ();
			});
	}
}

void UUI_DetailVector::RegisterElement(const EEditDetailType& InType, AElementBase* InElement)
{
	Super::RegisterElement(InType, InElement);
	if (InElement)
	{
		SelectElement->RegisterDetailVectorByType(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_DetailVector]: BindElementProperty, InElement Is Null"));
		return;
	}
}

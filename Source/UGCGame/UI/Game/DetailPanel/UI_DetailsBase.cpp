// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DetailsBase.h"
#include "../../../Element/ElementBase.h"

void UUI_DetailsBase::BindElementProperty(const EEditDetailType& InType, AElementBase* InElement)
{
}

void UUI_DetailsBase::RegisterElement(const EEditDetailType& InType, AElementBase* InElement)
{
	if (InElement)
	{
		SelectElement = InElement;
		DetailType = InType;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_DetailsBase]: RegisterElement, InElement Is Null"));
	}
}
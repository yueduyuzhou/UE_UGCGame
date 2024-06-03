// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DetailColor.h"
#include "Components/Image.h"
#include "../../../../Element/BuildElement.h"

void UUI_DetailColor::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_DetailColor::BindElementProperty(const EEditDetailType& InType, AElementBase* InElement)
{
	RegisterElement(InType, InElement);


}

void UUI_DetailColor::RegisterElement(const EEditDetailType& InType, AElementBase* InElement)
{
	Super::RegisterElement(InType, InElement);
}

void UUI_DetailColor::SetElementColor(FLinearColor NewColor)
{
	if (ABuildElement * BElement = Cast<ABuildElement>(SelectElement))
	{
		BElement->SetElementMeshColor(NewColor);
	}
}

FLinearColor UUI_DetailColor::GetElementColor()
{
	if (ABuildElement * BElement = Cast<ABuildElement>(SelectElement))
	{
		return BElement->GetElementMeshColor();
	}
	return FLinearColor();
}


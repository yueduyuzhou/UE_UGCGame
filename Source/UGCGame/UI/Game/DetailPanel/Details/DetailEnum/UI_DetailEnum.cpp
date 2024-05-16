// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DetailEnum.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "UGCGame/Element/ElementBase.h"
#include "UGCGame/Common/UGCGameType.h"

void UUI_DetailEnum::NativeConstruct()
{
	Super::NativeConstruct();

	PopulateComboBox();

	if (EnumSelect)
	{
		EnumSelect->OnSelectionChanged.AddDynamic(this, &UUI_DetailEnum::OnComboBoxSelectionChanged);
	}
}

void UUI_DetailEnum::SetEnumName()
{
	if (Name)
	{
		Name->SetText(FText::FromString(TEXT("EnumName")));
	}
}

void UUI_DetailEnum::BindElementProperty(const EEditDetailType& InType, AElementBase* InElement)
{
	RegisterElement(InType, InElement);
}

void UUI_DetailEnum::RegisterElement(const EEditDetailType& InType, AElementBase* InElement)
{
	if (InElement)
	{
		SelectElement = InElement;
		DetailType = InType;
		//SelectElement->RegisterDetailVectorByType(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_DetailEnum]: RegisterElement, InElement Is Null"));
		return;
	}
}

void UUI_DetailEnum::PopulateComboBox()
{
}

void UUI_DetailEnum::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
}

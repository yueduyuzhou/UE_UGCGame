// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_NetSetting.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUI_NetSetting::LeftButtonClick()
{
	Index = (Index - 1 + SetValues.Num()) % SetValues.Num();
	UpdateSetValue();
}

void UUI_NetSetting::RightButtonClick()
{
	Index = (Index + 1) % SetValues.Num();
	UpdateSetValue();
}

void UUI_NetSetting::UpdateSetValue()
{
	SetValue->SetText(SetValues[Index]);
}

void UUI_NetSetting::NativeConstruct()
{
	Super::NativeConstruct();

	Index = 0;

	SetValue->SetText(SetValues[0]);
	SetName->SetText(SettingName);

	LeftButton->OnClicked.AddDynamic(this, &UUI_NetSetting::LeftButtonClick);
	RightButton->OnClicked.AddDynamic(this, &UUI_NetSetting::RightButtonClick);
}

FText UUI_NetSetting::GetCurrentValue()
{
	return SetValues[Index];
}

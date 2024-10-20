// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_NetSetting.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ThreadManage.h"

#pragma optimize("", off)

void UUI_NetSetting::LeftButtonClick()
{
	if (Values.Num() > 0)
	{
		Index = (Index - 1 + Values.Num()) % Values.Num();
		UpdateSetValue();
	}
}

void UUI_NetSetting::RightButtonClick()
{
	if (Values.Num() > 0)
	{
		Index = (Index + 1) % Values.Num();
		UpdateSetValue();
	}
}

void UUI_NetSetting::UpdateSetValue()
{
	if (Index >= 0 && Index < Values.Num())
	{
		SetValue->SetText(Values[Index]);
	}
}

void UUI_NetSetting::NativeConstruct()
{
	Super::NativeConstruct();

	Index = 0;

	SetName->SetText(SettingName);

	LeftButton->OnClicked.AddDynamic(this, &UUI_NetSetting::LeftButtonClick);
	RightButton->OnClicked.AddDynamic(this, &UUI_NetSetting::RightButtonClick);

	GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
	{
		InitValueDelegate.ExecuteIfBound();

		if (Values.Num() > 0)
		{
			SetValue->SetText(Values[0]);
		}
	});
		
}

FText UUI_NetSetting::GetCurrentValue()
{
	if (Index >= 0 && Index < Values.Num())
	{
		return Values[Index];
	}
	return FText::FromString(FString(TEXT("##")));
}

void UUI_NetSetting::SetValues(const TArray<FString>& InValues)
{
	Values.Empty();
	for (auto& Tmp : InValues)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("%s"), *Tmp));
		Values.Add(FText::FromString(Tmp));
	}
}

#pragma optimize("", on)
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ColorValue.h"
#include "Components/EditableText.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UUI_ColorValue::NativeConstruct()
{
	Super::NativeConstruct();
	if (HSVType == EHSVType::HUE)
	{
		MaxValue = 360.f;
		HSVName->SetText(FText::FromString(TEXT("H ")));
	}
	else if (HSVType == EHSVType::SATURATION)
	{
		MaxValue = 1.f;
		HSVName->SetText(FText::FromString(TEXT("S ")));
	}
	else if (HSVType == EHSVType::VALUE)
	{
		MaxValue = 1.f;
		HSVName->SetText(FText::FromString(TEXT("V ")));
	}
}

void UUI_ColorValue::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Value)
	{
		FString TmpS = Value->GetText().ToString();

		if (TmpS.IsNumeric())
		{
			float CurVal = FCString::Atof(*TmpS);
			ColorValueBar->SetPercent(CurVal / MaxValue);
		}
	}
}

void UUI_ColorValue::UpdateValue(float InVal)
{
	InVal = FMath::Clamp<float>(InVal, 0.f, MaxValue);
	Value->SetText(FText::AsNumber(InVal));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DownTime.h"
#include "ThreadManage.h"
#include "Components/TextBlock.h"
#include "../../../FPSGamePlayerController.h"

UUI_DownTime::UUI_DownTime()
	:bStartDownTime(false)
	, DownTime(50.f)
{

}

void UUI_DownTime::StartDownTime(const float& InDownTime)
{
	DownTime = InDownTime;
	bStartDownTime = true;
}

void UUI_DownTime::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterToPlayerController();
}

void UUI_DownTime::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bStartDownTime)
	{
		DownTime -= InDeltaTime;
		if (DownTime <= 0.f)
		{
			DownTime = 0.f;
		}

		if (Time)
		{
			//¸üÐÂText
			int32 Minutes = FMath::FloorToInt(DownTime / 60.0f);
			int32 Seconds = FMath::FloorToInt(FMath::Fmod(DownTime, 60.0f));

			FString TimeString = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
			Time->SetText(FText::FromString(TimeString));
		}
	}
}

void UUI_DownTime::RegisterToPlayerController()
{
	if (AFPSGamePlayerController * PlayerController = Cast<AFPSGamePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->DownTimeUI = this;
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&]()
			{
				RegisterToPlayerController();
			});
	}
}

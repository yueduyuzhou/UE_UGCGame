// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Crosshair.h"
#include "ThreadManage.h"
#include "Components/TextBlock.h"
#include "../../FPSGamePlayerController.h"
#include "Components/ProgressBar.h"

void UUI_Crosshair::UpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo)
{
	UE_LOG(LogTemp, Warning, TEXT("[class UUI_Crosshair] : Call UpdateAmmo"));
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("[class UUI_Crosshair] : Call UpdateAmmo")));
	
	CurrentClipAmmo->SetText(FText::FromString(FString::FromInt(InCurrentClipAmmo)));
	CurrentAmmo->SetText(FText::FromString(FString::FromInt(InCurrentAmmo)));
}

void UUI_Crosshair::UpdateHealth(const float& InHealth, const float& InMaxHealth)
{
	HealthVal->SetText(FText::FromString(FString::FromInt((int32)InHealth)));
	HealthBar->SetPercent(InHealth / InMaxHealth);
	DoHealthAnim((int32)InHealth);

	if (InHealth >= 100)
	{
		StrongState();
	}
	else if (InHealth <= 50 && InHealth > 30)
	{
		InjureState();
	}
	else if (InHealth <= 30)
	{
		AgonalState();
	}
}

void UUI_Crosshair::RegisterToPlayerController()
{
	if (AFPSGamePlayerController * PlayerController = Cast<AFPSGamePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->CrosshairUI = this;
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&]()
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, TEXT("[class UUI_Crosshair]: Repeat Call RegisterToPlayerController"));
				RegisterToPlayerController();
			});
	}
}

void UUI_Crosshair::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterToPlayerController();
}

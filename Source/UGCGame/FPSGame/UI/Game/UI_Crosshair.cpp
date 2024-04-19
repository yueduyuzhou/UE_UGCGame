// Fill out your copyright notice in the Description page of Project Settings.

#include "ThreadManage.h"
#include "UI_Crosshair.h"
#include "../../FPSGamePlayerController.h"
#include "Components/TextBlock.h"

void UUI_Crosshair::UpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo)
{
	CurrentClipAmmo->SetText(FText::FromString(FString::FromInt(InCurrentClipAmmo)));
	CurrentAmmo->SetText(FText::FromString(FString::FromInt(InCurrentAmmo)));
}

void UUI_Crosshair::NativeConstruct()
{
	Super::NativeConstruct();

	if (AFPSGamePlayerController * PlayerController = Cast<AFPSGamePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->CrosshairUI = this;
	}
}

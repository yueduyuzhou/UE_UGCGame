// Fill out your copyright notice in the Description page of Project Settings.

#include "ThreadManage.h"
#include "UI_Crosshair.h"
#include "../../FPSGamePlayerController.h"

void UUI_Crosshair::NativeConstruct()
{
	Super::NativeConstruct();

	if (AFPSGamePlayerController * PlayerController = Cast<AFPSGamePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->CrosshairUI = this;
	}
}

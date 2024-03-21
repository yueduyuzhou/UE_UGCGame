// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Settings.h"
#include "Components/Button.h"
#include "../../../Common/MethodUnit.h"

void UUI_Settings::OnClickedWidget()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("Click"));
	//通知服务器
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		MyPlayerState->RequestSaveAndQuitOnServer();
	}
}

void UUI_Settings::NativeConstruct()
{
	Super::NativeConstruct();
}
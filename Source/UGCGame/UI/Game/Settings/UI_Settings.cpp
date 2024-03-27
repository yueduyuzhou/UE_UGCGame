// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Settings.h"
#include "Components/Button.h"
#include "../../../Common/MethodUnit.h"
#include "../../../System/GameMapManage.h"

void UUI_Settings::OnClickedWidget()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("Click"));
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		DestorySessionBP();

		if (MyPlayerState->GetLocalRole() == ROLE_Authority)
		{
			UGameMapManage::Get()->QuitAndSaveMap(GetWorld());
		}
		else
		{
			UGameMapManage::Get()->QuitGameMap(GetWorld());
		}
	}
	
}

void UUI_Settings::NativeConstruct()
{
	Super::NativeConstruct();
}
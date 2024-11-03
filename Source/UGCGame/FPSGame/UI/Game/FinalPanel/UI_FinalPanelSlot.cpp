// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_FinalPanelSlot.h"
#include "Components/Border.h"
#include "../../../Common/FPSGameType.h"
#include "../../../../Common/PlayerModule/PlayerModule.h"

void UUI_FinalPanelSlot::RefreshSlot(const int32& InRankNum, const FFPSPlayerInfo& InInfo)
{
	Rank->SetText(FText::FromString(FString::FromInt(InRankNum)));
	PlayerID->SetText(FText::FromString(FString::FromInt(InInfo.PlayerID)));
	Kill->SetText(FText::FromString(FString::FromInt(InInfo.KillCount)));
	Death->SetText(FText::FromString(FString::FromInt(InInfo.DeathCount)));
	Score->SetText(FText::FromString(FString::FromInt(
		InInfo.KillCount * 15 - InInfo.DeathCount * 10)));

	if (UPlayerModule::Get()->Account == FString::FromInt(InInfo.PlayerID))
	{
		BackGround->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.4f));
	}
	else
	{
		BackGround->SetBrushColor(FLinearColor(0.625f, 0.625f, 0.625f, 0.4f));
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TopInfoPanel.h"
#include "ThreadManage.h"
#include "Components/TextBlock.h"
#include "../../../FPSGamePlayerController.h"
#include "../../../../Common/UGCGameType.h"

UUI_TopInfoPanel::UUI_TopInfoPanel()
	:BlueKillCount(0)
	, RedKillCount(0)
{
}

void UUI_TopInfoPanel::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterToPlayerController();

	if (BlueKillText && RedKillText)
	{
		BlueKillText->SetText(FText::FromString(TEXT("0")));
		RedKillText->SetText(FText::FromString(TEXT("0")));
	}
}

void UUI_TopInfoPanel::RegisterToPlayerController()
{
	if (AFPSGamePlayerController * PlayerController = Cast<AFPSGamePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->TopInfoPanelUI = this;
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&]()
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, TEXT("[class UUI_TopInfoPanel]: Repeat Call RegisterToPlayerController"));

				RegisterToPlayerController();
			});
	}
}

void UUI_TopInfoPanel::UpdateKillText(const ETeamType& InKilTeamType)
{
	if (InKilTeamType == ETeamType::TEAM_BLUE)
	{
		BlueKillCount++;
		BlueKillText->SetText(FText::AsNumber(BlueKillCount));
	}
	else if (InKilTeamType == ETeamType::TEAM_RED)
	{
		RedKillCount++;
		RedKillText->SetText(FText::AsNumber(RedKillCount));
	}
}
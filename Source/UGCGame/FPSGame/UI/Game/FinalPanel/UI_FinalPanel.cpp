// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_FinalPanel.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI_FinalPanelSlot.h"
#include "UGCGame/UGCGameInstance.h"

void UUI_FinalPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (UUGCGameInstance * MyGI = GetGameInstance<UUGCGameInstance>())
	{
		if (MyGI->IsLocalTeamWin())
		{
			Result->SetText(FText::FromString(FString(TEXT("Win"))));
		}
		else
		{
			Result->SetText(FText::FromString(FString(TEXT("Defeated"))));
		}

		PlayerList->ClearChildren();
		for (int32 i = 0; i < MyGI->EndGamePlayerInfos.Num(); i++)
		{
			if (UUI_FinalPanelSlot * InfoSlot = CreateWidget<UUI_FinalPanelSlot>(GetWorld(), FinalPanelSlotClass))
			{
				if (UVerticalBoxSlot * VerSlot = PlayerList->AddChildToVerticalBox(InfoSlot))
				{
					//ÉèÖÃÊý¾Ý
					InfoSlot->Rank->SetText(FText::FromString(FString::FromInt(i + 1)));
					InfoSlot->Kill->SetText(FText::FromString(FString::FromInt(MyGI->EndGamePlayerInfos[i].KillCount)));
					InfoSlot->Death->SetText(FText::FromString(FString::FromInt(MyGI->EndGamePlayerInfos[i].DeathCount)));
					InfoSlot->Score->SetText(FText::FromString(FString::FromInt(
						MyGI->EndGamePlayerInfos[i].KillCount * 15 - 
						MyGI->EndGamePlayerInfos[i].DeathCount * 10)));

				}
			}
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_FinalPanel.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI_FinalPanelSlot.h"
#include "UGCGame/UGCGameInstance.h"
#include "UGCGame/UI/Reward/UI_RewardPanel.h"
#include "UGCGame/Common/ServerManage/ServerManage.h"

void UUI_FinalPanel::NativeConstruct()
{
	Super::NativeConstruct();

	InitPlayerList();
	InitRewardPanel();

	SettlementReward();
}

void UUI_FinalPanel::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_FinalPanel::InitPlayerList()
{
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
					/*GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
						FString::Printf(
							TEXT("[class UUI_FinalPanel] : NativeConstruct, Player_%d KillCount = %d, DeathCount = %d"),
							i + 1,
							MyGI->EndGamePlayerInfos[i].KillCount,
							MyGI->EndGamePlayerInfos[i].DeathCount));*/

							//ÉèÖÃÊý¾Ý
					InfoSlot->RefreshSlot(i + 1, MyGI->EndGamePlayerInfos[i]);
					/*InfoSlot->Rank->SetText(FText::FromString(FString::FromInt(i + 1)));
					InfoSlot->PlayerID->SetText(FText::FromString(FString::FromInt(MyGI->EndGamePlayerInfos[i].PlayerID)));
					InfoSlot->Kill->SetText(FText::FromString(FString::FromInt(MyGI->EndGamePlayerInfos[i].KillCount)));
					InfoSlot->Death->SetText(FText::FromString(FString::FromInt(MyGI->EndGamePlayerInfos[i].DeathCount)));
					InfoSlot->Score->SetText(FText::FromString(FString::FromInt(
						MyGI->EndGamePlayerInfos[i].KillCount * 15 -
						MyGI->EndGamePlayerInfos[i].DeathCount * 10)));*/

				}
			}
		}
	}
}

void UUI_FinalPanel::InitRewardPanel()
{
	if (UUGCGameInstance * MyGI = GetGameInstance<UUGCGameInstance>())
	{
		FFPSPlayerInfo PInfo = MyGI->GetLocalPlayerFPSInfo();
		if (PInfo.PlayerID != INDEX_NONE )
		{
			if (PInfo.Items_ID.Num())
			{
				RewardPanel->SetVisibility(ESlateVisibility::Visible);
				RewardPanel->SetItemsData(PInfo.ItemsToMap());
			}
			else
				RewardPanel->SetVisibility(ESlateVisibility::Collapsed);
			
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("~~~~~~~PInfo : %s~~~~~~"), *PInfo.InfoToString()));
			UE_LOG(LogTemp, Warning, TEXT("~~~~~~~PInfo : %s~~~~~~"), *PInfo.InfoToString());
		}
		else
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("[class UUI_FinalPanel] : PInfo PlayerID Is %d"), PInfo.PlayerID);
		}
	}
}

void UUI_FinalPanel::SettlementReward()
{
	if (UUGCGameInstance * MyGI = GetGameInstance<UUGCGameInstance>())
	{
		FFPSPlayerInfo PInfo = MyGI->GetLocalPlayerFPSInfo();
		FSETTLEMENT_REWARD_REQ InData = FSETTLEMENT_REWARD_REQ(PInfo.Items_ID, PInfo.Items_Count);
		FServerManage::Get()->Send<FSETTLEMENT_REWARD_REQ>(SP_C2D_SETTLEMENT_REWARD_REQ, &InData);
	}
}

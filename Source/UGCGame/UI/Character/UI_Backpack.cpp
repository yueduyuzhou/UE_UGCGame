// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Backpack.h"
#include "UI_HypermarketSlot.h"
#include "UGCGame/SaveData/PlayerSaveData.h"
#include "ThreadManage.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/GridSlot.h"
#include "Components/GridPanel.h"
#include "Components/ComboBoxString.h"
#include "../../Table/HypermarketTable.h"
#include "../../Hypermarket/HypermarketHUD.h"
#include "../../Hypermarket/HypermarketGameState.h"

void UUI_Backpack::NativeConstruct()
{
	Super::NativeConstruct();

	if (Selecter)
	{
		Selecter->OnSelectionChanged.AddDynamic(this, &UUI_Backpack::OnComboBoxSelectionChanged);
	}

	XButton->OnReleased.AddDynamic(this, &UUI_Backpack::OnXButtonClicked);

	UpdateItem(EHypermarkType::ALL);
}

void UUI_Backpack::UpdateItem(EHypermarkType InType)
{
	if (ItemGrid->GetAllChildren().Num() > 0)
	{
		ItemGrid->ClearChildren();
	}

	if (UPlayerSaveData * SaveMapData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerData_00"), 0)))
	{
		//获取PlayerState
		if (AHypermarketGameState * MyGS = GetWorld()->GetGameState<AHypermarketGameState>())
		{
			//获取HypermarketTable
			if (const TArray<FHypermarketTable*> * SlotTables = MyGS->GetWeaponTablesTemplate())
			{
				//获取符合InType的条目
				TArray<const FHypermarketTable*> SlotsByType;
				for (const auto& Tmp : *SlotTables)
				{
					if (Tmp->Types.Contains(InType))
					{
						SlotsByType.Add(Tmp);
					}
				}

				int32 Index = 0;
				//为每个条目创建UI
				for (int32 i = 0; i < SlotsByType.Num(); i++)
				{
					//是否为拥有的
					if (SaveMapData->ItemContains(SlotsByType[i]->ID))
					{
						if (UUI_HypermarketSlot * InWidgetItem = CreateWidget<UUI_HypermarketSlot>(GetWorld(), ItemClass))
						{
							if (UGridSlot * GridSlot = ItemGrid->AddChildToGrid(InWidgetItem))
							{
								//Slot排布
								GridSlot->SetRow(Index / 4);
								GridSlot->SetColumn(Index % 4);
								Index++;

								GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
								GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
								GridSlot->SetPadding(FMargin(20));

								//更新Item信息
								InWidgetItem->UpdateSlot(SlotsByType[i]);
								//绑定点击代理
								//InWidgetItem->HypermarketDelegate.BindUObject(this, &UUI_Hypermarket::UpdateItemImage);
							}
						}
					}
				}
			}
		}
		else
		{
			//失败尝试
			GThread::Get()->GetCoroutines().BindLambda(0.3f, [&]()
				{
					UpdateItem(InType);
				});
		}
	}
	
}

void UUI_Backpack::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "All")
	{
		UpdateItem(EHypermarkType::ALL);
	}
	else if (SelectedItem == "PrimaryWeapon")
	{
		UpdateItem(EHypermarkType::PRIMARYWEAPON);
	}
	else if (SelectedItem == "SecondaryWeapon")
	{
		UpdateItem(EHypermarkType::SECONDARYWEAPON);
	}
	else if (SelectedItem == "CloseRangeWeapon")
	{
		UpdateItem(EHypermarkType::CLOSERANGEWEAPON);
	}
}

void UUI_Backpack::OnXButtonClicked()
{
	//关闭背包
	if (APlayerController * MyPC = GetWorld()->GetFirstPlayerController<APlayerController>())
	{
		if (AHypermarketHUD * MyHyperHUD = MyPC->GetHUD<AHypermarketHUD>())
		{
			TArray<UWidget*> MainChilds = MyHyperHUD->GetMainScreenChildrens();
			for (auto* Tmp : MainChilds)
			{
				if (UUI_Backpack * BackpackUI = Cast<UUI_Backpack>(Tmp))
				{
					BackpackUI->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
}
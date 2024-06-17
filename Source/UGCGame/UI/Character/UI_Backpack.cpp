// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Backpack.h"
#include "UI_EquippedSlot.h"
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

	EquippedPrimary->SetEquippedName(FString(TEXT("Primary")));
	EquippedSecondary->SetEquippedName(FString(TEXT("Secondary")));
	EquippedCloseRange->SetEquippedName(FString(TEXT("CloseRange")));
	EquippedGrenade->SetEquippedName(FString(TEXT("Grenade")));

	UpdateItem(EHypermarkType::ALL);
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

void UUI_Backpack::UpdateItem(EHypermarkType InType)
{
	if (ItemGrid->GetAllChildren().Num() > 0)
	{
		ItemGrid->ClearChildren();
	}

	if (UPlayerSaveData * SaveMapData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerData_00"), 0)))
	{
		//��ȡPlayerState
		if (AHypermarketGameState * MyGS = GetWorld()->GetGameState<AHypermarketGameState>())
		{
			//��ȡHypermarketTable
			if (const TArray<FHypermarketTable*> * SlotTables = MyGS->GetWeaponTablesTemplate())
			{
				//��ȡ����InType����Ŀ
				TArray<const FHypermarketTable*> SlotsByType;
				for (const auto& Tmp : *SlotTables)
				{
					if (Tmp->Types.Contains(InType))
					{
						SlotsByType.Add(Tmp);
					}
				}

				int32 Index = 0;
				//Ϊÿ����Ŀ����UI
				for (int32 i = 0; i < SlotsByType.Num(); i++)
				{
					//�Ƿ�Ϊӵ�е�
					if (SaveMapData->ItemContains(SlotsByType[i]->ID))
					{
						if (UUI_EquippedSlot * InWidgetItem = CreateWidget<UUI_EquippedSlot>(GetWorld(), ItemClass))
						{
							if (UGridSlot * GridSlot = ItemGrid->AddChildToGrid(InWidgetItem))
							{
								//Slot�Ų�
								GridSlot->SetRow(Index / 4);
								GridSlot->SetColumn(Index % 4);
								Index++;

								GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
								GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
								GridSlot->SetPadding(FMargin(20));


								//��������EquippedName
								if (InWidgetItem->EquippedName)
								{
									InWidgetItem->EquippedName->SetVisibility(ESlateVisibility::Hidden);
								}
								//����Item��Ϣ
								InWidgetItem->UpdateSlot(SlotsByType[i]);
								//�󶨵������
								InWidgetItem->EquippedtDelegate.BindUObject(this, &UUI_Backpack::UpdateEquippedSlot);
							}
						}
					}
				}
			}
		}
		else
		{
			//ʧ�ܳ���
			GThread::Get()->GetCoroutines().BindLambda(0.3f, [&]()
				{
					UpdateItem(InType);
				});
		}
	}
	
}

void UUI_Backpack::OnXButtonClicked()
{
	//�رձ���
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

void UUI_Backpack::UpdateEquippedSlot(const int32& InID)
{
	//����Equipped��
	if (InID == INDEX_NONE)
	{
		
	}
	else
	{
		//��ȡPlayerState
		if (AHypermarketGameState * MyGS = GetWorld()->GetGameState<AHypermarketGameState>())
		{
			//��ȡHypermarketTable
			if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(InID))
			{
				if (SlotTable->WeaponType >= EWeaponType::AK47 &&
					SlotTable->WeaponType <= EWeaponType::SNIPER)
				{
					EquippedPrimary->UpdateSlot(SlotTable);
				}
				else if (SlotTable->WeaponType == EWeaponType::DESERTEAGLE)
				{
					EquippedSecondary->UpdateSlot(SlotTable);
				}

				//��������¼�Ѿ�װ��������ѧϰ
			}
		}
	}
}
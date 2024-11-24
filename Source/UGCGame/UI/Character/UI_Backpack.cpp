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
#include "../../Common/PlayerModule/PlayerModule.h"
#include "../../Common/ServerManage/ServerManage.h"

void UUI_Backpack::NativeConstruct()
{
	Super::NativeConstruct();

	CurMarkType = EHypermarkType::ALL;

	PMod = UPlayerModule::Get();
	if (PMod)
	{
		UpdateItemsHandle = PMod->OnItemsInfoDelegate.AddUObject(this, &UUI_Backpack::UpdateItem_Inner);
		UpdateEquipedHandle = PMod->OnPlayerInfoDelegate.AddUObject(this, &UUI_Backpack::UpdateEquippedSlots);
	}

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
	UpdateEquippedSlots();
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
	if (InType == EHypermarkType::DEFAULT) { InType = CurMarkType; }
	else { CurMarkType = InType; }

	if (ItemGrid->GetAllChildren().Num() > 0)
	{
		ItemGrid->ClearChildren();
	}

	//if (UPlayerSaveData * SaveMapData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerData_00"), 0)))
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
				if (PMod)
				{
					for (int32 i = 0; i < SlotsByType.Num(); i++)
					{
						TMap<int32, int32> ItemsData = PMod->GetItemsByMainClass(SlotsByType[i]->MainClass);
						//�Ƿ�Ϊӵ�е�
						if (ItemsData.Contains(SlotsByType[i]->ID))
						{
							//Ϊÿ����Ŀ����UI
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

									//����Item��Ϣ
									InWidgetItem->UpdateSlot(SlotsByType[i], false);
									//�󶨴���
									InWidgetItem->EquippedtDelegate.BindUObject(this, &UUI_Backpack::UpdateEquippedSlot);
								}
							}
						}
					}
					////�Ƿ�Ϊӵ�е� (��)
					//if (SaveMapData->ItemContains(SlotsByType[i]->ID))
					//{
					//	if (UUI_EquippedSlot * InWidgetItem = CreateWidget<UUI_EquippedSlot>(GetWorld(), ItemClass))
					//	{
					//		if (UGridSlot * GridSlot = ItemGrid->AddChildToGrid(InWidgetItem))
					//		{
					//			//Slot�Ų�
					//			GridSlot->SetRow(Index / 4);
					//			GridSlot->SetColumn(Index % 4);
					//			Index++;

					//			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					//			GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
					//			GridSlot->SetPadding(FMargin(20));


					//			//��������EquippedName
					//			if (InWidgetItem->EquippedName)
					//			{
					//				InWidgetItem->EquippedName->SetVisibility(ESlateVisibility::Hidden);
					//			}
					//			//����Item��Ϣ
					//			InWidgetItem->UpdateSlot(SlotsByType[i]);
					//			//�󶨴���
					//			InWidgetItem->EquippedtDelegate.BindUObject(this, &UUI_Backpack::UpdateEquippedSlot);
					//			//InWidgetItem->UpdateSaveEquippedDelegate.BindUObject(this, &UUI_Backpack::SaveEquippedItems);
					//		}
					//	}
					//}
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

void UUI_Backpack::UpdateItem_Inner()
{
	UpdateItem(CurMarkType);
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

				//��������¼�Ѿ�װ����������Ϣ
				SaveEquippedItems();
			}
		}
	}
}

void UUI_Backpack::UpdateEquippedSlots()
{
	if (AHypermarketGameState * MyGS = GetWorld()->GetGameState<AHypermarketGameState>())
	{
		if (PMod && PMod->EquippedItemIDs.Num() >= 4)
		{
			if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(PMod->EquippedItemIDs[0]))
			{
				EquippedPrimary->UpdateSlot(SlotTable);
			}

			if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(PMod->EquippedItemIDs[1]))
			{
				EquippedSecondary->UpdateSlot(SlotTable);
			}

			if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(PMod->EquippedItemIDs[2]))
			{
				EquippedCloseRange->UpdateSlot(SlotTable);
			}

			if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(PMod->EquippedItemIDs[3]))
			{
				EquippedGrenade->UpdateSlot(SlotTable);
			}
		}
	}
	/*(��)
	//��ȡPlayerState
	if (AHypermarketGameState * MyGS = GetWorld()->GetGameState<AHypermarketGameState>())
	{
		if (UPlayerSaveData * SaveItemsData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerData_00"), 0)))
		{
			

			if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(SaveItemsData->EquippedSecondary.ItemID))
			{
				EquippedSecondary->UpdateSlot(SlotTable);
			}

			if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(SaveItemsData->EquippedCloseRange.ItemID))
			{
				EquippedCloseRange->UpdateSlot(SlotTable);
			}

			if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(SaveItemsData->EquippedGrenade.ItemID))
			{
				EquippedGrenade->UpdateSlot(SlotTable);
			}
		}
	}*/
}

void UUI_Backpack::SaveEquippedItems()
{
	FSAVE_EQUIPPED_WEAPON_INFO_REQ InData;
	InData.ItemIDs.Add(EquippedPrimary->HyperTableID);
	InData.ItemIDs.Add(EquippedSecondary->HyperTableID);
	InData.ItemIDs.Add(EquippedCloseRange->HyperTableID);
	InData.ItemIDs.Add(EquippedGrenade->HyperTableID);

	FServerManage::Get()->Send<FSAVE_EQUIPPED_WEAPON_INFO_REQ>(SP_C2D_SAVE_EQUIPPED_WEAPON_INFO_REQ, &InData);

	/* ���ϣ�
	if (UPlayerSaveData * SaveItemsData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerData_00"), 0)))
	{
		SaveItemsData->EquippedPrimary = FItemInfo(EquippedPrimary->HyperTableID);
		SaveItemsData->EquippedSecondary = FItemInfo(EquippedSecondary->HyperTableID);
		SaveItemsData->EquippedCloseRange = FItemInfo(EquippedCloseRange->HyperTableID);
		SaveItemsData->EquippedGrenade = FItemInfo(EquippedGrenade->HyperTableID);
		UGameplayStatics::SaveGameToSlot(SaveItemsData, TEXT("PlayerData_00"), 0);
	}
	else if (UPlayerSaveData * SaveItems_Data = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass())))
	{
		SaveItems_Data->EquippedPrimary = FItemInfo(EquippedPrimary->HyperTableID);
		SaveItems_Data->EquippedSecondary = FItemInfo(EquippedSecondary->HyperTableID);
		SaveItems_Data->EquippedCloseRange = FItemInfo(EquippedCloseRange->HyperTableID);
		SaveItems_Data->EquippedGrenade = FItemInfo(EquippedGrenade->HyperTableID);
		UGameplayStatics::SaveGameToSlot(SaveItems_Data, TEXT("PlayerData_00"), 0);
	}

	if (UPlayerSaveData * SaveItemsData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerData_00"), 0)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Primary: %d"), SaveItemsData->EquippedPrimary.ItemID);
		UE_LOG(LogTemp, Warning, TEXT("Secondary: %d"), SaveItemsData->EquippedSecondary.ItemID);
		UE_LOG(LogTemp, Warning, TEXT("CloseRange: %d"), SaveItemsData->EquippedCloseRange.ItemID);
		UE_LOG(LogTemp, Warning, TEXT("Grenade: %d"), SaveItemsData->EquippedGrenade.ItemID);
	}*/
}

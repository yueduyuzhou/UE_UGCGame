// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Hypermarket.h"
#include "UI_Backpack.h"
#include "UI_HypermarketSlot.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "../../Table/HypermarketTable.h"
#include "../../Hypermarket/HypermarketGameState.h"
#include "../../Hypermarket/HypermarketHUD.h"
#include "UGCGame/SaveData/PlayerSaveData.h"
#include "Kismet/GameplayStatics.h"
#include "ThreadManage.h"
#include "UGCGame/Common/PlayerModule/PlayerModule.h"
#include "UGCGame/Common/ServerManage/ServerManage.h"

void UUI_Hypermarket::NativeConstruct()
{
	Super::NativeConstruct();

	All->OnCheckStateChanged.AddDynamic(this, &UUI_Hypermarket::CheckBoxAll);
	PrimaryWeapon->OnCheckStateChanged.AddDynamic(this, &UUI_Hypermarket::CheckBoxPrimaryWeapon);
	SecondaryWeapon->OnCheckStateChanged.AddDynamic(this, &UUI_Hypermarket::CheckBoxSecondaryWeapon);
	CloseRangeWeapon->OnCheckStateChanged.AddDynamic(this, &UUI_Hypermarket::CheckBoxCloseRangeWeapon);

	BuyButton->OnReleased.AddDynamic(this, &UUI_Hypermarket::OnBuyButtonClicked);
	CancelButton->OnReleased.AddDynamic(this, &UUI_Hypermarket::OnCancelButtonClicked);
	GoBackButton->OnReleased.AddDynamic(this, &UUI_Hypermarket::OnGoBackButtonClicked);
	BackpackButton->OnReleased.AddDynamic(this, &UUI_Hypermarket::OnBackpackButtonClicked);

	CheckBoxArray.Add(All);
	CheckBoxArray.Add(PrimaryWeapon);
	CheckBoxArray.Add(SecondaryWeapon);
	CheckBoxArray.Add(CloseRangeWeapon);

	PMod = UPlayerModule::Get();
	if (PMod)
	{
		UpdateGoldHandle = PMod->OnPlayerInfoDelegate.AddUObject(this, &UUI_Hypermarket::UpdateGold);
		UpdateItemsHandle = PMod->OnItemsInfoDelegate.AddUObject(this, &UUI_Hypermarket::UpdateItemImage);
	}


	UpdateItem(EHypermarkType::ALL);
	UpdateGold();
}

void UUI_Hypermarket::NativeDestruct()
{
	PMod->OnPlayerInfoDelegate.Remove(UpdateGoldHandle);
	PMod->OnItemsInfoDelegate.Remove(UpdateItemsHandle);

	Super::NativeDestruct();
}

void UUI_Hypermarket::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_Hypermarket::UpdateItem(EHypermarkType InType)
{
	if (ItemGrid->GetAllChildren().Num() > 0)
	{
		ItemGrid->ClearChildren();
	}
	
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

			//为每个条目创建UI
			for (int32 i = 0; i < SlotsByType.Num(); i++)
			{
				if (UUI_HypermarketSlot * InWidgetItem = CreateWidget<UUI_HypermarketSlot>(GetWorld(), ItemClass))
				{
					if (UUniformGridSlot * GridSlot = ItemGrid->AddChildToUniformGrid(InWidgetItem))
					{
						//Slot排布
						GridSlot->SetRow(FMath::FloorToInt((float)i / 2.f));

						if (i & 0x1) { GridSlot->SetColumn(1); }
						else { GridSlot->SetColumn(0); }

						GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
						GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

						//更新Item信息
						InWidgetItem->UpdateSlot(SlotsByType[i], false);
						//绑定点击代理
						InWidgetItem->HypermarketDelegate.BindUObject(this, &UUI_Hypermarket::UpdateItemImage);
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
				UpdateItem(EHypermarkType::ALL);
			});
	}	
}

void UUI_Hypermarket::UpdateItemImage(const int32& InID)
{
	if (InID == INDEX_NONE)
	{
		CurTableID = INDEX_NONE;
		ItemImage->SetBrush(FSlateBrush());
		BuyButton->SetIsEnabled(true);
	}
	else
	{
		if (AHypermarketGameState * MyGS = GetWorld()->GetGameState<AHypermarketGameState>())
		{
			//获取HypermarketTable
			if (FHypermarketTable * CurTable = MyGS->GetWeaponTableTemplate(InID))
			{
				TMap<int32, int32> ItemsData = PMod->GetItemsByMainClass(CurTable->MainClass);

				if (ItemsData.Contains(InID))
				{
					BuyButton->SetIsEnabled(false);
				}
				else
				{
					BuyButton->SetIsEnabled(true);
				}

				ItemImage->SetBrushFromTexture(CurTable->ItemIcon);
				CurTableID = CurTable->ID;
			}
		}

		//if (UPlayerSaveData * SaveMapData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerData_00"), 0)))
		//{
		//	
		//	if (SaveMapData->Items.Contains(InID))
		//	{
		//		BuyButton->SetIsEnabled(false);
		//	}
		//	else
		//	{
		//		BuyButton->SetIsEnabled(true);
		//	}
		//}
		////获取PlayerState
		//if (AHypermarketGameState * MyGS = GetWorld()->GetGameState<AHypermarketGameState>())
		//{
		//	//获取HypermarketTable
		//	if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(InID))
		//	{
		//		ItemImage->SetBrushFromTexture(SlotTable->ItemIcon);
		//		CurTableID = SlotTable->ID;
		//	}
		//}
	}
}

void UUI_Hypermarket::UpdateItemImage()
{
	UpdateItemImage(PreBuyItemID);
}

void UUI_Hypermarket::CheckBoxAll(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	All->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(EHypermarkType::ALL);
}

void UUI_Hypermarket::CheckBoxPrimaryWeapon(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	PrimaryWeapon->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(EHypermarkType::PRIMARYWEAPON);
}

void UUI_Hypermarket::CheckBoxSecondaryWeapon(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	SecondaryWeapon->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(EHypermarkType::SECONDARYWEAPON);
}

void UUI_Hypermarket::CheckBoxCloseRangeWeapon(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	CloseRangeWeapon->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(EHypermarkType::CLOSERANGEWEAPON);
}

void UUI_Hypermarket::OnBuyButtonClicked()
{
	//TODO：数据存储到DBServer
	//获取PlayerState
	if (AHypermarketGameState * MyGS = GetWorld()->GetGameState<AHypermarketGameState>())
	{
		//获取HypermarketTable
		if (FHypermarketTable * CurTable = MyGS->GetWeaponTableTemplate(CurTableID))
		{
			//if (UPlayerSaveData * SaveItemsData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerData_00"), 0)))
			//{
			//	if (SaveItemsData->Gold >= CurTable->ItemGold)
			//	{
			//		//发送TableID到
			//		SaveItemsData->Gold -= CurTable->ItemGold;
			//		SaveItemsData->Items.Add(CurTable->ID);
			//		UGameplayStatics::SaveGameToSlot(SaveItemsData, TEXT("PlayerData_00"), 0);
			//	}
			//}
			//else
			//{
			//	if (UPlayerSaveData * SaveItems_Data = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass())))
			//	{
			//		SaveItems_Data->Gold = 11999;
			//		SaveItems_Data->Gold -= CurTable->ItemGold;
			//		SaveItems_Data->Items.Add(CurTable->ID);
			//		UGameplayStatics::SaveGameToSlot(SaveItems_Data, TEXT("PlayerData_00"), 0);
			//	}
			//}
			//更新ItemImage状态
			//UpdateItemImage(CurTable->ID);
			//UpdateGold();

			if (PMod && PMod->Gold >= CurTable->ItemGold)
			{
				TMap<int32, int32> ItemsData = PMod->GetItemsByMainClass(CurTable->MainClass);
				int32 ItemCount = ItemsData.Contains(CurTable->ID) ? ItemsData[CurTable->ID] : 0;

				if (CurTable->MainClass != EItemMainClass::WEAPON || ItemCount < 1)
				{
					//发送购买请求
					FBUY_REQ args = FBUY_REQ(CurTable->ID);
					FServerManage::Get()->Send<FBUY_REQ>(SP_C2D_BUY_REQ, &args);
				}
			}
			PreBuyItemID = CurTable->ID;
			
		}
	}
}

void UUI_Hypermarket::OnCancelButtonClicked()
{
	UpdateItemImage(INDEX_NONE);
}

void UUI_Hypermarket::OnGoBackButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")), true);
}

void UUI_Hypermarket::OnBackpackButtonClicked()
{
	//显示背包
	if (APlayerController * MyPC = GetWorld()->GetFirstPlayerController<APlayerController>())
	{
		if (AHypermarketHUD * MyHyperHUD = MyPC->GetHUD<AHypermarketHUD>())
		{
			TArray<UWidget*> MainChilds = MyHyperHUD->GetMainScreenChildrens();
			for (auto* Tmp : MainChilds)
			{
				if (UUI_Backpack * BackpackUI = Cast<UUI_Backpack>(Tmp))
				{
					BackpackUI->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void UUI_Hypermarket::SetCheckBoxArray(ECheckBoxState CheckBoxState)
{
	for (auto& Tmp : CheckBoxArray)
	{
		Tmp->SetCheckedState(CheckBoxState);
	}
}

void UUI_Hypermarket::UpdateGold()
{
	if (PMod)
	{
		GoldText->SetText(FText::FromString(FString::FromInt(PMod->Gold)));
	}
}

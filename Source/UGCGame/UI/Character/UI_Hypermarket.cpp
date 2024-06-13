// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Hypermarket.h"
#include "UI_HypermarketSlot.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "../../Table/HypermarketTable.h"
#include "../../Hypermarket/HypermarketGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ThreadManage.h"

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

	CheckBoxArray.Add(All);
	CheckBoxArray.Add(PrimaryWeapon);
	CheckBoxArray.Add(SecondaryWeapon);
	CheckBoxArray.Add(CloseRangeWeapon);

	UpdateItem(EHypermarkType::ALL);
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
						InWidgetItem->UpdateSlot(SlotsByType[i]);
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
	//获取PlayerState
	if (AHypermarketGameState * MyGS = GetWorld()->GetGameState<AHypermarketGameState>())
	{
		//获取HypermarketTable
		if (FHypermarketTable * SlotTable = MyGS->GetWeaponTableTemplate(InID))
		{
			ItemImage->SetBrushFromTexture(SlotTable->ItemIcon);
			CurTable = SlotTable;
		}
	}
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
}

void UUI_Hypermarket::OnCancelButtonClicked()
{
}

void UUI_Hypermarket::OnGoBackButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")), true);
}

void UUI_Hypermarket::SetCheckBoxArray(ECheckBoxState CheckBoxState)
{
	for (auto& Tmp : CheckBoxArray)
	{
		Tmp->SetCheckedState(CheckBoxState);
	}
}



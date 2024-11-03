// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HypermarketSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI_Hypermarket.h"
#include "UGCGame/Hypermarket/HypermarketCharacterBase.h"

UUI_HypermarketSlot::UUI_HypermarketSlot()
	:HyperTableID(INDEX_NONE),
	HyperChracter(nullptr)
{

}

void UUI_HypermarketSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (AActor * Out_Actor = UGameplayStatics::GetActorOfClass(GetWorld(), AHypermarketCharacterBase::StaticClass()))
	{
		HyperChracter = Cast<AHypermarketCharacterBase>(Out_Actor);
		if (!HyperChracter)
		{
			UE_LOG(LogTemp, Error, TEXT("[class UUI_HypermarketSlot] : NativeConstruct, HyperChracter Is Null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_HypermarketSlot] : NativeConstruct, Out_Actor Is Null"));
	}
}

void UUI_HypermarketSlot::OnClickedWidget()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%d : Team Blue"), MyGameInstance->LocalPlayerData.PlayerID));
	if (HyperTableID != INDEX_NONE)
	{
		//附加到人物模型上
		HyperChracter->EquipItem(HyperTableID);
		HypermarketDelegate.ExecuteIfBound(HyperTableID);
	}
	//通知服务器购买
}

void UUI_HypermarketSlot::SetSlotName(const FString& InName)
{
	SlotName->SetText(FText::FromString(InName));
}

void UUI_HypermarketSlot::SetSlotGoldText(const int32& InValue, bool IsGold)
{
	if(IsGold)
		SlotGold->SetText(FText::FromString(FString::Printf(TEXT("$%d"), InValue)));
	else
		SlotGold->SetText(FText::FromString(FString::Printf(TEXT("%d"), InValue)));

	SlotGold->SetVisibility(ESlateVisibility::Visible);
}

void UUI_HypermarketSlot::SetEquippedName(const FString& InName)
{
	EquippedName->SetText(FText::FromString(InName));
}

void UUI_HypermarketSlot::UpdateSlot(const FHypermarketTable* InTable, bool IsShowEquipped)
{
	if (InTable)
	{
		SetSlotName(InTable->ItemName.ToString());
		SetSlotGoldText(InTable->ItemGold);
		SetIcon(InTable->ItemIcon);
		SetHyperTableID(InTable->ID);

		if (IsShowEquipped) { EquippedName->SetVisibility(ESlateVisibility::Visible); }
		else { EquippedName->SetVisibility(ESlateVisibility::Hidden); }
	}
}

void UUI_HypermarketSlot::UpdateSlot(const FItemTable* InTable, bool IsShowEquipped)
{
	if (InTable)
	{
		SetSlotName(InTable->ItemName.ToString());
		SetIcon(InTable->ItemIcon);
		SetHyperTableID(InTable->ID);

		SlotGold->SetVisibility(ESlateVisibility::Hidden);

		if (IsShowEquipped) { EquippedName->SetVisibility(ESlateVisibility::Visible); }
		else { EquippedName->SetVisibility(ESlateVisibility::Hidden); }
	}
}

void UUI_HypermarketSlot::SetHyperTableID(const int32& InID)
{
	HyperTableID = InID;
}

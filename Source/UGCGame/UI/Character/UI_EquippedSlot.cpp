// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquippedSlot.h"

UUI_EquippedSlot::UUI_EquippedSlot()
	:Super()
{

}

void UUI_EquippedSlot::OnClickedWidget()
{
	if (HyperTableID != INDEX_NONE)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Equip"));
		EquippedtDelegate.ExecuteIfBound(HyperTableID);
	}
}

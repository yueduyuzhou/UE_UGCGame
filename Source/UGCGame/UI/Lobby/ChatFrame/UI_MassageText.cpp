// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MassageText.h"
#include "Components/TextBlock.h"

void UUI_MassageText::SetPlayer(const FString& InPlayer)
{
	Player->SetText(FText::FromString(InPlayer));
}

void UUI_MassageText::SetMassage(const FString& InMsg)
{
	Massage->SetText(FText::FromString(InMsg));
}
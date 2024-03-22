// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MapSearchSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUI_MapSearchSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_MapSearchSlot::OnClickedWidget()
{
	JoinSessionBP();
}

void UUI_MapSearchSlot::SetInfo(FBlueprintSessionResult InSessionRes)
{
	SessionRes = InSessionRes;
	PingMs->SetText(FText::FromString(FString::FromInt(InSessionRes.OnlineResult.PingInMs)));
	MapName->SetText(FText::FromString(InSessionRes.OnlineResult.GetSessionIdStr()));
	
}
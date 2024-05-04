// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MapSearchSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../../UGCGameInstance.h"

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
	PingMs->SetText(FText::FromString(FString::FromInt(InSessionRes.OnlineResult.PingInMs) + TEXT("Ms")));
	MapName->SetText(FText::FromString(InSessionRes.OnlineResult.GetSessionIdStr()));
	
	int32 MaxPlayers = InSessionRes.OnlineResult.Session.SessionSettings.NumPublicConnections + InSessionRes.OnlineResult.Session.SessionSettings.NumPrivateConnections;
	int32 CurPlayers = MaxPlayers - InSessionRes.OnlineResult.Session.NumOpenPublicConnections;
	PlayerCount->SetText(
		FText::FromString(
			FString::FromInt(CurPlayers) + 
			TEXT("/") + 
			FString::FromInt(MaxPlayers))
	);
}
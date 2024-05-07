// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerListSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUI_PlayerListSlot::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerID = INDEX_NONE;

	PlayerButton->OnClicked.AddDynamic(this, &UUI_PlayerListSlot::PlayerButtonClick);
}

void UUI_PlayerListSlot::SetPlayerButtonEnable(const bool InIsEnable)
{
	PlayerButton->SetIsEnabled(InIsEnable);
}

void UUI_PlayerListSlot::SetButtonText(const FString& InButtonText)
{
	ButtonText->SetText(FText::FromString(InButtonText));
}

void UUI_PlayerListSlot::SetPlayerName(const FString& InPlayerName)
{
	PlayerName->SetText(FText::FromString(InPlayerName));
}

int32 UUI_PlayerListSlot::GetPlayerIDFromName()
{
	return FCString::Atoi(*PlayerName->GetText().ToString());
}

void UUI_PlayerListSlot::SetPlayerID(const int32& InPlayerID)
{
	PlayerID = InPlayerID;
}

ENetRole UUI_PlayerListSlot::GetLocalRole()
{
	if (APlayerController * MyPC = GetWorld()->GetFirstPlayerController())
	{
		return MyPC->GetLocalRole();
	}
	return ENetRole::ROLE_None;
}

void UUI_PlayerListSlot::PlayerButtonClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Call PlayerButtonClick"));
	//Server:Ìß³ö·¿¼ä
}

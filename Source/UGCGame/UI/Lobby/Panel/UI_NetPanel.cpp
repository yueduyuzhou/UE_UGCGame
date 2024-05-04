// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_NetPanel.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "UI_JoinPanel.h"
#include "UI_CreatePanel.h"
#include "NetSetting/UI_NetSetting.h"
#include "../UI_SearchPanel.h"

void UUI_NetPanel::NativeConstruct()
{
	Super::NativeConstruct();

	CreateGame->OnClicked.AddDynamic(this, &UUI_NetPanel::CreateGameButtonClick);
	JoinGame->OnClicked.AddDynamic(this, &UUI_NetPanel::JoinGameButtonClick);

	SwitchPanel(ENetPanelType::CREATE_GAME_PANEL);
}

void UUI_NetPanel::CreateGameButtonClick()
{
	SwitchPanel(ENetPanelType::CREATE_GAME_PANEL);
}

void UUI_NetPanel::JoinGameButtonClick()
{
	SwitchPanel(ENetPanelType::JOIN_GAME_PANEL);
}

void UUI_NetPanel::SwitchPanel(const ENetPanelType& InPanelType)
{
	if (InPanelType == ENetPanelType::CREATE_GAME_PANEL)
	{
		CreatePanel->SetVisibility(ESlateVisibility::Visible);
		JoinPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if(InPanelType == ENetPanelType::JOIN_GAME_PANEL)
	{
		CreatePanel->SetVisibility(ESlateVisibility::Collapsed);
		JoinPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

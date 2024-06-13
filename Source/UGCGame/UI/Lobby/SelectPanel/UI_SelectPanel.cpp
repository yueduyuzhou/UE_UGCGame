// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SelectPanel.h"
#include "Components/Widget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Lobby/LobbyHUD.h"


void UUI_SelectPanel::NativeConstruct()
{
	Super::NativeConstruct();

	HypermarketButton->OnReleased.AddDynamic(this, &UUI_SelectPanel::OnHypermarketButtonClicked);
	LobbyButton->OnReleased.AddDynamic(this, &UUI_SelectPanel::OnLobbyButtonClicked);
	CreativeWorkshopButton->OnReleased.AddDynamic(this, &UUI_SelectPanel::OnCreativeWorkshopButtonClicked);

	UpdateMainPanel(ELobbyPanelType::LOBBY_UIPANEL_LOBBY);
}

void UUI_SelectPanel::OnHypermarketButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("HypermarketMap")), true);
}

void UUI_SelectPanel::OnLobbyButtonClicked()
{
	UpdateMainPanel(ELobbyPanelType::LOBBY_UIPANEL_LOBBY);
}

void UUI_SelectPanel::OnCreativeWorkshopButtonClicked()
{
	UpdateMainPanel(ELobbyPanelType::LOBBY_UIPANEL_CREATIVEWORKSHOP);
}

void UUI_SelectPanel::UpdateMainPanel(ELobbyPanelType InType)
{
	if (ALobbyHUD * LHUD = GetWorld()->GetFirstPlayerController()->GetHUD<ALobbyHUD>())
	{
		TArray<UWidget*> Childrens = LHUD->GetMainScreenChildrens();
		for (auto* Tmp : Childrens)
		{
			if (UUI_Base * BasePanel = Cast<UUI_Base>(Tmp))
			{
				if (BasePanel->GetPanelType() == InType ||
					BasePanel->GetPanelType() == ELobbyPanelType::LOBBY_UIPANEL_PERMANENT)
				{
					BasePanel->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					BasePanel->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UGCGame/UI/Core/UI_Base.h"
#include "UI_SelectPanel.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_SelectPanel : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UButton* HypermarketButton;

	UPROPERTY(meta = (BindWidget))
		UButton* LobbyButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CreativeWorkshopButton;

	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
		void OnHypermarketButtonClicked();

	UFUNCTION()
		void OnLobbyButtonClicked();

	UFUNCTION()
		void OnCreativeWorkshopButtonClicked();
	
	UFUNCTION()
		void OnQuitButtonClicked();

	void UpdateMainPanel(ELobbyPanelType InType);
};

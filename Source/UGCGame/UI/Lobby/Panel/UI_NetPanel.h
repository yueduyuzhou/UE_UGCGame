// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_NetPanel.generated.h"

class UVerticalBox;
class UButton;
class UUI_CreatePanel;
class UUI_JoinPanel;

UENUM(BlueprintType)
enum class ENetPanelType : uint8
{
	CREATE_GAME_PANEL,
	JOIN_GAME_PANEL
};

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_NetPanel : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UButton* CreateGame;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinGame;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* Panel;

	UPROPERTY(meta = (BindWidget))
		UUI_CreatePanel* CreatePanel;

	UPROPERTY(meta = (BindWidget))
		UUI_JoinPanel* JoinPanel;

	UFUNCTION()
	void CreateGameButtonClick();
	UFUNCTION()
	void JoinGameButtonClick();

	void SwitchPanel(const ENetPanelType& InPanelType);

protected:
	virtual void NativeConstruct();


};

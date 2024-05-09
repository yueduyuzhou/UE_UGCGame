// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_PlayerListSlot.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_PlayerListSlot : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ButtonText;

	UPROPERTY(meta = (BindWidget))
		UButton* PlayerButton;

protected:
	virtual void NativeConstruct();

public:
	void SetPlayerButtonEnable(const bool InIsEnable);
	void SetButtonText(const FString& InButtonText);
	void SetPlayerName(const FString& InPlayerName);
	int32 GetPlayerIDFromName();

	void SetPlayerID(const int32& InPlayerID);
	int32 GetPlayerID();

	ENetRole GetLocalRole();

private:
	UFUNCTION()
		void PlayerButtonClick();

private:
	int32 PlayerID;
};

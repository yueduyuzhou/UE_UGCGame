// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UGCGame/Common/UGCGameType.h"
#include "UI_PlayerList.generated.h"

class UButton;
class UUI_PlayerListSlot;
class UUI_NetSetting;
class UVerticalBox;
enum class ETeamType : uint8;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_PlayerList : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UButton* StartGame;

	UPROPERTY(meta = (BindWidget))
		UButton* RedSelectButton;

	UPROPERTY(meta = (BindWidget))
		UButton* BlueSelectButton;

	UPROPERTY(meta = (BindWidget))
		UUI_NetSetting * Select_GameMap;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* RedPlayers;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* BluePlayers;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_PlayerListSlot> PlayerListSlotClass;

protected:
	virtual void NativeConstruct();

public:
	void AddNewPlayer(const ETeamType& InTeamType, const int32& InPlayerID);
	void RemovePlayer(const int32& InPlayerID);

	void UpdatePlayerList(const TArray<FPlayerNetData>& InPlayersDatas);
	void UpdateMapList();

	/***************************************************
	*
	****************************************************/
	UFUNCTION(BlueprintCallable)
	bool IsAuthority();

	UFUNCTION(BlueprintImplementableEvent)
	void QuitMap();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DestroySessionBP();

public:
	ENetRole GetLocalRole();
	int32 GetPlayerID();

private:
	UFUNCTION()
	void RedSelectButtonClick();
	UFUNCTION()
	void BlueSelectButtonClick();
	UFUNCTION()
	void StartGameButtonClick();
};

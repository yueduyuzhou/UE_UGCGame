// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyPlayersGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API ALobbyPlayersGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALobbyPlayersGameMode();

	/**********************************************************************
	*	Notify All Client Method
	**********************************************************************/
	void NotifyAllPlayerUpdateList();
	void NotifyAllPlayerAddMassage(const int32& InPlayerID, const FString& InMsg);

	UFUNCTION(BlueprintCallable)
	void NotifyAllClientQuit();
	
	/**********************************************************************
	*	Instance
	**********************************************************************/
	void AddPlayerDataInInstance(const FPlayerNetData& InPlayerData);
	void RemovePlayerDataInInstance(const FPlayerNetData& InPlayerData);
	void ClearAllPlayer();

	/**********************************************************************
	*	PlayerController
	**********************************************************************/
	ALobbyPlayerController* GetLocalPlayerController();

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Logout(AController* Exiting);
};

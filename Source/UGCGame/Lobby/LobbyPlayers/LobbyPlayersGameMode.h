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

	void NotifyAllPlayerUpdateList();

	void AddPlayerDataInInstance(const FPlayerNetData& InPlayerData);
	void RemovePlayerDataInInstance(const FPlayerNetData& InPlayerData);

protected:
	virtual void PostLogin(APlayerController* NewPlayer);

	virtual void Logout(AController* Exiting);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UGCGame/Common/UGCGameType.h"
#include "LobbyPlayerController.generated.h"

class UUI_PlayerList;

/**
 * 
 */
UCLASS()
class UGCGAME_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

	ALobbyPlayerController();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:
	UFUNCTION(client, reliable)
		void ServerCallClientUpdatePlayerList(const TArray<FPlayerNetData>& InPlayerDatas);

	UFUNCTION(server, reliable)
		void PlayerListChangeOnServer(const FPlayerNetData& InPlayerData);

public:
	void SetPlayerList(UUI_PlayerList* InPlayerList);

public:
	UPROPERTY(Replicated)
	int32 PlayerID;

private:
	UUI_PlayerList* PlayerList;
};

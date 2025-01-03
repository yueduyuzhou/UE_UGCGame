// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UGCGame/Common/UGCGameType.h"
#include "LobbyPlayerController.generated.h"

class UUI_PlayerList;
class UUI_ChatFrame;

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
	virtual void Tick(float DeltaSeconds) override;
public:
	UFUNCTION(client, reliable)
		void ServerCallClientUpdatePlayerList(const TArray<FPlayerNetData>& InPlayerDatas);

	UFUNCTION(client, reliable)
		void ServerCallClientQuit();

	UFUNCTION(client, reliable)
		void ServerCallClientUpdateLocalPlayerData(const FPlayerNetData& InPlayerData);

	UFUNCTION(server, reliable)
		void PlayerListChangeOnServer(const FPlayerNetData& InPlayerData);

	UFUNCTION(server, reliable)
		void AddMassageOnServer(const int32& InPlayerID, const FString& InMsg);

	UFUNCTION(client, reliable)
		void ServerCallClientAddMassage(const int32& InPlayerID, const FString& InMsg);

	UFUNCTION(client, reliable)
		void ServerCallClientSendPlayerInfo();

	UFUNCTION(server, reliable)
		void ServerReceivePlayerInfo(const FString& InNewPlayerID, const TArray<int32>& InEquipedWeapons);

public:
	void SetPlayerList(UUI_PlayerList* InPlayerList);
	void SetChatFrame(UUI_ChatFrame* InChatFrame);

	FORCEINLINE const int32& GetPlayerID() { return PlayerID; }

public:
	UPROPERTY(Replicated)
	int32 PlayerID;

private:
	UUI_PlayerList* PlayerList;
	UUI_ChatFrame* ChatFrame;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UGCGamePlayerState.generated.h"

class AElementBase;

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOneKeyMulticastDelegate, const TArray<int32>&);

UCLASS()
class UGCGAME_API AUGCGamePlayerState : public APlayerState
{
	GENERATED_BODY()

	friend class AUGCGameGameMode;

public:
	FOneKeyMulticastDelegate InitSlotDelegate;

private:

	virtual void BeginPlay() override;

public:
	/*获取表格数据*/
	void GetInventorySlotNetPackage(TArray<int32>& InKeys);

	/*RPC相关参数*/
public:
	UFUNCTION(Client, reliable)
		void ServerCallClientInitInventory(const TArray<int32>& InKeys);

	UFUNCTION(Client, reliable)
		void ServerCallClientInitPlayerData(const int32& InPlayerID);

	/*****************************************************************
	*	Element
	*****************************************************************/
	UFUNCTION(Server, reliable)
		void RequestSpawnElementOnServer(const int32& InPlayerID, const int32& InElementID);

	UFUNCTION(Server, unreliable)
		void UpdateElementLocationOnServer(const FVector& InMouseLocation, const FVector& InMouseDirection);

	UFUNCTION(Server, reliable)
		void TryReturnElementControlOnServer();

	UFUNCTION(Server, reliable)
		void TryGetElementControlOnServer(const FVector& InMouseLocation, const FVector& InMouseDirection);

public:


public:
	UPROPERTY(EditDefaultsOnly, Category = "Player Data")
		int32 TPlayerID;

	AElementBase* ControlElement;
};

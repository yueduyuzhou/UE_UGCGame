// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UGCGamePlayerState.generated.h"

class AElementBase;
enum class EElementModifyType : uint8;

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOneKeyMulticastDelegate, const TArray<int32>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FChangeModifyTypeMulticastDelegate, const ETransformationType&);

UCLASS()
class UGCGAME_API AUGCGamePlayerState : public APlayerState
{
	GENERATED_BODY()

	friend class AUGCGameGameMode;

public:
	FOneKeyMulticastDelegate InitSlotDelegate;
	FChangeModifyTypeMulticastDelegate ChangeModifyTypeDelegate;

private:
	virtual void BeginPlay() override;

public:
	AUGCGamePlayerState();

	/*获取表格数据*/
	void GetInventorySlotNetPackage(TArray<int32>& InKeys);

	/*RPC相关参数*/
public:
	UFUNCTION(Client, reliable)
		void ServerCallClientInitInventory(const TArray<int32>& InKeys);

	UFUNCTION(Client, reliable)
		void ServerCallClientInitPlayerData(const int32& InPlayerID);

	/*****************************************************************
	*	
	*****************************************************************/

	UFUNCTION(Server, reliable)
		void RequestSaveAndQuitOnServer();

	/*****************************************************************
	*	Element
	*****************************************************************/
	UFUNCTION(Server, reliable)
		void RequestSpawnElementOnServer(const int32& InPlayerID, const int32& InElementID);

	UFUNCTION(Server, unreliable)
		void UpdateElementLocationOnServer(const FVector& InMouseLocation, const FVector& InMouseDirection);

	UFUNCTION(Server, unreliable)
		void UpdateElementRotationOnServer(const float& InRotationX, const float& InRotationY);

	UFUNCTION(Server, reliable)
		void TryReturnElementControlOnServer();

	UFUNCTION(Server, reliable)
		void TryGetElementControlOnServer(const FVector& InMouseLocation, const FVector& InMouseDirection);

	UFUNCTION(Server, reliable)
		void RequestChangeElementModifyValueOnServer(const int32& InValue, const EElementModifyType& InModifyType);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:
	void SpawnElement(const int32& InPlayerID, const int32& InElementID);
	FVector SnapToGrid(const FVector& InOldPosition, const float& InGridSize);

	FRotator SnapToGridRotation(const FRotator& InOldRotation, const float& InAngleSize);

public:
	bool SaveMapName(const FString& InMapName);
	TArray<FString> GetMapList();

	FORCEINLINE const ETransformationType& GetModifyType() { return CurModifyType; }
	void SetModifyType(const ETransformationType& InModifyType);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Player Data")
		int32 TPlayerID;

	AElementBase* ControlElement;

private:
	int32 GridSize;
	int32 AngleSize;

	float RotationSpeed;

	UPROPERTY(Replicated)
		ETransformationType CurModifyType;
};

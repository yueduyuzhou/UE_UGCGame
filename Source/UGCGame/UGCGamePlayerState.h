#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RuntimeTransformer.h"
#include "UGCGame/Common/UGCGameType.h"
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
	friend class AUGCGameGameMode;

	GENERATED_BODY()

public:
	FChangeModifyTypeMulticastDelegate ChangeModifyTypeDelegate;

public:
	AUGCGamePlayerState();

	/*根据类型获取表格项ID*/
	void GetInventorySlotNetPackage(const ESlotType& InType, TArray<int32>& InKeys);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:
	/*****************************************************************
	*	Element
	*****************************************************************/
	void UpdateElementLocation(const FVector& InMouseLocation, const FVector& InMouseDirection);
	void UpdateElementRotation(const float& InRotationX, const float& InRotationY);

	void TryReturnElementControl();

	void SpawnElement(const int32& InPlayerID, const int32& InElementID);

	FVector SnapToGrid(const FVector& InOldPosition, const float& InGridSize);
	FRotator SnapToGridRotation(const FRotator& InOldRotation, const float& InAngleSize);

public:
	bool SaveMapName(const FString& InMapName);
	TArray<FString> GetMapList();

	FORCEINLINE const ETransformationType& GetModifyType() { return CurModifyType; }
	void SetModifyType(const ETransformationType& InModifyType);

	void InitPlayerData(const int32& InPlayerID);

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
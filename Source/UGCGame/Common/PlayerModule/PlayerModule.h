#pragma once

#include "CoreMinimal.h"
#include "UGCGame/Common/UGCGameType.h"
#include "PlayerModule.generated.h"

struct FLOGIN_REP;
struct FPLAYER_INFO_REP;
struct FITEM_INFO_REP;
//enum class EItemMainClass : uint8;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoOneDelegate, const FPLAYER_INFO_REP&);
DECLARE_MULTICAST_DELEGATE(FOnPlayerInfoDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemsInfoOneDelegate, const FITEM_INFO_REP&);
DECLARE_MULTICAST_DELEGATE(FOnItemsInfoDelegate);

UCLASS()
class UPlayerModule : public UObject
{
	GENERATED_BODY()

private:
	UPlayerModule();
	~UPlayerModule();

public:
	static UPlayerModule* Get();

	static void OnPlayerInfo_S(FPLAYER_INFO_REP InData);
	static void OnItemsInfo_S(FITEM_INFO_REP InData);

	void OnPlayerInfo(const FPLAYER_INFO_REP& InData);
	void OnItemsInfo(const FITEM_INFO_REP& InData);
	void InitPlayerInfo(const FLOGIN_REP& InData);

	const TMap<int32, int32>& GetItemsByMainClass(const EItemMainClass& InMC = EItemMainClass::DEFAULT);

private:
	void InitAllItemIDToMC();

private:
	static UPlayerModule* Ins;
	
	static FOnPlayerInfoOneDelegate OnPlayerInfoOneDelegate_S;
	static FOnItemsInfoOneDelegate OnItemsInfoOneDelegate_S;

public:
	/********************************代理******************************/
	FOnPlayerInfoOneDelegate OnPlayerInfoOneDelegate;
	FOnPlayerInfoDelegate OnPlayerInfoDelegate;
	
	FOnItemsInfoOneDelegate OnItemsInfoOneDelegate;
	FOnItemsInfoDelegate OnItemsInfoDelegate;

	/******************************基础信息****************************/
	FString Account;
	int32 Gold;
	/* PrimaryWeaponID SecondaryWeaponID CloseRangeWeaponID GrenadeWeaponID */
	TArray<int32> EquippedItemIDs;

	/******************************背包信息****************************/
	TMap<EItemMainClass, TMap<int32, int32>> MCToItemsData;
	TMap<int32, EItemMainClass> AllItemIDToMC;
	TMap<int32, int32> EmptyMap;
};
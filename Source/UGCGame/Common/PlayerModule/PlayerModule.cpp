#include "PlayerModule.h"
#include "../ServerManage/Macro/ServerType.h"
#include "../ServerManage/ServerManage.h"
#include "../../Hypermarket/HypermarketGameState.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

UPlayerModule * UPlayerModule::Ins = nullptr;

FOnPlayerInfoOneDelegate UPlayerModule::OnPlayerInfoOneDelegate_S;
FOnItemsInfoOneDelegate UPlayerModule::OnItemsInfoOneDelegate_S;

UPlayerModule::UPlayerModule()
	:Account("")
	, Gold(0)
{
	OnPlayerInfoOneDelegate_S.AddUObject(this, &UPlayerModule::OnPlayerInfo);
	OnItemsInfoOneDelegate_S.AddUObject(this, &UPlayerModule::OnItemsInfo);

	FServerManage::Get()->AddCallback<FPLAYER_INFO_REP>(SP_D2C_PLAYER_INFO_REP, UPlayerModule::OnPlayerInfo_S);
	FServerManage::Get()->AddCallback<FITEM_INFO_REP>(SP_D2C_ITEM_INFO_REP, UPlayerModule::OnItemsInfo_S);

	InitAllItemIDToMC();
}

UPlayerModule::~UPlayerModule()
{
	FServerManage::Get()->RemoveCallback<FPLAYER_INFO_REP>(SP_D2C_PLAYER_INFO_REP, UPlayerModule::OnPlayerInfo_S);
	if (Ins)
	{
		delete Ins;
		Ins = nullptr;
	}
}

UPlayerModule* UPlayerModule::Get()
{
	if (!Ins) 
	{
		Ins = NewObject<UPlayerModule>();
		Ins->AddToRoot();
		//Ins = new FPlayerModule(); 
	}
	return Ins;
}

void UPlayerModule::OnPlayerInfo_S(FPLAYER_INFO_REP InData)
{
	if (OnPlayerInfoOneDelegate_S.IsBound())
	{
		OnPlayerInfoOneDelegate_S.Broadcast(InData);
	}
}

void UPlayerModule::OnItemsInfo_S(FITEM_INFO_REP InData)
{
	if (OnItemsInfoOneDelegate_S.IsBound())
	{
		OnItemsInfoOneDelegate_S.Broadcast(InData);
	}
}

void UPlayerModule::OnPlayerInfo(const FPLAYER_INFO_REP& InData)
{
	Account = InData.Account;
	Gold = InData.Gold;
	EquippedItemIDs = InData.ItemIDs;

	if (OnPlayerInfoOneDelegate.IsBound())
	{
		OnPlayerInfoOneDelegate.Broadcast(InData);
	}

	if (OnPlayerInfoDelegate.IsBound())
	{
		OnPlayerInfoDelegate.Broadcast();
	}
}

void UPlayerModule::OnItemsInfo(const FITEM_INFO_REP& InData)
{
	int32 Len = InData.ItemIDs.Num();
	for (int32 i = 0; i < Len; i++)
	{
		int32 ItemID = InData.ItemIDs[i];
		int32 Count = InData.Counts[i];
		EItemMainClass MainClass = AllItemIDToMC.Contains(ItemID) ? AllItemIDToMC[ItemID] : EItemMainClass::DEFAULT;

		if (MCToItemsData.Contains(MainClass))
		{
			MCToItemsData[MainClass].Add(ItemID, Count);
		}
		else
		{
			TMap<int32, int32> ItemData;
			ItemData.Add(ItemID, Count);
			MCToItemsData.Add(MainClass, ItemData);
		}
	}

	if (OnItemsInfoOneDelegate.IsBound())
	{
		OnItemsInfoOneDelegate.Broadcast(InData);
	}

	if (OnItemsInfoDelegate.IsBound())
	{
		OnItemsInfoDelegate.Broadcast();
	}
}

void UPlayerModule::InitPlayerInfo(const FLOGIN_REP& InData)
{
	if (InData.IsSuccess == 1)
	{
		OnPlayerInfo(InData.PlayerInfo);
	}
}

const TMap<int32, int32>& UPlayerModule::GetItemsByMainClass(const EItemMainClass& InMC)
{
	//MCToItemsData[InMC];
	return  MCToItemsData.Contains(InMC) ? MCToItemsData[InMC] : EmptyMap;
}

void UPlayerModule::InitAllItemIDToMC()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Hypermarket_Table(TEXT("/Game/Table/HypermarketTable"));
	UDataTable* WeaponTablePtr = Hypermarket_Table.Object;
	TArray<FHypermarketTable*> CacheWeaponTables;

	if (WeaponTablePtr)
	{
		WeaponTablePtr->GetAllRows(TEXT("Weapon Table"), CacheWeaponTables);
		
		for (auto* Tmp : CacheWeaponTables)
		{
			AllItemIDToMC.Add(Tmp->ID, Tmp->MainClass);
		}
	}
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

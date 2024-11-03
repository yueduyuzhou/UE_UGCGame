#pragma once

#include "CoreMinimal.h"
#include "../../BaseModule.h"
#include "../../../ServerType.h"

class UElementMod : public UBaseModule
{
	UElementMod();
	~UElementMod();

public:
	static UElementMod* Get();

	/* 主处理接口 */
	//virtual void Processing(uint32 InProtocol) override;

public:
	FUGC_MAP_ELEMENT_INFO_RESPONSE C2D_UGC_MAP_ELEMENT_INFO_REQUEST(const FString& InGuid = "", const FUGC_MAP_ELEMENT_INFO_REQUEST& InMapID = FUGC_MAP_ELEMENT_INFO_REQUEST());
	FUGC_MAP_INFO_RESPONSE C2D_UGC_MAP_INFO_REQUEST(const FString& InGuid = "");
	FUGC_SAVE_MAP_INFO_REP C2D_UGC_SAVE_MAP_INFO_REQ(const FString& InGuid = "", const FUGC_SAVE_MAP_INFO_REQ& InData = FUGC_SAVE_MAP_INFO_REQ());
	FUGC_MAP_INFO_RESPONSE C2D_UGC_CREATE_MAP_REQ(const FString& InGuid = "", const FUGC_CREATE_MAP_REQ& InData = FUGC_CREATE_MAP_REQ());
	
	FLOGIN_REP C2D_LOGIN_REQ(const FString& InGuid = "", const FLOGIN_REQ& InData = FLOGIN_REQ());
	FQUIT_REP C2D_QUIT_REQ(const FString& InGuid = "");

	FPLAYER_INFO_REP C2D_PLAYER_INFO_REQ(const FString& InGuid = "", const FPLAYER_INFO_REQ& InData = FPLAYER_INFO_REQ());
	
	FITEM_INFO_REP C2D_ITEM_INFO_REQ(const FString& InGuid = "", const FITEM_INFO_REQ& InData = FITEM_INFO_REQ());
	void C2D_BUY_REQ(const FString& InGuid = "", const FBUY_REQ& InData = FBUY_REQ());

	void C2D_SAVE_EQUIPPED_WEAPON_INFO_REQ(const FString& InGuid = "", const FSAVE_EQUIPPED_WEAPON_INFO_REQ& InData = FSAVE_EQUIPPED_WEAPON_INFO_REQ());

	void C2D_SETTLEMENT_REWARD_REQ(const FString& InGuid = "", const FSETTLEMENT_REWARD_REQ& InData = FSETTLEMENT_REWARD_REQ());

public:
	void UpdateMapIDToName();
	void UpdateMapIDToNameBy(const FUGC_MAP_INFO_RESPONSE& InMapInfo);

	/* 获取玩家拥有金币 */
	int32 GetPlayerGold(const FString& InGuid);
	/* 获取物品价格 */
	int32 GetGoldByItemID(const int32& InItemID);
	/* 获取拥有物品数量 */
	int32 GetCountByItemID(const FString& InGuid, const int32& InItemID);
	/* 获取商品基础属性 */
	TArray<FSimpleMysqlResult> GetMarkDataByItemID(const int32& InItemID);

	/* 更新玩家拥有金币 */
	void UpdatePlayerGold(const FString& InGuid, const int32& InNewGold);

private:
	static UElementMod* Ins;

	//TODO:更新
	TMap<int32, FString> MapIDToName;

	TMap<FString, FString> AccountToGuid;
	TMap<FString, FString> GuidToAccount;
};
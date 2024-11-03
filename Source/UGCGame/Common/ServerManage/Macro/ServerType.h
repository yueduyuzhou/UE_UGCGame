#pragma once

#include "CoreMinimal.h"

/*****提前声明*****/

/* 201 */
struct FPLAYER_INFO_REP
{
	FPLAYER_INFO_REP() {}

	FString Account;
	int32 Gold;

	/* PrimaryWeaponID SecondaryWeaponID CloseRangeWeaponID GrenadeWeaponID */
	TArray<int32> ItemIDs;
};

/*****************/


struct FDATA_REQUEST
{
	FDATA_REQUEST() {}

	FDATA_REQUEST(FString InStr, int32 InNum)
		:TestStr(InStr)
		, TestNum(InNum)
	{}

	FString TestStr;
	int32 TestNum;
};

/* 100 */
struct FLOGIN_REQ
{
	FLOGIN_REQ() {}

	FLOGIN_REQ(FString InAccount, FString InPassword)
		:Account(InAccount)
		, Password(InPassword)
	{}

	FString Account;
	FString Password;
};

/* 101 */
struct FLOGIN_REP
{
	FLOGIN_REP() {}

	FLOGIN_REP(int32 InIsSuccess)
		:IsSuccess(InIsSuccess)
	{}

	/* 0失败 1成功 */
	int32 IsSuccess;

	/* Player Info */
	FPLAYER_INFO_REP PlayerInfo;
};

/* 102 */
struct FQUIT_REQ
{
	FQUIT_REQ() {}
};

/* 103 */
struct FQUIT_REP
{
	FQUIT_REP() {}

	FQUIT_REP(int32 InIsSuccess)
		:IsSuccess(InIsSuccess)
	{}

	/* 0失败 1成功 */
	int32 IsSuccess;
};

/* 150 */
struct FUGC_MAP_ELEMENT_INFO_REQUEST
{
	FUGC_MAP_ELEMENT_INFO_REQUEST() {}

	FUGC_MAP_ELEMENT_INFO_REQUEST(int32 InMapID)
		:MapID(InMapID)
	{}

	int32 MapID;
};

/* 151 */
struct FUGC_MAP_ELEMENT_INFO_RESPONSE
{
	FUGC_MAP_ELEMENT_INFO_RESPONSE() {}

	int32 MapID;
	TArray<int32> IntData;
	TArray<float> FloatData;
};

/* 152 */
struct FUGC_MAP_INFO_REQUEST
{
	FUGC_MAP_INFO_REQUEST() {}

};

/* 153 */
struct FUGC_MAP_INFO_RESPONSE
{
	FUGC_MAP_INFO_RESPONSE() {}

	TArray<FString> MapNames;
	TArray<int32> MapIDs;
};

/* 154 */
struct FUGC_SAVE_MAP_INFO_REQ
{
	FUGC_SAVE_MAP_INFO_REQ() {}

	int32 MapID;
	FString MapName;
	TArray<int32> IntData;
	TArray<float> FloatData;
};

/* 155 */
struct FUGC_SAVE_MAP_INFO_REP
{
	FUGC_SAVE_MAP_INFO_REP() {}
};

/* 156 */
struct FUGC_CREATE_MAP_REQ
{
	FUGC_CREATE_MAP_REQ() {}
	FUGC_CREATE_MAP_REQ(TArray<FString> InMapNames)
		:MapNames(InMapNames)
	{}

	TArray<FString> MapNames;
};

/* 157 */
struct FUGC_REMOVE_MAP_REP
{
	FUGC_REMOVE_MAP_REP() {}

};

/* 200 */
struct FPLAYER_INFO_REQ
{
	FPLAYER_INFO_REQ() {}

};

/* 250 */
struct FITEM_INFO_REQ
{
	FITEM_INFO_REQ() {}

};

/* 251 */
struct FITEM_INFO_REP
{
	FITEM_INFO_REP() {}

	TArray<int32> ItemIDs;
	TArray<int32> Counts;
};

/* 252 */
struct FBUY_REQ
{
	FBUY_REQ() {}
	FBUY_REQ(int32 InItemID)
		:ItemID(InItemID)
	{}

	int32 ItemID;
};

/* 300 */
struct FSAVE_EQUIPPED_WEAPON_INFO_REQ
{
	FSAVE_EQUIPPED_WEAPON_INFO_REQ() {}

	/* PrimaryWeaponID SecondaryWeaponID CloseRangeWeaponID GrenadeWeaponID */
	TArray<int32> ItemIDs;
};

/* 350 */
struct FSETTLEMENT_REWARD_REQ
{
	FSETTLEMENT_REWARD_REQ() {}

	FSETTLEMENT_REWARD_REQ(const TArray<int32>& InItems_ID, const TArray<int32>& InItems_Count)
		:Items_ID(InItems_ID)
		, Items_Count(InItems_Count)
	{}

	TArray<int32> Items_ID;
	TArray<int32> Items_Count;
};
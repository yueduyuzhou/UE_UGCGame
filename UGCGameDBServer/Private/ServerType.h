#pragma once

#include "CoreMinimal.h"

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
struct FUGC_REMOVE_MAP_REQ
{
	FUGC_REMOVE_MAP_REQ() {}


};
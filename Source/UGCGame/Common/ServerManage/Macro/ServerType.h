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

struct FUGC_MAP_ELEMENT_INFO_REQUEST
{
	FUGC_MAP_ELEMENT_INFO_REQUEST() {}

	FUGC_MAP_ELEMENT_INFO_REQUEST(int32 InMapID)
		:MapID(InMapID)
	{}

	int32 MapID;
};

struct FUGC_MAP_ELEMENT_INFO_RESPONSE
{
	FUGC_MAP_ELEMENT_INFO_RESPONSE() {}

	int32 MapID;
	TArray<int32> IntData;
	TArray<float> FloatData;
};

struct FUGC_MAP_INFO_REQUEST
{
	FUGC_MAP_INFO_REQUEST() {}

};

struct FUGC_MAP_INFO_RESPONSE
{
	FUGC_MAP_INFO_RESPONSE() {}

	TArray<FString> MapNames;
	TArray<int32> MapIDs;
};

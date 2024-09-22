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
	FUGC_MAP_ELEMENT_INFO_RESPONSE C2D_UGC_MAP_ELEMENT_INFO_REQUEST(int32& InMapID);
	FUGC_MAP_INFO_RESPONSE C2D_UGC_MAP_INFO_REQUEST();
	FUGC_SAVE_MAP_INFO_REP C2D_UGC_SAVE_MAP_INFO_REQ(const FUGC_SAVE_MAP_INFO_REQ& InData);

	void UpdateMapIDToName();

private:
	static UElementMod* Ins;

	//TODO:更新
	TMap<int32, FString> MapIDToName;
};
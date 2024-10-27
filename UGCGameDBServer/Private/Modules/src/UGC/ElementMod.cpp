#include "ElementMod.h"

UElementMod* UElementMod::Ins = nullptr;

UElementMod::UElementMod()
{
	UpdateMapIDToName();
}

UElementMod::~UElementMod()
{
}

UElementMod* UElementMod::Get()
{
	if (!Ins) { Ins = new UElementMod(); }
	return Ins;
}

//void UElementMod::Processing(uint32 InProtocol)
//{
//	switch (InProtocol)
//	{
//		case SP_C2D_UGC_ELEMENT_REQUEST:
//		{
//			C2D_UGC_ELEMENT_REQUEST();
//			break;
//		}
//	}
//}

FUGC_MAP_ELEMENT_INFO_RESPONSE UElementMod::C2D_UGC_MAP_ELEMENT_INFO_REQUEST(const FString& InGuid, const FUGC_MAP_ELEMENT_INFO_REQUEST& InData)
{
	FUGC_MAP_ELEMENT_INFO_RESPONSE OutData;

	//数据库的地图表中查询对应的数据ID为MapID
	FString Sql;
	Sql = FString::Printf(TEXT("Select* FROM mapelementinfo where MapID = %d;"), InData.MapID);
	
	TArray<FSimpleMysqlResult> Res = FMysqlConfig::Get()->QueryBySql(Sql);

	OutData.MapID = InData.MapID;
	for (auto& Tmp : Res)
	{
		for (auto& Row : Tmp.Rows)
		{
			FString KeyStr = Row.Key;
			if(KeyStr == "ElementID" || KeyStr == "TeamType")
			{
				OutData.IntData.Add(FCString::Atoi(*Row.Value));
			}
			else if (KeyStr == "LocationX" || KeyStr == "LocationY" || KeyStr == "LocationZ" 
				  || KeyStr == "RotationPitch" || KeyStr == "RotationYaw" || KeyStr == "RotationRoll" 
				  || KeyStr == "ScaleX" || KeyStr == "ScaleY" || KeyStr == "ScaleZ"
				  || KeyStr == "ColorR" || KeyStr == "ColorG" || KeyStr == "ColorB" || KeyStr == "ColorA")
			{
				OutData.FloatData.Add(FCString::Atof(*Row.Value));
			}
		}
	}

	return OutData;
}

FUGC_MAP_INFO_RESPONSE UElementMod::C2D_UGC_MAP_INFO_REQUEST(const FString& InGuid)
{
	FUGC_MAP_INFO_RESPONSE OutData;

	FString Sql, Cond;
	if (InGuid == "") { Sql = FString::Printf(TEXT("Select* FROM mapinfo")); }
	else 
	{
		Cond = GuidToAccount.Contains(InGuid) ? "where Account = " + GuidToAccount[InGuid] : "";
		Sql = FString::Printf(TEXT("Select* FROM mapinfo %s"), *Cond);
	}

	TArray<FSimpleMysqlResult> Res = FMysqlConfig::Get()->QueryBySql(Sql);
	for (auto& Tmp : Res)
	{
		for (auto& Row : Tmp.Rows)
		{
			FString KeyStr = Row.Key;
			if (KeyStr == "MapID")
			{
				OutData.MapIDs.Add(FCString::Atoi(*Row.Value));
			}
			else if (KeyStr == "MapName")
			{
				OutData.MapNames.Add(Row.Value);
			}
		}
	}

	return OutData;
}

FUGC_SAVE_MAP_INFO_REP UElementMod::C2D_UGC_SAVE_MAP_INFO_REQ(const FString& InGuid, const FUGC_SAVE_MAP_INFO_REQ& InData)
{
	FUGC_SAVE_MAP_INFO_REP OutData;
	TArray<TMap<FString, FString>> RepDatas;
	TArray<FSimpleMysqlComparisonOperator> ClearCond;

	FString MapIDStr;

	if (InData.MapID == INDEX_NONE)
	{
		FUGC_CREATE_MAP_REQ Tmp;
		Tmp.MapNames.Add(InData.MapName);
		FUGC_MAP_INFO_RESPONSE MapInfo = C2D_UGC_CREATE_MAP_REQ(InGuid, Tmp);
		int32 Len = MapInfo.MapNames.Num();
		for (int32 i = 0; i < Len; i++)
		{
			if (MapInfo.MapNames[i] == InData.MapName)
			{
				MapIDStr = FString::FromInt(MapInfo.MapIDs[i]);
			}
		}
	}
	else
	{
		MapIDStr = FString::FromInt(InData.MapID);
	}

	//设置清除条件
	FSimpleMysqlComparisonOperator OP;
	OP.Assignment.A = "MapID";
	OP.Assignment.B = MapIDStr;
	OP.Assignment.ComparisonOperator = EMysqlComparisonOperator::EQUAL;
	ClearCond.Add(OP);

	//整理数据
	int32 IntIdx = 0, FloatIdx = 0;
	int32 IntLen = InData.IntData.Num();
	int32 FloatLen = InData.FloatData.Num();
	TArray<FString> DataFarmat = TArray<FString>(
		{ "MapID",
		"ElementID",
		"TeamType",
		"LocationX",
		"LocationY",
		"LocationZ",
		"RotationPitch",
		"RotationYaw",
		"RotationRoll",
		"ScaleX",
		"ScaleY",
		"ScaleZ",
		"ColorR",
		"ColorG",
		"ColorB",
		"ColorA" });

	for (; IntIdx < IntLen && FloatIdx < FloatLen; IntIdx += 2, FloatIdx += 13)
	{
		TMap<FString, FString> Tmp;
		Tmp.Add("MapID", MapIDStr);
		Tmp.Add("ElementID", FString::FromInt(InData.IntData[IntIdx]));
		Tmp.Add("TeamType", FString::FromInt(InData.IntData[IntIdx + 1]));

		for (int32 j = 3; j < DataFarmat.Num(); j++)
		{
			Tmp.Add(DataFarmat[j], FString::SanitizeFloat(InData.FloatData[FloatIdx + j - 3]));
		}
		/*Tmp["LocationX"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["LocationY"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["LocationZ"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["RotationPitch"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["RotationYaw"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["RotationRoll"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["ScaleX"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["ScaleY"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["ScaleZ"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["ColorR"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["ColorG"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["ColorB"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);
		Tmp["ColorA"] = FString::SanitizeFloat(InData.FloatData[FloatIdx]);*/
		RepDatas.Add(Tmp);
	}

	FMysqlConfig::Get()->ReplaceTableDatas("mapelementinfo", RepDatas, true, ClearCond);

	return OutData;
}

FUGC_MAP_INFO_RESPONSE UElementMod::C2D_UGC_CREATE_MAP_REQ(const FString& InGuid, const FUGC_CREATE_MAP_REQ& InData)
{
	FUGC_MAP_INFO_RESPONSE OutData;

	TArray<TMap<FString, FString>> RepDatas;
	int32 Len = InData.MapNames.Num();
	FString Account;

	if (!InGuid.IsEmpty())
	{
		Account = GuidToAccount.Contains(InGuid) ? GuidToAccount[InGuid] : "";
	}

	for (int i = 0; i < Len; i++)
	{
		TMap<FString, FString> Tmp;
		Tmp.Add("MapName", InData.MapNames[i]);
		Tmp.Add("id", FString::FromInt(1111));
		Tmp.Add("Account", Account);
		RepDatas.Add(Tmp);
	}

	FMysqlConfig::Get()->InsertTableDatas("mapinfo", RepDatas);

	FUGC_MAP_INFO_RESPONSE MapInfo = C2D_UGC_MAP_INFO_REQUEST(InGuid);

	UpdateMapIDToNameBy(MapInfo);
	return MapInfo;
}

FLOGIN_REP UElementMod::C2D_LOGIN_REQ(const FString& InGuid, const FLOGIN_REQ& InData)
{
	FLOGIN_REP OutData = FLOGIN_REP(0);

	OutData.PlayerInfo.Account = InData.Account;

	if (AccountToGuid.Contains(InData.Account))
	{
		//已经登录过
		OutData.IsSuccess = 0;
	}
	else
	{
		AccountToGuid.Add(InData.Account, InGuid);
		GuidToAccount.Add(InGuid, InData.Account);

		//查询账号
		FString Sql = FString::Printf(TEXT("Select* FROM playerinfobase where Account = %s;"), *InData.Account);
		TArray<FSimpleMysqlResult> Res = FMysqlConfig::Get()->QueryBySql(Sql);

		for (auto& Tmp : Res)
		{
			for (auto& Row : Tmp.Rows)
			{
				if (Row.Key == "Password" && Row.Value == InData.Password) { OutData.IsSuccess = 1; }
			}
		}

		if (OutData.IsSuccess == 1)
		{
			OutData.PlayerInfo = C2D_PLAYER_INFO_REQ(InGuid);
		}
	}

	return OutData;
}

FQUIT_REP UElementMod::C2D_QUIT_REQ(const FString& InGuid)
{
	FQUIT_REP OutData;

	if (GuidToAccount.Contains(InGuid))
	{
		AccountToGuid.Remove(GuidToAccount[InGuid]);
		GuidToAccount.Remove(InGuid);
		OutData.IsSuccess = 1;
	}
	else { OutData.IsSuccess = 0; }

	return OutData;
}

FPLAYER_INFO_REP UElementMod::C2D_PLAYER_INFO_REQ(const FString& InGuid, const FPLAYER_INFO_REQ& InData)
{
	FPLAYER_INFO_REP OutData;

	//查询信息
	OutData.Account = GuidToAccount.Contains(InGuid) ? GuidToAccount[InGuid] : "";
	FString Sql = FString::Printf(TEXT("Select* FROM playerinfo where Account = %s;"), *(OutData.Account));
	TArray<FSimpleMysqlResult> Res = FMysqlConfig::Get()->QueryBySql(Sql);

	TMap<FString, int32> DataFarmat;
	DataFarmat.Add("EquippedPrimaryID", 0);
	DataFarmat.Add("EquippedSecondaryID", 1);
	DataFarmat.Add("EquippedCloseRangeID", 2);
	DataFarmat.Add("EquippedGrenadeID", 3);
	OutData.ItemIDs = TArray<int32>({ -1, -1, -1, -1 });
	for (auto& Tmp : Res)
	{
		for (auto& Row : Tmp.Rows)
		{
			//if (Row.Key == "Gold") { OutData.Gold = FCString::Atoi(*(Row.Value)); }
			if (Row.Key == "EquippedPrimaryID" 
				|| Row.Key == "EquippedSecondaryID" 
				|| Row.Key == "EquippedCloseRangeID" 
				|| Row.Key == "EquippedGrenadeID")
			{
				OutData.ItemIDs[DataFarmat[Row.Key]] = FCString::Atoi(*(Row.Value));
			}
		}
	}

	OutData.Gold = GetPlayerGold(InGuid);
	return OutData;
}

FITEM_INFO_REP UElementMod::C2D_ITEM_INFO_REQ(const FString& InGuid, const FITEM_INFO_REQ& InData)
{
	FITEM_INFO_REP OutData;

	FString Account = GuidToAccount.Contains(InGuid) ? GuidToAccount[InGuid] : "";
	FString Sql = FString::Printf(TEXT("Select* FROM playeritemsinfo where Account = %s"), *Account);

	TArray<FSimpleMysqlResult> Res = FMysqlConfig::Get()->QueryBySql(Sql);

	for (auto& Tmp : Res)
	{
		for (auto& row : Tmp.Rows)
		{
			if (row.Key == "ItemID") { OutData.ItemIDs.Add(FCString::Atoi(*row.Value)); }
			else if (row.Key == "Count") { OutData.Counts.Add(FCString::Atoi(*row.Value)); }
		}
	}

	return OutData;
}

void UElementMod::C2D_BUY_REQ(const FString& InGuid, const FBUY_REQ& InData)
{
	int32 Gold = GetPlayerGold(InGuid);
	int32 Cost = GetGoldByItemID(InData.ItemID);

	if (Gold >= Cost)
	{
		int32 Count = GetCountByItemID(InGuid, InData.ItemID);

		//更新金币
		UpdatePlayerGold(InGuid, Gold - Cost);

		TArray<FSimpleMysqlComparisonOperator> ClearCond;
		FSimpleMysqlComparisonOperator OP;

		//设置清除条件
		OP.Assignment.A = "ItemID";
		OP.Assignment.B = FString::FromInt(InData.ItemID);
		OP.Assignment.ComparisonOperator = EMysqlComparisonOperator::EQUAL;
		ClearCond.Add(OP);

		//更新物品数
		TArray<TMap<FString, FString>> RepDatas;
		FString Account = GuidToAccount.Contains(InGuid) ? GuidToAccount[InGuid] : "";
		TMap<FString, FString> Tmp;
		Tmp.Add("Account", Account);
		Tmp.Add("ItemID", FString::FromInt(InData.ItemID));
		Tmp.Add("Count", FString::FromInt(Count + 1));
		RepDatas.Add(Tmp);

		FMysqlConfig::Get()->ReplaceTableDatas("playeritemsinfo", RepDatas, true, ClearCond);
	}

	return;
}

void UElementMod::C2D_SAVE_EQUIPPED_WEAPON_INFO_REQ(const FString& InGuid, const FSAVE_EQUIPPED_WEAPON_INFO_REQ& InData)
{
	TArray<TMap<FString, FString>> RepDatas;
	FString Account = GuidToAccount.Contains(InGuid) ? GuidToAccount[InGuid] : "";
	TArray<FSimpleMysqlComparisonOperator> ClearCond;
	FSimpleMysqlComparisonOperator OP;

	/*FString Sql = FString::Printf(TEXT("Select* FROM playerinfo where Account = %s;"), *Account);
	TArray<FSimpleMysqlResult> Res = FMysqlConfig::Get()->QueryBySql(Sql);
	FString Gold = "";

	for (auto& Tmp : Res)
	{
		for (auto& Row : Tmp.Rows)
		{
			if (Row.Key == "Gold") { Gold = Row.Value; }
		}
	}*/

	//设置清除条件
	OP.Assignment.A = "Account";
	OP.Assignment.B = Account;
	OP.Assignment.ComparisonOperator = EMysqlComparisonOperator::EQUAL;
	ClearCond.Add(OP);

	//获取武器ID字符串
	auto GetIDStr = [&](int32 InIndex) ->FString
	{
		int32 ID = InData.ItemIDs.Num() > InIndex ? InData.ItemIDs[InIndex] : INDEX_NONE;
		return FString::FromInt(ID);
	};

	//设置新数据
	TMap<FString, FString> Tmp;
	Tmp.Add("Account", Account);
	//Tmp.Add("Gold", Gold);
	Tmp.Add("EquippedPrimaryID", GetIDStr(0));
	Tmp.Add("EquippedSecondaryID", GetIDStr(1));
	Tmp.Add("EquippedCloseRangeID", GetIDStr(2));
	Tmp.Add("EquippedGrenadeID", GetIDStr(3));
	RepDatas.Add(Tmp);

	FMysqlConfig::Get()->ReplaceTableDatas("playerinfo", RepDatas, true, ClearCond);
}

void UElementMod::UpdateMapIDToName()
{
	FUGC_MAP_INFO_RESPONSE MapInfo = C2D_UGC_MAP_INFO_REQUEST();

	UpdateMapIDToNameBy(MapInfo);
}

void UElementMod::UpdateMapIDToNameBy(const FUGC_MAP_INFO_RESPONSE& InMapInfo)
{
	MapIDToName.Empty();

	int32 Len = InMapInfo.MapIDs.Num();
	for (int32 i = 0; i < Len; i++)
	{
		MapIDToName.Add(InMapInfo.MapIDs[i], InMapInfo.MapNames[i]);
	}
}

int32 UElementMod::GetPlayerGold(const FString& InGuid)
{
	FString Account = GuidToAccount.Contains(InGuid) ? GuidToAccount[InGuid] : "";
	FString Sql = FString::Printf(TEXT("Select* FROM playeritemsinfo where Account = %s and ItemID = 1;"), *Account);
	TArray<FSimpleMysqlResult> Res = FMysqlConfig::Get()->QueryBySql(Sql);

	for (auto& Tmp : Res)
	{
		for (auto& Row : Tmp.Rows)
		{
			if (Row.Key == "Count") { return FCString::Atoi(*(Row.Value)); }
		}
	}

	return 0;
}

int32 UElementMod::GetGoldByItemID(const int32& InItemID)
{
	TArray<FSimpleMysqlResult> Res = GetMarkDataByItemID(InItemID);

	for (auto& Tmp : Res)
	{
		for (auto& Row : Tmp.Rows)
		{
			if (Row.Key == "ItemGold") { return FCString::Atoi(*(Row.Value)); }
		}
	}

	return 0;
}

int32 UElementMod::GetCountByItemID(const FString& InGuid, const int32& InItemID)
{
	FString Account = GuidToAccount.Contains(InGuid) ? GuidToAccount[InGuid] : "";
	FString Sql = FString::Printf(TEXT("Select* FROM playeritemsinfo where Account = %s and ItemID = %d;"), *Account, InItemID);
	TArray<FSimpleMysqlResult> Res = FMysqlConfig::Get()->QueryBySql(Sql);

	for (auto& Tmp : Res)
	{
		for (auto& Row : Tmp.Rows)
		{
			if (Row.Key == "Count") { return FCString::Atoi(*(Row.Value)); }
		}
	}
	return 0;
}

TArray<FSimpleMysqlResult> UElementMod::GetMarkDataByItemID(const int32& InItemID)
{
	FString Sql = FString::Printf(TEXT("Select* FROM hypermarkettable where ID = %d;"), InItemID);
	return FMysqlConfig::Get()->QueryBySql(Sql);
}

void UElementMod::UpdatePlayerGold(const FString& InGuid, const int32& InNewGold)
{
	TArray<TMap<FString, FString>> RepDatas;
	FString Account = GuidToAccount.Contains(InGuid) ? GuidToAccount[InGuid] : "";
	TArray<FSimpleMysqlComparisonOperator> ClearCond;
	FSimpleMysqlComparisonOperator OP, OP2;

	//设置清除条件
	OP.Assignment.A = "Account";
	OP.Assignment.B = Account;
	OP2.Assignment.A = "ItemID";
	OP2.Assignment.B = "1";
	OP.Assignment.ComparisonOperator = OP2.Assignment.ComparisonOperator = EMysqlComparisonOperator::EQUAL;
	ClearCond.Add(OP);
	ClearCond.Add(OP2);

	//设置新数据
	TMap<FString, FString> Tmp;
	Tmp.Add("Account", Account);
	Tmp.Add("ItemID", "1");
	Tmp.Add("Gold", FString::FromInt(InNewGold));
	RepDatas.Add(Tmp);

	FMysqlConfig::Get()->ReplaceTableDatas("playeritemsinfo", RepDatas, true, ClearCond);
}
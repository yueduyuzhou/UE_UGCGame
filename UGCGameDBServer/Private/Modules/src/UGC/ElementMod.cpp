#include "ElementMod.h"

UElementMod* UElementMod::Ins = nullptr;

UElementMod::UElementMod()
{
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

FUGC_MAP_ELEMENT_INFO_RESPONSE UElementMod::C2D_UGC_MAP_ELEMENT_INFO_REQUEST(int32& InMapID)
{
	FUGC_MAP_ELEMENT_INFO_RESPONSE OutData;

	//数据库的地图表中查询对应的数据ID为MapID
	FString Sql = FString::Printf(TEXT("Select* FROM mapelementinfo where MapID = %d;"), InMapID);
	TArray<FSimpleMysqlResult> Res = FMysqlConfig::Get()->QueryBySql(Sql);

	OutData.MapID = InMapID;
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

FUGC_MAP_INFO_RESPONSE UElementMod::C2D_UGC_MAP_INFO_REQUEST()
{
	FUGC_MAP_INFO_RESPONSE OutData;

	FString Sql = FString::Printf(TEXT("Select* FROM mapinfo"));
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

FUGC_SAVE_MAP_INFO_REP UElementMod::C2D_UGC_SAVE_MAP_INFO_REQ(const FUGC_SAVE_MAP_INFO_REQ& InData)
{
	FUGC_SAVE_MAP_INFO_REP OutData;
	TArray<TMap<FString, FString>> RepDatas;
	TArray<FSimpleMysqlComparisonOperator> ClearCond;
	FString MapIDStr = FString::FromInt(InData.MapID);
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

void UElementMod::UpdateMapIDToName()
{
	FUGC_MAP_INFO_RESPONSE MapInfo = C2D_UGC_MAP_INFO_REQUEST();

	MapIDToName.Empty();

	int32 Len = MapInfo.MapIDs.Num();
	for (int32 i = 0; i < Len; i++)
	{
		MapIDToName.Add(MapInfo.MapIDs[i], MapInfo.MapNames[i]);
	}
}

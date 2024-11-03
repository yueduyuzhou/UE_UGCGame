#include "MysqlConfig.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

FMysqlConfig* FMysqlConfig::Ins = nullptr;

FMysqlConfig::FMysqlConfig()
	:User("root"), 
	Host("127.0.0.1"), 
	Pawd("root"), 
	DB("ugcdatabase"), 
	Port(3306),
	MysqlObj(nullptr)
{
	InitMysql();
}

FMysqlConfig::~FMysqlConfig()
{
	if (Ins)
	{
		delete Ins;
		Ins = nullptr;
	}
}

FMysqlConfig* FMysqlConfig::Get()
{
	if (!Ins) { Ins = new FMysqlConfig(); }
	return Ins;
}

TArray<FSimpleMysqlResult> FMysqlConfig::QueryBySql(const FString& InSql)
{
	FString ErrMsg = "";
	TArray<FSimpleMysqlResult> Results;
	FSimpleMysqlDebugResult Debug;
	Debug.bPrintToLog = true;
	USimpleMySQLLibrary::QueryLinkResult(
		MysqlObj, 
		InSql, 
		Results, 
		ErrMsg, 
		EMysqlQuerySaveType::STORE_RESULT, 
		Debug);

	if (!ErrMsg.IsEmpty())
	{
		UE_LOG(LogUGCGameDBServer, Display, TEXT("QueryBySql : [%s] Fail!!!"), *InSql);
	}
	return Results;
}

void FMysqlConfig::ReplaceTableDatas(const FString& TableName, const TArray<TMap<FString, FString>>& InDatas, bool IsClear = false, const TArray<FSimpleMysqlComparisonOperator>& InClearCondition = TArray<FSimpleMysqlComparisonOperator>())
{
	/*FString ErrMsg = "";

	if (IsClear)
	{
		//清除数据
		USimpleMySQLLibrary::DeleteFromTableWhereData(
			MysqlObj,
			TableName,
			InClearCondition,
			ErrMsg);

		if (!ErrMsg.IsEmpty())
		{
			UE_LOG(LogUGCGameDBServer, Display, TEXT("ReplaceTableDatas : Clear Data Fail!!!"));
		}
	}

	//插入数据
	FSimpleMysqlQueryParameters QueryParam;
	ErrMsg = "";
	for (int32 i = 0; i < InDatas.Num(); i++)
	{
		USimpleMySQLLibrary::InsertTableData(
			MysqlObj,
			TableName,
			InDatas[i],
			QueryParam,
			ErrMsg);
	}

	if (!ErrMsg.IsEmpty())
	{
		UE_LOG(LogUGCGameDBServer, Display, TEXT("ReplaceTableDatas : Insert Data Fail!!!"));
	}*/

	if (IsClear && !DeleteTableDatas(TableName, InClearCondition))
	{
		UE_LOG(LogUGCGameDBServer, Display, TEXT("ReplaceTableDatas : Clear Data Fail!!!"));
	}

	if (!InsertTableDatas(TableName, InDatas))
	{
		UE_LOG(LogUGCGameDBServer, Display, TEXT("ReplaceTableDatas : Insert Data Fail!!!"));
	}
}

bool FMysqlConfig::InsertTableDatas(const FString& TableName, const TArray<TMap<FString, FString>>& InDatas)
{
	//插入数据
	FSimpleMysqlQueryParameters QueryParam;
	FString ErrMsg = "";
	for (int32 i = 0; i < InDatas.Num(); i++)
	{
		USimpleMySQLLibrary::InsertTableData(
			MysqlObj,
			TableName,
			InDatas[i],
			QueryParam,
			ErrMsg);
	}

	if (!ErrMsg.IsEmpty())
	{
		UE_LOG(LogUGCGameDBServer, Display, TEXT("InsertTableDatas : Insert Data Fail!!!"));
		return false;
	}

	return true;
}

bool FMysqlConfig::DeleteTableDatas(const FString& TableName, const TArray<FSimpleMysqlComparisonOperator>& InClearCondition)
{
	//清除数据
	FString ErrMsg = "";
	USimpleMySQLLibrary::DeleteFromTableWhereData(
		MysqlObj,
		TableName,
		InClearCondition,
		ErrMsg);

	if (!ErrMsg.IsEmpty())
	{
		UE_LOG(LogUGCGameDBServer, Error, TEXT("DeleteTableDatas : Clear Data Fail, %s"), *ErrMsg);
		return false;
	}

	return true;
}

void FMysqlConfig::InitMysql()
{
	ClientFlags.Add(ESimpleClientFlags::Client_Multi_Statements);

	MysqlObj = USimpleMySQLLibrary::CreateMysqlObject(
		nullptr,
		User,
		Host,
		Pawd,
		DB,
		Port,
		ClientFlags);
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

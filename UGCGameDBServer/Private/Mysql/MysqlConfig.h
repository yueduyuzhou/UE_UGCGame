// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/SimpleMysqlLinkType.h"
#include "SimpleMySQLibrary.h"

class FMysqlConfig
{
private:
	FMysqlConfig();
	~FMysqlConfig();

public:
	static FMysqlConfig* Get();

	/*输入查询语句*/
	TArray<FSimpleMysqlResult> QueryBySql(const FString& InSql);

	/*替换数据*/
	void ReplaceTableDatas(const FString& TableName, const TArray<TMap<FString, FString>>& InDatas, bool IsClear, const TArray<FSimpleMysqlComparisonOperator>& InClearCondition);

	/*插入*/
	bool InsertTableDatas(const FString& TableName, const TArray<TMap<FString, FString>>& InDatas);

	/*删除*/
	bool DeleteTableDatas(const FString& TableName, const TArray<FSimpleMysqlComparisonOperator>& InClearCondition);

private:
	void InitMysql();

private:
	FString User;
	FString Host;
	FString Pawd;
	FString DB;
	int32 Port;
	TArray<ESimpleClientFlags> ClientFlags;

	USimpleMysqlObject* MysqlObj;

	static FMysqlConfig* Ins;
};
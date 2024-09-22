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

	TArray<FSimpleMysqlResult> QueryBySql(const FString& InSql);
	void ReplaceTableDatas(const FString& TableName, const TArray<TMap<FString, FString>>& InDatas, bool IsClear, const TArray<FSimpleMysqlComparisonOperator>& InClearCondition);

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
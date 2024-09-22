#pragma once

#include "CoreMinimal.h"
#include "SimpleProtocolsDefinition.h"
#include "../Log/LogUGCGameDBServer.h"
#include "../Protocols/UGCGameProtocols.h"
#include "../Mysql/MysqlConfig.h"
//#include "BaseModule.generated.h"

//UCLASS()
class UBaseModule
{
	//GENERATED_BODY()
protected:
	UBaseModule();
	~UBaseModule();

//public:
	/* 主处理接口，子类重写处理行为 */
	//virtual void Processing(uint32 InProtocol);
};
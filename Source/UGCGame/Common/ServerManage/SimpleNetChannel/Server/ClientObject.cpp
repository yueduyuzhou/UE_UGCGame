#include "ClientObject.h"
#include "SimpleProtocolsDefinition.h"
#include "../../Protocols/UGCGameProtocols.h"
#include "../../Macro/ServerMacro.h"
#include "../../ServerManage.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

void UClientObject::Init()
{
	Super::Init();
}

void UClientObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UClientObject::Close()
{
	Super::Close();
}

void UClientObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	Recv<FString, int32>(InProtocol);
}

template<typename... Args>
void UClientObject::Send(uint32 InProtocol, TTuple<Args...>& args)
{
	//if (IsMainConnetion())
	{
		switch (InProtocol)
		{
			case SP_C2D_DATA_REQUEST:
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, InS + " " + FString::Printf(TEXT("%d %d"), InProtocol, InNum));
				SIMPLE_PROTOCOLS_SEND(SP_C2D_DATA_REQUEST, EXPAND_ARGS(2, args));
				break;
			}
		}

	}
}

template<typename ...Args>
void UClientObject::Recv(uint32 InProtocol)
{
	TTuple<Args...> args;

	switch (InProtocol)
	{
		case SP_D2C_DATA_RESPONSE:
		{
			//接受任意参数
			SIMPLE_PROTOCOLS_RECEIVE(SP_D2C_DATA_RESPONSE, EXPAND_ARGS(2, args));
			break;
		}
	}

	//把接收到的数据传给协议对应回调函数
	FServerManage::Get()->ExecuteCallback<Args...>(InProtocol, EXPAND_ARGS(2, args));
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

#include "ClientObject.h"
#include "SimpleProtocolsDefinition.h"
#include "../../Protocols/UGCGameProtocols.h"
#include "../../Macro/ServerType.h"
#include "../../ServerManage.h"
#include "../../Macro/ServerMacro.h"

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

	switch (InProtocol)
	{
		case SP_D2C_DATA_RESPONSE:
		{
			//SIMPLE_PROTOCOLS_RECEIVE(SP_D2C_DATA_RESPONSE);
			break;
		}
		case SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE:
		{
			FUGC_MAP_ELEMENT_INFO_RESPONSE RecvData;
			SIMPLE_PROTOCOLS_RECEIVE(SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, RecvData.MapID, RecvData.IntData, RecvData.FloatData);
			FServerManage::Get()->ExecuteCallback<FUGC_MAP_ELEMENT_INFO_RESPONSE>(InProtocol, RecvData);
			break;
		}
		case SP_D2C_UGC_MAP_INFO_RESPONSE:
		{
			FUGC_MAP_INFO_RESPONSE RecvData;
			SIMPLE_PROTOCOLS_RECEIVE(SP_D2C_UGC_MAP_INFO_RESPONSE, RecvData.MapIDs, RecvData.MapNames);
			FServerManage::Get()->ExecuteCallback<FUGC_MAP_INFO_RESPONSE>(InProtocol, RecvData);
			break;
		}
		case SP_D2C_LOGIN_REP:
		{
			FLOGIN_REP RecvData;
			SIMPLE_PROTOCOLS_RECEIVE(SP_D2C_UGC_MAP_INFO_RESPONSE, RecvData.IsSuccess);
			FServerManage::Get()->ExecuteCallback<FLOGIN_REP>(InProtocol, RecvData);
			break;
		}
	}	
}

//template<typename T>
//void UClientObject::Send(uint32 InProtocol, T* args)
//{
//	
//}

void UClientObject::Send(uint32 InProtocol, FDATA_REQUEST* args)
{
	SIMPLE_PROTOCOLS_SEND(SP_C2D_DATA_REQUEST, args->TestStr, args->TestNum);
}

void UClientObject::Send(uint32 InProtocol, FLOGIN_REQ* args)
{
	SIMPLE_PROTOCOLS_SEND(SP_C2D_LOGIN_REQ, args->Account, args->Password);
}

void UClientObject::Send(uint32 InProtocol, FUGC_MAP_ELEMENT_INFO_REQUEST* args)
{
	SIMPLE_PROTOCOLS_SEND(SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, args->MapID);
}

void UClientObject::Send(uint32 InProtocol, FUGC_MAP_INFO_REQUEST* args)
{
	SIMPLE_PROTOCOLS_SEND(SP_C2D_UGC_MAP_INFO_REQUEST);
}

void UClientObject::Send(uint32 InProtocol, FUGC_SAVE_MAP_INFO_REQ* args)
{
	SIMPLE_PROTOCOLS_SEND(SP_C2D_UGC_SAVE_MAP_INFO_REQ, args->MapID, args->MapName, args->IntData, args->FloatData);
}

void UClientObject::Send(uint32 InProtocol, FUGC_CREATE_MAP_REQ* args)
{
	SIMPLE_PROTOCOLS_SEND(SP_C2D_UGC_CREATE_MAP_REQ, args->MapNames);
}

//template<typename ...Args>
//void UClientObject::Recv(uint32 InProtocol)
//{
//	TTuple<Args...> args;
//
//	
//}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

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
			PROTOCOLS_RECEIVE(SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, RecvData.MapID, RecvData.IntData, RecvData.FloatData);
			FServerManage::Get()->ExecuteCallback<FUGC_MAP_ELEMENT_INFO_RESPONSE>(InProtocol, RecvData);
			break;
		}
		case SP_D2C_UGC_MAP_INFO_RESPONSE:
		{
			FUGC_MAP_INFO_RESPONSE RecvData;
			PROTOCOLS_RECEIVE(SP_D2C_UGC_MAP_INFO_RESPONSE, RecvData.MapIDs, RecvData.MapNames);
			FServerManage::Get()->ExecuteCallback<FUGC_MAP_INFO_RESPONSE>(InProtocol, RecvData);
			break;
		}
		case SP_D2C_LOGIN_REP:
		{
			FLOGIN_REP RecvData;
			PROTOCOLS_RECEIVE(
				SP_D2C_UGC_MAP_INFO_RESPONSE, 
				RecvData.IsSuccess, 
				RecvData.PlayerInfo.Account, 
				RecvData.PlayerInfo.Gold,
				RecvData.PlayerInfo.ItemIDs);
			FServerManage::Get()->ExecuteCallback<FLOGIN_REP>(InProtocol, RecvData);
			break;
		}
		case SP_D2C_QUIT_REP:
		{
			FQUIT_REP RecvData;
			PROTOCOLS_RECEIVE(
				SP_D2C_UGC_MAP_INFO_RESPONSE,
				RecvData.IsSuccess);
			FServerManage::Get()->ExecuteCallback<FQUIT_REP>(InProtocol, RecvData);
			break;
		}
		case SP_D2C_PLAYER_INFO_REP:
		{
			FPLAYER_INFO_REP RecvData;
			PROTOCOLS_RECEIVE(SP_D2C_PLAYER_INFO_REP, RecvData.Account, RecvData.Gold);
			FServerManage::Get()->ExecuteCallback<FPLAYER_INFO_REP>(InProtocol, RecvData);
			break;
		}
		case SP_D2C_ITEM_INFO_REP:
		{
			FITEM_INFO_REP RecvData;
			PROTOCOLS_RECEIVE(SP_D2C_ITEM_INFO_REP, RecvData.ItemIDs, RecvData.Counts);
			FServerManage::Get()->ExecuteCallback<FITEM_INFO_REP>(InProtocol, RecvData);
			break;
		}
	}	
}

//template<typename T>
//void UClientObject::Send(uint32 InProtocol, T* args)
//{
//	
//}

//void UClientObject::Send(uint32 InProtocol, FDATA_REQUEST* args)
//{
//	PROTOCOLS_SEND(SP_C2D_DATA_REQUEST, args->TestStr, args->TestNum);
//}
//
//void UClientObject::Send(uint32 InProtocol, FLOGIN_REQ* args)
//{
//	PROTOCOLS_SEND(SP_C2D_LOGIN_REQ, args->Account, args->Password);
//}
//
//void UClientObject::Send(uint32 InProtocol, FUGC_MAP_ELEMENT_INFO_REQUEST* args)
//{
//	PROTOCOLS_SEND(SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, args->MapID);
//}
//
//void UClientObject::Send(uint32 InProtocol, FUGC_MAP_INFO_REQUEST* args)
//{
//	PROTOCOLS_SEND(SP_C2D_UGC_MAP_INFO_REQUEST);
//}
//
//void UClientObject::Send(uint32 InProtocol, FUGC_SAVE_MAP_INFO_REQ* args)
//{
//	PROTOCOLS_SEND(SP_C2D_UGC_SAVE_MAP_INFO_REQ, args->MapID, args->MapName, args->IntData, args->FloatData);
//}
//
//void UClientObject::Send(uint32 InProtocol, FUGC_CREATE_MAP_REQ* args)
//{
//	PROTOCOLS_SEND(SP_C2D_UGC_CREATE_MAP_REQ, args->MapNames);
//}

SEND_FUNC_IMPLEMENTATION_P(DATA_REQUEST, args->TestStr, args->TestNum);
SEND_FUNC_IMPLEMENTATION_P(LOGIN_REQ, args->Account, args->Password);
SEND_FUNC_IMPLEMENTATION(QUIT_REQ);
SEND_FUNC_IMPLEMENTATION_P(UGC_MAP_ELEMENT_INFO_REQUEST, args->MapID);
SEND_FUNC_IMPLEMENTATION(UGC_MAP_INFO_REQUEST);
SEND_FUNC_IMPLEMENTATION_P(UGC_SAVE_MAP_INFO_REQ, args->MapID, args->MapName, args->IntData, args->FloatData);
SEND_FUNC_IMPLEMENTATION_P(UGC_CREATE_MAP_REQ, args->MapNames);
SEND_FUNC_IMPLEMENTATION(ITEM_INFO_REQ);
SEND_FUNC_IMPLEMENTATION_P(BUY_REQ, args->ItemID);
SEND_FUNC_IMPLEMENTATION_P(SAVE_EQUIPPED_WEAPON_INFO_REQ, args->ItemIDs);

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

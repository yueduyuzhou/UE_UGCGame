#include "ServerObject.h"
#include "../Protocols/UGCGameProtocols.h"
#include "Log/LogUGCGameDBServer.h"
#include "../ServerType.h"

/*---------------------------Mod头文件---------------------------*/
#include "../Modules/src/UGC/ElementMod.h"
/*---------------------------------------------------------------*/

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

void UServerObject::Init()
{
	Super::Init();
}

void UServerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UServerObject::Close()
{
	Super::Close();
}

void UServerObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	switch (InProtocol)
	{
		case SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST:
		{
			FUGC_MAP_ELEMENT_INFO_REQUEST InData;
			FUGC_MAP_ELEMENT_INFO_RESPONSE OutData;

			//1.接收
			SIMPLE_PROTOCOLS_RECEIVE(SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, InData.MapID);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [MapID=%d]"), SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, InData.MapID);

			//2.协议对应处理
			OutData = UElementMod::Get()->C2D_UGC_MAP_ELEMENT_INFO_REQUEST(InData.MapID);

			//3.发送响应
			if (OutData.IntData.Num() > 0 || OutData.FloatData.Num() > 0)
			{
				OutData.MapID = InData.MapID;
				UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [MapID=%d, OutData.IntData Size=%d, OutData.FloatData Size=%d]"), SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, InData.MapID, OutData.IntData.Num(), OutData.FloatData.Num());
				SIMPLE_PROTOCOLS_SEND(SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, OutData.MapID, OutData.IntData, OutData.FloatData);
			}
			break;
		}
		case SP_C2D_UGC_MAP_INFO_REQUEST:
		{
			FUGC_MAP_INFO_RESPONSE OutData;

			//1.接收
			SIMPLE_PROTOCOLS_RECEIVE(SP_C2D_UGC_MAP_INFO_REQUEST);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : []"), SP_C2D_UGC_MAP_INFO_REQUEST);

			//2.协议对应处理
			OutData = UElementMod::Get()->C2D_UGC_MAP_INFO_REQUEST();

			//3.发送响应
			if (OutData.MapNames.Num() > 0 || OutData.MapIDs.Num() > 0)
			{
				UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [OutData.MapNames Size=%d, OutData.MapIDs Size=%d]"), SP_D2C_UGC_MAP_INFO_RESPONSE, OutData.MapNames.Num(), OutData.MapIDs.Num());
				SIMPLE_PROTOCOLS_SEND(SP_D2C_UGC_MAP_INFO_RESPONSE, OutData.MapIDs, OutData.MapNames);
			}
			break;
		}
		case SP_C2D_UGC_SAVE_MAP_INFO_REQ:
		{
			FUGC_SAVE_MAP_INFO_REQ InData;
			FUGC_SAVE_MAP_INFO_REP OutData;

			//1.接收
			SIMPLE_PROTOCOLS_RECEIVE(SP_C2D_UGC_SAVE_MAP_INFO_REQ, InData.MapID, InData.MapName, InData.IntData, InData.FloatData);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [MapID=%d, MapName=%s, InData.IntData Size=%d, InData.FloatData Size=%d]"), SP_C2D_UGC_SAVE_MAP_INFO_REQ, InData.MapID, *InData.MapName, InData.IntData.Num(), InData.FloatData.Num());

			//2.协议对应处理
			OutData = UElementMod::Get()->C2D_UGC_SAVE_MAP_INFO_REQ(InData);

			break;
		}
		case SP_C2D_UGC_CREATE_MAP_REQ:
		{
			FUGC_CREATE_MAP_REQ InData;
			FUGC_MAP_INFO_RESPONSE OutData;

			//1.接收
			SIMPLE_PROTOCOLS_RECEIVE(SP_C2D_UGC_CREATE_MAP_REQ, InData.MapNames);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [InData.MapNames Size=%d]"), SP_C2D_UGC_CREATE_MAP_REQ, InData.MapNames.Num());

			//2.协议对应处理
			OutData = UElementMod::Get()->C2D_UGC_CREATE_MAP_REQ(InData);

			//3.发送响应
			if (OutData.MapNames.Num() > 0 || OutData.MapIDs.Num() > 0)
			{
				UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [OutData.MapNames Size=%d, OutData.MapIDs Size=%d]"), SP_C2D_UGC_CREATE_MAP_REQ, OutData.MapNames.Num(), OutData.MapIDs.Num());
				SIMPLE_PROTOCOLS_SEND(SP_D2C_UGC_MAP_INFO_RESPONSE, OutData.MapIDs, OutData.MapNames);
			}
			break;
		}
		case SP_C2D_LOGIN_REQ:
		{
			FLOGIN_REQ InData;
			FLOGIN_REP OutData;

			//1.接收
			SIMPLE_PROTOCOLS_RECEIVE(SP_C2D_LOGIN_REQ, InData.Account, InData.Password);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [InData.Account=%s, InData.Password=%s]"), SP_C2D_LOGIN_REQ, *InData.Account, *InData.Password);

			//2.协议对应处理
			OutData = UElementMod::Get()->C2D_LOGIN_REQ(InData);

			//3.发送响应
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [Player Account=%s Login %s!]"), SP_D2C_LOGIN_REP, *(InData.Account), (OutData.IsSuccess ? "Success" : "Fail"));
			SIMPLE_PROTOCOLS_SEND(SP_D2C_LOGIN_REP, OutData.IsSuccess);
			break;
		}
	}
}

#if PLATFORM_WINDOWS
#pragma optimize("",on)
#endif
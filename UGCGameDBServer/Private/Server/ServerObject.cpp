#include "ServerObject.h"
#include "../Protocols/UGCGameProtocols.h"
#include "Log/LogUGCGameDBServer.h"
#include "../ServerType.h"
#include "../ServerMacro.h"

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
			PROTOCOLS_RECEIVE_P(SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, InData.MapID);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [MapID=%d]"), SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, InData.MapID);

			//2.协议对应处理
			//OutData = UElementMod::Get()->C2D_UGC_MAP_ELEMENT_INFO_REQUEST(InData.MapID);
			PROTOCOLS_IO_DEAL_WITH_SERVER(C2D_UGC_MAP_ELEMENT_INFO_REQUEST, OutData, InData);

			//3.发送响应
			if (OutData.IntData.Num() > 0 || OutData.FloatData.Num() > 0)
			{
				OutData.MapID = InData.MapID;
				UE_LOG(
					LogUGCGameDBServer, 
					Display, 
					TEXT("[Send Protocol=%d] : [MapID=%d, OutData.IntData Size=%d, OutData.FloatData Size=%d]"), 
					SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, 
					InData.MapID, 
					OutData.IntData.Num(), 
					OutData.FloatData.Num());
				PROTOCOLS_SEND(SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, OutData.MapID, OutData.IntData, OutData.FloatData);
			}
			break;
		}
		case SP_C2D_UGC_MAP_INFO_REQUEST:
		{
			FUGC_MAP_INFO_RESPONSE OutData;

			//1.接收
			PROTOCOLS_RECEIVE(SP_C2D_UGC_MAP_INFO_REQUEST);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : []"), SP_C2D_UGC_MAP_INFO_REQUEST);

			//2.协议对应处理
			//OutData = UElementMod::Get()->C2D_UGC_MAP_INFO_REQUEST();
			PROTOCOLS_O_DEAL_WITH_SERVER(C2D_UGC_MAP_INFO_REQUEST, OutData);

			//3.发送响应
			if (OutData.MapNames.Num() > 0 || OutData.MapIDs.Num() > 0)
			{
				UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [OutData.MapNames Size=%d, OutData.MapIDs Size=%d]"), SP_D2C_UGC_MAP_INFO_RESPONSE, OutData.MapNames.Num(), OutData.MapIDs.Num());
				PROTOCOLS_SEND(SP_D2C_UGC_MAP_INFO_RESPONSE, OutData.MapIDs, OutData.MapNames);
			}
			break;
		}
		case SP_C2D_UGC_SAVE_MAP_INFO_REQ:
		{
			FUGC_SAVE_MAP_INFO_REQ InData;
			FUGC_SAVE_MAP_INFO_REP OutData;
			FUGC_MAP_ELEMENT_INFO_REQUEST InMapData;
			FUGC_MAP_ELEMENT_INFO_RESPONSE OutEleData;

			//1.接收
			PROTOCOLS_RECEIVE_P(SP_C2D_UGC_SAVE_MAP_INFO_REQ, InData.MapID, InData.MapName, InData.IntData, InData.FloatData);
			UE_LOG(
				LogUGCGameDBServer, 
				Display, 
				TEXT("[Recv Protocol=%d] : [MapID=%d, MapName=%s, InData.IntData Size=%d, InData.FloatData Size=%d]"), 
				SP_C2D_UGC_SAVE_MAP_INFO_REQ, 
				InData.MapID, 
				*InData.MapName, 
				InData.IntData.Num(), 
				InData.FloatData.Num());

			//2.协议对应处理
			InMapData.MapID = InData.MapID;
			PROTOCOLS_IO_DEAL_WITH_SERVER(C2D_UGC_SAVE_MAP_INFO_REQ, OutData, InData);
			PROTOCOLS_IO_OTHRE_DEAL_WITH_SERVER(C2D_UGC_SAVE_MAP_INFO_REQ, C2D_UGC_MAP_ELEMENT_INFO_REQUEST, OutEleData, InMapData);

			//3.发送响应
			if (OutEleData.IntData.Num() > 0 || OutEleData.FloatData.Num() > 0)
			{
				OutEleData.MapID = InMapData.MapID;
				UE_LOG(
					LogUGCGameDBServer, 
					Display, 
					TEXT("[Send Protocol=%d] : [MapID=%d, OutData.IntData Size=%d, OutData.FloatData Size=%d]"), 
					SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, 
					OutEleData.MapID, 
					OutEleData.IntData.Num(), 
					OutEleData.FloatData.Num());
				PROTOCOLS_SEND(SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, OutEleData.MapID, OutEleData.IntData, OutEleData.FloatData);
			}
			break;
		}
		case SP_C2D_UGC_CREATE_MAP_REQ:
		{
			FUGC_CREATE_MAP_REQ InData;
			FUGC_MAP_INFO_RESPONSE OutData;

			//1.接收
			PROTOCOLS_RECEIVE_P(SP_C2D_UGC_CREATE_MAP_REQ, InData.MapNames);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [InData.MapNames Size=%d]"), SP_C2D_UGC_CREATE_MAP_REQ, InData.MapNames.Num());

			//2.协议对应处理
			//OutData = UElementMod::Get()->C2D_UGC_CREATE_MAP_REQ(InData);
			PROTOCOLS_IO_DEAL_WITH_SERVER(C2D_UGC_CREATE_MAP_REQ, OutData, InData);

			//3.发送响应
			if (OutData.MapNames.Num() > 0 || OutData.MapIDs.Num() > 0)
			{
				UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [OutData.MapNames Size=%d, OutData.MapIDs Size=%d]"), SP_C2D_UGC_CREATE_MAP_REQ, OutData.MapNames.Num(), OutData.MapIDs.Num());
				PROTOCOLS_SEND(SP_D2C_UGC_MAP_INFO_RESPONSE, OutData.MapIDs, OutData.MapNames);
			}
			break;
		}
		case SP_C2D_LOGIN_REQ:
		{
			FLOGIN_REQ InData;
			FLOGIN_REP OutData;
			FITEM_INFO_REP ItemsData;

			//1.接收
			PROTOCOLS_RECEIVE_P(SP_C2D_LOGIN_REQ, InData.Account, InData.Password);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [InData.Account=%s, InData.Password=%s]"), SP_C2D_LOGIN_REQ, *InData.Account, *InData.Password);

			//2.协议对应处理
			PROTOCOLS_IO_DEAL_WITH_SERVER(C2D_LOGIN_REQ, OutData, InData);
			PROTOCOLS_O_OTHRE_DEAL_WITH_SERVER(C2D_LOGIN_REQ, C2D_ITEM_INFO_REQ, ItemsData);

			//3.发送响应
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [Player Account=%s Login %s!]"), SP_D2C_LOGIN_REP, *(OutData.PlayerInfo.Account), (OutData.IsSuccess ? "Success" : "Fail"));
			PROTOCOLS_SEND(SP_D2C_LOGIN_REP, 
				OutData.IsSuccess, 
				OutData.PlayerInfo.Account, 
				OutData.PlayerInfo.Gold,
				OutData.PlayerInfo.ItemIDs);

			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [OutData.ItemIDs Length=%d, OutData.Counts Length=%d]"), SP_D2C_ITEM_INFO_REP, ItemsData.ItemIDs.Num(), ItemsData.Counts.Num());
			PROTOCOLS_SEND(SP_D2C_ITEM_INFO_REP,
				ItemsData.ItemIDs,
				ItemsData.Counts);
			break;
		}
		case SP_C2D_QUIT_REQ:
		{
			FQUIT_REP OutData;

			//1.接收
			PROTOCOLS_RECEIVE_P(SP_C2D_QUIT_REQ);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [Player Guid=%s Request Quit]"), SP_C2D_QUIT_REQ, *Guid_SP_C2D_QUIT_REQ);

			//2.协议对应处理
			PROTOCOLS_O_DEAL_WITH_SERVER(C2D_QUIT_REQ, OutData);

			//3.发送响应
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [Player Guid=%s Quit %s]"), SP_D2C_QUIT_REP, *Guid_SP_C2D_QUIT_REQ, (OutData.IsSuccess ? "Success" : "Fail"));
			PROTOCOLS_SEND(SP_D2C_QUIT_REP, OutData.IsSuccess);
			break;
		}
		case SP_C2D_PLAYER_INFO_REQ:
		{
			FPLAYER_INFO_REP OutData;

			//1.接收
			PROTOCOLS_RECEIVE(SP_C2D_PLAYER_INFO_REQ);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : []"), SP_C2D_PLAYER_INFO_REQ);

			//2.协议对应处理
			PROTOCOLS_O_DEAL_WITH_SERVER(C2D_PLAYER_INFO_REQ, OutData);

			//3.发送响应
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [Player Account=%s, Gold=%d]"), SP_D2C_PLAYER_INFO_REP, *(OutData.Account), OutData.Gold);
			PROTOCOLS_SEND(SP_D2C_PLAYER_INFO_REP,
				OutData.Account,
				OutData.Gold,
				OutData.ItemIDs);
			break;
		}
		case SP_C2D_ITEM_INFO_REQ:
		{
			FITEM_INFO_REQ InData;
			FITEM_INFO_REP OutData;

			//1.接收
			PROTOCOLS_RECEIVE(SP_C2D_ITEM_INFO_REQ);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : []"), SP_C2D_ITEM_INFO_REQ);

			//2.协议对应处理
			PROTOCOLS_IO_DEAL_WITH_SERVER(C2D_ITEM_INFO_REQ, OutData, InData);

			//3.发送响应
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [OutData.ItemIDs Length=%d, OutData.Counts Length=%d]"), SP_D2C_ITEM_INFO_REP, OutData.ItemIDs.Num(), OutData.Counts.Num());
			PROTOCOLS_SEND(SP_D2C_ITEM_INFO_REP,
				OutData.ItemIDs,
				OutData.Counts);
			break;
		}
		case SP_C2D_BUY_REQ:
		{
			FBUY_REQ InData;
			FITEM_INFO_REP ItemsData;
			FPLAYER_INFO_REP PlayerInfo;

			//1.接收
			PROTOCOLS_RECEIVE_P(SP_C2D_BUY_REQ, InData.ItemID);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [ItemID=%d]"), SP_C2D_BUY_REQ, InData.ItemID);

			//2.协议对应处理
			PROTOCOLS_I_DEAL_WITH_SERVER(C2D_BUY_REQ, InData);
			PROTOCOLS_O_OTHRE_DEAL_WITH_SERVER(C2D_BUY_REQ, C2D_PLAYER_INFO_REQ, PlayerInfo);
			PROTOCOLS_O_OTHRE_DEAL_WITH_SERVER(C2D_BUY_REQ, C2D_ITEM_INFO_REQ, ItemsData);

			//3.发送响应(item和playerinfo)
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [OutData.ItemIDs Length=%d, OutData.Counts Length=%d]"), SP_D2C_ITEM_INFO_REP, ItemsData.ItemIDs.Num(), ItemsData.Counts.Num());
			PROTOCOLS_SEND(SP_D2C_ITEM_INFO_REP,
				ItemsData.ItemIDs,
				ItemsData.Counts);

			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [Player Account=%s, Gold=%d]"), SP_D2C_PLAYER_INFO_REP, *(PlayerInfo.Account), PlayerInfo.Gold);
			PROTOCOLS_SEND(SP_D2C_PLAYER_INFO_REP,
				PlayerInfo.Account,
				PlayerInfo.Gold,
				PlayerInfo.ItemIDs);
			break;
		}
		case SP_C2D_SAVE_EQUIPPED_WEAPON_INFO_REQ:
		{
			FSAVE_EQUIPPED_WEAPON_INFO_REQ InData;

			//1.接收
			PROTOCOLS_RECEIVE_P(SP_C2D_SAVE_EQUIPPED_WEAPON_INFO_REQ, InData.ItemIDs);
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Recv Protocol=%d] : [InData.ItemIDs Length=%d]"), SP_C2D_SAVE_EQUIPPED_WEAPON_INFO_REQ, InData.ItemIDs.Num());

			//2.协议对应处理
			PROTOCOLS_I_DEAL_WITH_SERVER(C2D_SAVE_EQUIPPED_WEAPON_INFO_REQ, InData);

			break;
		}
		case SP_C2D_SETTLEMENT_REWARD_REQ:
		{
			FSETTLEMENT_REWARD_REQ InData;
			FPLAYER_INFO_REP PlayerInfo;

			//1.接收
			PROTOCOLS_RECEIVE_P(SP_C2D_SETTLEMENT_REWARD_REQ, InData.Items_ID, InData.Items_Count);
			UE_LOG(
				LogUGCGameDBServer, 
				Display, 
				TEXT("[Recv Protocol=%d] : [InData.Items_ID Length=%d, InData.Items_Count Length=%d]"), 
				SP_C2D_SETTLEMENT_REWARD_REQ,
				InData.Items_ID.Num(),
				InData.Items_Count.Num());

			//2.协议对应处理
			PROTOCOLS_I_DEAL_WITH_SERVER(C2D_SETTLEMENT_REWARD_REQ, InData);
			PROTOCOLS_O_OTHRE_DEAL_WITH_SERVER(C2D_SETTLEMENT_REWARD_REQ, C2D_PLAYER_INFO_REQ, PlayerInfo);

			//3.发送响应
			UE_LOG(LogUGCGameDBServer, Display, TEXT("[Send Protocol=%d] : [Player Account=%s, Gold=%d]"), SP_D2C_PLAYER_INFO_REP, *(PlayerInfo.Account), PlayerInfo.Gold);
			PROTOCOLS_SEND(SP_D2C_PLAYER_INFO_REP,
				PlayerInfo.Account,
				PlayerInfo.Gold,
				PlayerInfo.ItemIDs);
			break;
		}
	}
}

#if PLATFORM_WINDOWS
#pragma optimize("",on)
#endif
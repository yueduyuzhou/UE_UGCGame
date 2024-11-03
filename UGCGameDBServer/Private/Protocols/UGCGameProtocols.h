#pragma once

#include "CoreMinimal.h"
#include "Protocols/SimpleNetProtocols.h"
#include "SimpleProtocolsDefinition.h"

DEFINITION_SIMPLE_PROTOCOLS(C2D_DATA_REQUEST, 50);		/* 测试 获取UGC元素信息请求 {...} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_DATA_RESPONSE, 51);	/* 测试响应 {...} */


/**************************************************登录退出***************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_LOGIN_REQ, 100);		/* 登录 {FString, FString} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_LOGIN_REP, 101);	/* 测试响应 {int32(bool)} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_QUIT_REQ, 102);		/* 退出请求 {} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_QUIT_REP, 103);		/* 退出响应 {int32(bool)} */


/**************************************************UGC***************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_MAP_ELEMENT_INFO_REQUEST, 150);		/* 获取UGC元素信息请求 {int32 MapID} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, 151);	/* 获取UGC元素信息响应 {int32 MapID, int32[], float[]} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_MAP_INFO_REQUEST, 152);		/* 获取UGC所有Map信息请求 {} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_MAP_INFO_RESPONSE, 153);	/* 获取UGC所有Map信息响应 {FString[], int32[]} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_SAVE_MAP_INFO_REQ, 154);	/* 保存UGC的Map信息请求 {int32 MapID, int32[], float[]} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_SAVE_MAP_INFO_REP, 155);	/* 保存UGC的Map信息响应 {} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_CREATE_MAP_REQ, 156);		/* 创建UGC Map请求 {FString[]} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_REMOVE_MAP_REP, 157);		/* 移除UGC Map响应 {...} */


/**************************************************Player***************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_PLAYER_INFO_REQ, 200);		/* 玩家信息请求 {FString[]} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_PLAYER_INFO_REP, 201);		/* 玩家信息响应 {...} */


/************************************************Hypermarket*************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_ITEM_INFO_REQ, 250);		/* item信息请求 {} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_ITEM_INFO_REP, 251);		/* item信息响应 {...} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_BUY_REQ, 252);		/* 购买请求 {int32} */

/**************************************************Backpack**************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_SAVE_EQUIPPED_WEAPON_INFO_REQ, 300);		/* 保存已装备信息请求 {int32[]} */

/**************************************************Reward**************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_SETTLEMENT_REWARD_REQ, 350);		/* 奖励结算请求 {int32[]，int32[]} */
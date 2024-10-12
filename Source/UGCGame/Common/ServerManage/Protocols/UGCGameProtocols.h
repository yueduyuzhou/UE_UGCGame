#pragma once

#include "CoreMinimal.h"
#include "Protocols/SimpleNetProtocols.h"
#include "SimpleProtocolsDefinition.h"

DEFINITION_SIMPLE_PROTOCOLS(C2D_DATA_REQUEST, 50);		/* ���� ��ȡUGCԪ����Ϣ���� {...} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_DATA_RESPONSE, 51);	/* ������Ӧ {...} */


/**************************************************��¼***************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_LOGIN_REQ, 100);		/* ��¼ {FString, FString} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_LOGIN_REP, 101);	/* ������Ӧ {int32(bool)} */


/**************************************************UGC***************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_MAP_ELEMENT_INFO_REQUEST, 150);		/* ��ȡUGCԪ����Ϣ���� {int32 MapID} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, 151);	/* ��ȡUGCԪ����Ϣ��Ӧ {int32 MapID, int32[], float[]} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_MAP_INFO_REQUEST, 152);		/* ��ȡUGC����Map��Ϣ���� {} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_MAP_INFO_RESPONSE, 153);	/* ��ȡUGC����Map��Ϣ��Ӧ {FString[], int32[]} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_SAVE_MAP_INFO_REQ, 154);	/* ����UGC��Map��Ϣ���� {int32 MapID, int32[], float[]} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_SAVE_MAP_INFO_REP, 155);	/* ����UGC��Map��Ϣ��Ӧ {} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_CREATE_MAP_REQ, 156);		/* ����UGC Map���� {FString[]} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_REMOVE_MAP_REP, 157);		/* �Ƴ�UGC Map��Ӧ {...} */


/**************************************************Player***************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_PLAYER_INFO_REQ, 200);		/* �����Ϣ���� {FString[]} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_PLAYER_INFO_REP, 201);		/* �����Ϣ��Ӧ {...} */


/************************************************Hypermarket*************************************************/
DEFINITION_SIMPLE_PROTOCOLS(C2D_ITEM_INFO_REQ, 250);		/* item��Ϣ���� {} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_ITEM_INFO_REP, 251);		/* item��Ϣ��Ӧ {...} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_BUY_REQ, 252);		/* �������� {int32} */
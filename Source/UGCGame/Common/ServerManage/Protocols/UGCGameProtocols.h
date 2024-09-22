#pragma once

#include "CoreMinimal.h"
#include "Protocols/SimpleNetProtocols.h"
#include "SimpleProtocolsDefinition.h"

DEFINITION_SIMPLE_PROTOCOLS(C2D_DATA_REQUEST, 100);		/* ���� ��ȡUGCԪ����Ϣ���� {...} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_DATA_RESPONSE, 101);	/* ������Ӧ {...} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_MAP_ELEMENT_INFO_REQUEST, 150);		/* ��ȡUGCԪ����Ϣ���� {int32 MapID} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, 151);	/* ��ȡUGCԪ����Ϣ��Ӧ {int32 MapID, int32[], float[]} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_MAP_INFO_REQUEST, 152);		/* ��ȡUGC����Map��Ϣ���� {} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_MAP_INFO_RESPONSE, 153);	/* ��ȡUGC����Map��Ϣ��Ӧ {FString[], int32[]} */

DEFINITION_SIMPLE_PROTOCOLS(C2D_UGC_SAVE_MAP_INFO_REQ, 154);		/* ����UGC��Map��Ϣ���� {int32 MapID, int32[], float[]} */
DEFINITION_SIMPLE_PROTOCOLS(D2C_UGC_SAVE_MAP_INFO_REP, 155);	/* ����UGC��Map��Ϣ��Ӧ {} */
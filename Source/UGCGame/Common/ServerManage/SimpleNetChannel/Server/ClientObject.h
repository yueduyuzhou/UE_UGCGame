#pragma once

#include "CoreMinimal.h"
#include "UObject/SimpleController.h"
#include "../../Macro/ServerType.h"
#include "../../Macro/ServerMacro.h"
#include "ClientObject.generated.h"

UCLASS()
class UGCGAME_API UClientObject : public USimpleController
{
	GENERATED_BODY()

protected:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();

	virtual void RecvProtocol(uint32 InProtocol);

public:

	/*void Send(uint32 InProtocol, FDATA_REQUEST* args);
	void Send(uint32 InProtocol, FLOGIN_REQ* args);
	void Send(uint32 InProtocol, FUGC_MAP_ELEMENT_INFO_REQUEST* args);
	void Send(uint32 InProtocol, FUGC_MAP_INFO_REQUEST* args);
	void Send(uint32 InProtocol, FUGC_SAVE_MAP_INFO_REQ* args);
	void Send(uint32 InProtocol, FUGC_CREATE_MAP_REQ* args);*/
	SEND_FUNC_DECLARATION(DATA_REQUEST);
	SEND_FUNC_DECLARATION(LOGIN_REQ);
	SEND_FUNC_DECLARATION(QUIT_REQ);
	SEND_FUNC_DECLARATION(UGC_MAP_ELEMENT_INFO_REQUEST);
	SEND_FUNC_DECLARATION(UGC_MAP_INFO_REQUEST);
	SEND_FUNC_DECLARATION(UGC_SAVE_MAP_INFO_REQ);
	SEND_FUNC_DECLARATION(UGC_CREATE_MAP_REQ);
	SEND_FUNC_DECLARATION(ITEM_INFO_REQ);
	SEND_FUNC_DECLARATION(BUY_REQ);
	SEND_FUNC_DECLARATION(SAVE_EQUIPPED_WEAPON_INFO_REQ);

	/*template<typename... Args>
	void Recv(uint32 InProtocol);*/
};
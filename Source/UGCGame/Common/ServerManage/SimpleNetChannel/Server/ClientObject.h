#pragma once

#include "CoreMinimal.h"
#include "UObject/SimpleController.h"
#include "../../Macro/ServerType.h"
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
	/*template<typename T>
	void Send(uint32 InProtocol, T* args);*/

	void Send(uint32 InProtocol, FDATA_REQUEST* args);
	void Send(uint32 InProtocol, FUGC_MAP_ELEMENT_INFO_REQUEST* args);
	void Send(uint32 InProtocol, FUGC_MAP_INFO_REQUEST* args);
	void Send(uint32 InProtocol, FUGC_SAVE_MAP_INFO_REQ* args);

	/*template<typename... Args>
	void Recv(uint32 InProtocol);*/
};
#pragma once

#include "CoreMinimal.h"
#include "UObject/SimpleController.h"
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
	template<typename... Args>
	void Send(uint32 InProtocol, TTuple<Args...>& args);

	template<typename... Args>
	void Recv(uint32 InProtocol);
};
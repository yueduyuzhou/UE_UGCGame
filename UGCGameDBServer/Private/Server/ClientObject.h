#pragma once

#include "CoreMinimal.h"
#include "UObject/SimpleController.h"
#include "ClientObject.generated.h"

UCLASS()
class UClientObject : public USimpleController
{
	GENERATED_BODY()

protected:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();

	virtual void RecvProtocol(uint32 InProtocol);
};
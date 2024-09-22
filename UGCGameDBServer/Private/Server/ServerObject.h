#pragma once

#include "CoreMinimal.h"
#include "UObject/SimpleController.h"
#include "ServerObject.generated.h"

UCLASS()
class UServerObject : public USimpleController
{
	GENERATED_BODY()

protected:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();

	virtual void RecvProtocol(uint32 InProtocol);
};
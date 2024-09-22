#include "ClientObject.h"
#include "../Protocols/UGCGameProtocols.h"

void UClientObject::Init()
{
	Super::Init();
}

void UClientObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UClientObject::Close()
{
	Super::Close();
}

void UClientObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);
}
#include "ServerManage.h"
#include "SimpleNetManage.h"
#include "SimpleNetChannel/ServerList.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "SimpleProtocolsDefinition.h"
#include "SimpleNetChannel/Server/ClientObject.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

/*-----------------------------------ʾ��------------------------------------*/
/*
	//��̬�ص�
	static void TestCallBack(FString str, int32 number);

	void UUI_InventorySlot::TestCallBack(FString str, int32 number)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("[TestCallBack]: str = %s, number = %d"), *str, number));
	}

	//���ݿ������ʾ������
	//��ǰ��˫�˶��幫��Э�顪������UGCGameProtocols.h
	FServerManage::Get()->AddCallback<FString, int32>(SP_D2C_DATA_RESPONSE, UUI_InventorySlot::TestCallBack);
	TTuple<FString, int32> args("asd", 2325);
	FServerManage::Get()->Send<FString, int32>(SP_C2D_DATA_REQUEST, args);

	...

	FServerManage::Get()->RemoveCallback<FString, int32>(SP_D2C_DATA_RESPONSE);
*/
/*---------------------------------------------------------------------------*/

FServerManage* FServerManage::Ins = nullptr;

template<typename... Args>
TMultiMap<uint32, CallbackFunction<Args...>> FServerManage::PtcCallBacks;

FServerManage::FServerManage()
{
	/*-------------------------------��ʼ��SNC����-------------------------------*/

	/* 1.��ʼ������*/
	FSimpleNetGlobalInfo::Get()->Init();

	/* 2.�����������ͻ��˶�Ӧʵ��*/
	DBServer = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_UDP);
	/*FSimpleChannel::SimpleControllerDelegate.BindLambda([]() ->UClass*
		{
			return UClientObject::StaticClass();
		});*/

	/* 3.ע��ʵ�� */
	DBServer->NetworkObjectClass = UClientObject::StaticClass();

	/* 4.��ʼ��ʵ�� */
	//��д�������˿ڣ�Ĭ�ϱ���IP��
	if (!DBServer->Init(11223))
	{
		delete DBServer;
		UE_LOG(LogTemp, Error, TEXT("Init FSimpleNetManage Object DBServer Fail"));
		return;
	}

	/* 5.ѭ�����(��ҿͻ��˲���Ҫ) */
	/*double LastTime = FPlatformTime::Seconds();
	while (!IsEngineExitRequested())
	{
		FPlatformProcess::Sleep(0.03f);
		double CurTime = FPlatformTime::Seconds();
		double DeltaSeconds = CurTime - LastTime;

		DBServer->Tick(DeltaSeconds);

		LastTime = CurTime;
	}*/
}

FServerManage::~FServerManage()
{
	/* ����ʵ�� */
	FSimpleNetManage::Destroy(DBServer);

	if (Ins) 
	{
		delete Ins; 
		Ins = nullptr;
	}
}

void FServerManage::Tick(float DeltaSeconds)
{
	DBServer->Tick(DeltaSeconds);
}

FServerManage* FServerManage::Get()
{
	if (!Ins) { Ins = new FServerManage(); }
	return Ins;
}

/*----------------------------Э�鷢�ͽ���----------------------------*/

template<typename T>
void FServerManage::Send(uint32 InProtocol, T* args)
{
	if (UClientObject * ClientObj = Cast<UClientObject>(DBServer->GetController()))
	{
		ClientObj->Send(InProtocol, args);
	}
}

//template<typename... Args>
//void FServerManage::Recv(uint32 InProtocol)
//{
//	switch (InProtocol)
//	{
//		case SP_D2C_DATA_RESPONSE:
//		{
//			//�����������
//			SIMPLE_PROTOCOLS_RECEIVE(SP_D2C_DATA_RESPONSE, Args...);
//			//�ѽ��յ������ݴ���Э���Ӧ�ص�����
//			ExecuteCallback(SP_D2C_DATA_RESPONSE, Args...);
//			break;
//		}
//	}
//	
//}


/*-----------------------------Э��ص�-----------------------------*/

template<typename... Args>
void FServerManage::AddCallback(uint32 InProtocol, CallbackFunction<Args...> Callback)
{
	PtcCallBacks<Args...>.Add(InProtocol, Callback);
}

template<typename... Args>
void FServerManage::ExecuteCallback(uint32 InProtocol, Args... args)
{
	TArray<CallbackFunction<Args...>> Callbacks;
	PtcCallBacks<Args...>.MultiFind(InProtocol, Callbacks);

	// ִ�������ҵ��Ļص�
	for (const auto& Callback : Callbacks)
	{
		if (Callback)  // ���ص��Ƿ���Ч
		{
			Callback(args...);
		}
	}

	//if (auto * Callback = PtcCallBacks<Args...>.Find(InProtocol))
	//{
	//	//ִ�д洢�Ļص����������ݲ���
	//	(*Callback)(args...); 
	//}
}

/*TODO*/
template<typename... Args>
void FServerManage::RemoveCallback(uint32 InProtocol, CallbackFunction<Args...> Callback)
{
	if (IsExist<Args...>(InProtocol, Callback))
	{
		//PtcCallBacks<Args...>.Remove(InProtocol, Callback);
	}
}

template<typename... Args>
bool FServerManage::IsExist(uint32 InProtocol, CallbackFunction<Args...> Callback)
{
	TArray<CallbackFunction<Args...>> Funcs;
	PtcCallBacks<Args...>.MultiFind(InProtocol, Funcs);  // ��ȡ�� key ��Ӧ������ values

	for (auto& Tmp : Funcs)
	{
		if (&Callback == &Tmp) 
		{
			return true;
		}
	}
	return false;
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

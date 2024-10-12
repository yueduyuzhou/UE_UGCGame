#include "ServerManage.h"
#include "SimpleNetManage.h"
#include "SimpleNetChannel/ServerList.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "SimpleProtocolsDefinition.h"
#include "SimpleNetChannel/Server/ClientObject.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

/*-----------------------------------示例------------------------------------*/
/*
	//静态回调
	static void TestCallBack(FString str, int32 number);

	void UUI_InventorySlot::TestCallBack(FString str, int32 number)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("[TestCallBack]: str = %s, number = %d"), *str, number));
	}

	//数据库服务器示例代码
	//提前在双端定义公共协议————UGCGameProtocols.h
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
	/*-------------------------------初始化SNC对象-------------------------------*/

	/* 1.初始化配置*/
	FSimpleNetGlobalInfo::Get()->Init();

	/* 2.创建服务器客户端对应实例*/
	DBServer = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_UDP);
	/*FSimpleChannel::SimpleControllerDelegate.BindLambda([]() ->UClass*
		{
			return UClientObject::StaticClass();
		});*/

	/* 3.注册实例 */
	DBServer->NetworkObjectClass = UClientObject::StaticClass();

	/* 4.初始化实例 */
	//填写服务器端口（默认本地IP）
	if (!DBServer->Init(11223))
	{
		delete DBServer;
		UE_LOG(LogTemp, Error, TEXT("Init FSimpleNetManage Object DBServer Fail"));
		return;
	}

	/* 5.循环检测(玩家客户端不需要) */
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
	/* 销毁实例 */
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

/*----------------------------协议发送接收----------------------------*/

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
//			//接受任意参数
//			SIMPLE_PROTOCOLS_RECEIVE(SP_D2C_DATA_RESPONSE, Args...);
//			//把接收到的数据传给协议对应回调函数
//			ExecuteCallback(SP_D2C_DATA_RESPONSE, Args...);
//			break;
//		}
//	}
//	
//}


/*-----------------------------协议回调-----------------------------*/

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

	// 执行所有找到的回调
	for (const auto& Callback : Callbacks)
	{
		if (Callback)  // 检查回调是否有效
		{
			Callback(args...);
		}
	}

	//if (auto * Callback = PtcCallBacks<Args...>.Find(InProtocol))
	//{
	//	//执行存储的回调函数并传递参数
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
	PtcCallBacks<Args...>.MultiFind(InProtocol, Funcs);  // 获取该 key 对应的所有 values

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

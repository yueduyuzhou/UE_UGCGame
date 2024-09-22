#include "./UGCGameDBServer.h"
#include "Log/LogUGCGameDBServer.h"
#include "RequiredProgramMainCPPInclude.h"
#include "Containers/Ticker.h"
#include "SimpleNetManage.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "ServerList.h"
#include "Server/ClientObject.h"
#include "Server/ServerObject.h"

IMPLEMENT_APPLICATION(UGCGameDBServer, "UGCGameDBServer");

INT32_MAIN_INT32_ARGC_TCHAR_ARGV()
{
	GEngineLoop.PreInit(ArgC, ArgV);
	UE_LOG(LogUGCGameDBServer, Display, TEXT("Hello World"));


	/* 1.初始化配置*/
	FSimpleNetGlobalInfo::Get()->Init();

	/* 2.创建服务器客户端对应实例*/
	LocalServer = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_LISTEN, ESimpleSocketType::SIMPLESOCKETTYPE_UDP);
	//Client = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_UDP);

	/* 3.注册实例 */
	LocalServer->NetworkObjectClass = UServerObject::StaticClass();
	//Client->NetworkObjectClass = UClientObject::StaticClass();

	/* 4.初始化实例 */
	if (!LocalServer->Init())
	{
		delete LocalServer;
		UE_LOG(LogUGCGameDBServer, Display, TEXT("LocalServer Init Fail!!!"));
		return -1;
	}

	/*if (!Client->Init(11224))
	{
		delete Client;
		UE_LOG(LogUGCGameDBServer, Display, TEXT("Client Init Fail!!!"));
		return -1;
	}*/

	/* 5.循环检测 */
	double LastTime = FPlatformTime::Seconds();
	while (!IsEngineExitRequested())
	{
		FPlatformProcess::Sleep(0.03f);
		double CurTime = FPlatformTime::Seconds();
		double DeltaSeconds = CurTime - LastTime;

		LocalServer->Tick(DeltaSeconds);
		//Client->Tick(DeltaSeconds);

		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		//FTSTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
		FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());

		LastTime = CurTime;
	}

	/* 6.销毁实例 */
	FSimpleNetManage::Destroy(LocalServer);
	//FSimpleNetManage::Destroy(Client);

	return 0;
}

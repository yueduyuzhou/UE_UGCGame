#pragma once

#include "CoreMinimal.h"
#include "Macro/ServerType.h"
#include "Protocols/UGCGameProtocols.h"

// 定义一个模板别名，用于存储带参数的回调函数
template<typename... Args>
using CallbackFunction = TFunction<void(Args...)>;

class FServerManage
{
private:
	FServerManage();
	~FServerManage();

public:
	void Tick(float DeltaSeconds);

public:
	static FServerManage* Get();

	/*----------------------------协议发送接收----------------------------*/
	/* 发送协议 */
	template<typename T>
	void Send(uint32 InProtocol, T* args);

	/* 接受协议 */
	/*template<typename... Args>
	void Recv(uint32 InProtocol);*/

	/*-----------------------------协议回调-----------------------------*/
	/* 添加带参数的回调函数 */
	template<typename... Args>
	void AddCallback(uint32 InProtocol, CallbackFunction<Args...> Callback);

	/* 执行对应回调函数 */
	template<typename... Args>
	void ExecuteCallback(uint32 InProtocol, Args... args);

	/* 删除特定协议号的回调函数 */
	template<typename... Args>
	void RemoveCallback(uint32 InProtocol, CallbackFunction<Args...> Callback);

	/* 是否存在回调 */
	template<typename... Args>
	bool IsExist(uint32 InProtocol, CallbackFunction<Args...> Callback);



private:
	static FServerManage* Ins;

	// 存储回调函数的 TMap，键是协议号，值是接受参数的 TFunction
	template<typename... Args>
	static TMultiMap<uint32, CallbackFunction<Args...>> PtcCallBacks;
};
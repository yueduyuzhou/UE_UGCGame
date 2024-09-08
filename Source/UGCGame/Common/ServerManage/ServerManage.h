#pragma once

#include "CoreMinimal.h"

// ����һ��ģ����������ڴ洢�������Ļص�����
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

	/*----------------------------Э�鷢�ͽ���----------------------------*/
	/* ����Э�� */
	template<typename... Args>
	void Send(uint32 InProtocol, TTuple<Args...>& args);

	/* ����Э�� */
	/*template<typename... Args>
	void Recv(uint32 InProtocol);*/

	/*-----------------------------Э��ص�-----------------------------*/
	/* ��Ӵ������Ļص����� */
	template<typename... Args>
	void AddCallback(uint32 InProtocol, CallbackFunction<Args...> Callback);

	/* ִ�ж�Ӧ�ص����� */
	template<typename... Args>
	void ExecuteCallback(uint32 InProtocol, Args... args);

	/* ɾ���ض�Э��ŵĻص����� */
	template<typename... Args>
	void RemoveCallback(uint32 InProtocol);

private:
	static FServerManage* Ins;

	// �洢�ص������� TMap������Э��ţ�ֵ�ǽ��ܲ����� TFunction
	template<typename... Args>
	static TMap<uint32, CallbackFunction<Args...>> PtcCallBacks;
};
// Copyright (C) RenZhai.2021.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "SimpleProtocolsDefinition.h"

/***
 //C++
 // 定义一个 TTuple 包含多个元素
	TTuple<int, float, FString, int, float, FString> args(1, 3.14f, FString(TEXT("Hello")), 42, 2.718f, FString(TEXT("World")));

	// 使用宏展开 3 个 args.Get<N>()，即 args.Get<0>(), args.Get<1>(), args.Get<2>()
	std::cout << EXPAND_ARGS(3, args) << std::endl;  // 输出: 1, 3.14, Hello

	// 使用宏展开 6 个 args.Get<N>()，即 args.Get<0>() 到 args.Get<5>()
	std::cout << EXPAND_ARGS(6, args) << std::endl;  // 输出: 1, 3.14, Hello, 42, 2.718, World

***/

// 定义递归宏，处理从1到10的情况
#define EXPAND_1(args) args.Get<0>()
#define EXPAND_2(args) args.Get<0>(), args.Get<1>()
#define EXPAND_3(args) args.Get<0>(), args.Get<1>(), args.Get<2>()
#define EXPAND_4(args) args.Get<0>(), args.Get<1>(), args.Get<2>(), args.Get<3>()
#define EXPAND_5(args) args.Get<0>(), args.Get<1>(), args.Get<2>(), args.Get<3>(), args.Get<4>()
#define EXPAND_6(args) args.Get<0>(), args.Get<1>(), args.Get<2>(), args.Get<3>(), args.Get<4>(), args.Get<5>()
#define EXPAND_7(args) args.Get<0>(), args.Get<1>(), args.Get<2>(), args.Get<3>(), args.Get<4>(), args.Get<5>(), args.Get<6>()
#define EXPAND_8(args) args.Get<0>(), args.Get<1>(), args.Get<2>(), args.Get<3>(), args.Get<4>(), args.Get<5>(), args.Get<6>(), args.Get<7>()
#define EXPAND_9(args) args.Get<0>(), args.Get<1>(), args.Get<2>(), args.Get<3>(), args.Get<4>(), args.Get<5>(), args.Get<6>(), args.Get<7>(), args.Get<8>()
#define EXPAND_10(args) args.Get<0>(), args.Get<1>(), args.Get<2>(), args.Get<3>(), args.Get<4>(), args.Get<5>(), args.Get<6>(), args.Get<7>(), args.Get<8>(), args.Get<9>()

// 主宏：根据传入的数字 a 展开相应数量的 args.Get<N>()
#define EXPAND_ARGS(a, args) EXPAND_##a(args)

/* 通用发送 */
#define PROTOCOLS_SEND(InProtocols,...) \
			FString Guid_##InProtocols = GetChannel()->GetGuid().ToString(); \
			SIMPLE_PROTOCOLS_SEND(InProtocols,Guid_##InProtocols,__VA_ARGS__)

/* 通用接收 */
#define PROTOCOLS_RECEIVE(InProtocols,...) \
			FString Guid_##InProtocols; \
			SIMPLE_PROTOCOLS_RECEIVE(InProtocols,Guid_##InProtocols,__VA_ARGS__) \
			if(Guid_##InProtocols != GetChannel()->GetGuid().ToString()) {return;}

/* 客户端处理 ClientObject 发送函数声明和定义宏 */
#define SEND_FUNC_DECLARATION(PrototolName) \
			void Send(uint32 InProtocol,F##PrototolName* args);

#define SEND_FUNC_IMPLEMENTATION_P(PrototolName,...) \
			void UClientObject::Send(uint32 InProtocol,F##PrototolName* args) \
			{PROTOCOLS_SEND(SP_C2D_##PrototolName,__VA_ARGS__);}

#define SEND_FUNC_IMPLEMENTATION(PrototolName) \
			void UClientObject::Send(uint32 InProtocol,F##PrototolName* args) \
			{PROTOCOLS_SEND(SP_C2D_##PrototolName);}
// Copyright (C) RenZhai.2021.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

/***
 //C++
 // ����һ�� TTuple �������Ԫ��
	TTuple<int, float, FString, int, float, FString> args(1, 3.14f, FString(TEXT("Hello")), 42, 2.718f, FString(TEXT("World")));

	// ʹ�ú�չ�� 3 �� args.Get<N>()���� args.Get<0>(), args.Get<1>(), args.Get<2>()
	std::cout << EXPAND_ARGS(3, args) << std::endl;  // ���: 1, 3.14, Hello

	// ʹ�ú�չ�� 6 �� args.Get<N>()���� args.Get<0>() �� args.Get<5>()
	std::cout << EXPAND_ARGS(6, args) << std::endl;  // ���: 1, 3.14, Hello, 42, 2.718, World

***/

// ����ݹ�꣬�����1��10�����
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

// ���꣺���ݴ�������� a չ����Ӧ������ args.Get<N>()
#define EXPAND_ARGS(a, args) EXPAND_##a(args)

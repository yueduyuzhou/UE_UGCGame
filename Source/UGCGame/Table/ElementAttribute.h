// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UGCGameTableBase.h"
#include "ElementAttribute.generated.h"

class AElementBase;

USTRUCT(BlueprintType)
struct FElementAttribute : public FUGCGameTableBase
{
	GENERATED_USTRUCT_BODY()

public:
	FElementAttribute();

	/*模型*/
	UPROPERTY(EditDefaultsOnly, Category = "Element Attribute")
		UStaticMesh* ElementMeth;
	
	UPROPERTY(EditDefaultsOnly, Category = "Element Attribute")
		TSubclassOf<AElementBase> ElementClass;

	/* -1：无限制    0：不可生成    1(???)：最多生成一个(???个)*/
	UPROPERTY(EditDefaultsOnly, Category = "Element Attribute")
		int32 SpanwLimit;

public:
	/*属性初始化*/
	void Init();
};
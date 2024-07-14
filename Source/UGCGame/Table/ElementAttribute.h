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

	/*ģ��*/
	UPROPERTY(EditDefaultsOnly, Category = "Element Attribute")
		UStaticMesh* ElementMeth;
	
	UPROPERTY(EditDefaultsOnly, Category = "Element Attribute")
		TSubclassOf<AElementBase> ElementClass;

	/* -1��������    0����������    1(???)���������һ��(???��)*/
	UPROPERTY(EditDefaultsOnly, Category = "Element Attribute")
		int32 SpanwLimit;

public:
	/*���Գ�ʼ��*/
	void Init();
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UGCGame/Common/UGCGameType.h"
#include "UI_DetailsBase.generated.h"

class AElementBase;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_DetailsBase : public UUI_Base
{
	GENERATED_BODY()
	
public:
	/*子类重写，行为绑定到Element的属性上*/
	virtual void BindElementProperty(const EEditDetailType& InType, AElementBase* InElement);

	/*子类重写，注册修改类型和Element实例*/
	virtual void RegisterElement(const EEditDetailType& InType, AElementBase* InElement);

protected:
	EEditDetailType DetailType;

	AElementBase* SelectElement;
};

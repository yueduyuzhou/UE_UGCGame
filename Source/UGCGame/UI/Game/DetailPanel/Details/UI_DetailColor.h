// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI_DetailsBase.h"
#include "UI_DetailColor.generated.h"

/**
 * 
 */

//DECLARE_DELEGATE_TwoParams(FBindElementPropertyDelegate, const float, AElementBase*)

UCLASS()
class UGCGAME_API UUI_DetailColor : public UUI_DetailsBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();

public:
	virtual void BindElementProperty(const EEditDetailType& InType, AElementBase* InElement) override;

	virtual void RegisterElement(const EEditDetailType& InType, AElementBase* InElement) override;

	UFUNCTION(BlueprintCallable, Category = "ElementColor")
	void SetElementColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable, Category = "ElementColor")
	FLinearColor GetElementColor();
};

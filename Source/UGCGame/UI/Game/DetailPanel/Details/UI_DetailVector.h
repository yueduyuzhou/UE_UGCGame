// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_Base.h"
#include "UGCGame/Common/UGCGameType.h"
#include "UI_DetailVector.generated.h"

class UTextBlock;
class UUI_Item;
class AElementBase;

/**
 * 
 */

DECLARE_DELEGATE_TwoParams(FBindElementPropertyDelegate, const float, AElementBase*)

UCLASS()
class UGCGAME_API UUI_DetailVector : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Name;

	UPROPERTY(meta = (BindWidget))
		UUI_Item* X;

	UPROPERTY(meta = (BindWidget))
		UUI_Item* Y;

	UPROPERTY(meta = (BindWidget))
		UUI_Item* Z;

protected:
	virtual void NativeConstruct();

public:
	void BindElementProperty(const EEditDetailType& InType, AElementBase* InElement);
	
	void UpdateVector(const EVectorDime& InDime, const float InValue);
	void UpdateVectorText(const FVector& InVector);

	void InitXYZ();
	void RegisterElement(const EEditDetailType& InType, AElementBase* InElement);

	void SetVectorName(const FString& InName);
	FORCEINLINE const EEditDetailType& GetDetailType() { return DetailType; }


public:
	FBindElementPropertyDelegate BindElementPropertyXDelegate;
	FBindElementPropertyDelegate BindElementPropertyYDelegate;
	FBindElementPropertyDelegate BindElementPropertyZDelegate;

private:
	EEditDetailType DetailType;

	AElementBase* SelectElement;

};

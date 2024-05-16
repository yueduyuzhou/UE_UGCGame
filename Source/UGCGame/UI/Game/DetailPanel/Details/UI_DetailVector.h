// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI_DetailsBase.h"
#include "UI_DetailVector.generated.h"

class UTextBlock;
class UUI_Item;
class AElementBase;

/**
 * 
 */

DECLARE_DELEGATE_TwoParams(FBindElementPropertyDelegate, const float, AElementBase*)

UCLASS()
class UGCGAME_API UUI_DetailVector : public UUI_DetailsBase
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
	virtual void BindElementProperty(const EEditDetailType& InType, AElementBase* InElement) override;
	
	void UpdateVector(const EVectorDime& InDime, const float InValue);
	void UpdateVectorText(const FVector& InVector);

	void InitXYZ();
	virtual void RegisterElement(const EEditDetailType& InType, AElementBase* InElement) override;

	void SetVectorName(const FString& InName);
	FORCEINLINE const EEditDetailType& GetDetailType() { return DetailType; }


public:
	/*************
	*  Location
	*************/
	FBindElementPropertyDelegate BindElementLocationXDelegate;
	FBindElementPropertyDelegate BindElementLocationYDelegate;
	FBindElementPropertyDelegate BindElementLocationZDelegate;

	/*************
	*  Rotation
	*************/
	FBindElementPropertyDelegate BindElementRotationXDelegate;
	FBindElementPropertyDelegate BindElementRotationYDelegate;
	FBindElementPropertyDelegate BindElementRotationZDelegate;

	/*************
	*  Scale
	*************/
	FBindElementPropertyDelegate BindElementScaleXDelegate;
	FBindElementPropertyDelegate BindElementScaleYDelegate;
	FBindElementPropertyDelegate BindElementScaleZDelegate;

};

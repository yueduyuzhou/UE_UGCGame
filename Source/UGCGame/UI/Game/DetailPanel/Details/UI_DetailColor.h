// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI_DetailsBase.h"
#include "UI_DetailColor.generated.h"

class UUI_ColorPalette;
/**
 * 
 */

//DECLARE_DELEGATE_TwoParams(FBindElementPropertyDelegate, const float, AElementBase*)

UCLASS()
class UGCGAME_API UUI_DetailColor : public UUI_DetailsBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		class UImage* Color;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUI_ColorPalette> ColorPaletteClass;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void BindElementProperty(const EEditDetailType& InType, AElementBase* InElement) override;

	virtual void RegisterElement(const EEditDetailType& InType, AElementBase* InElement) override;

	UFUNCTION(BlueprintCallable, Category = "ElementColor")
	void SetElementColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable, Category = "ElementColor")
	FLinearColor GetElementColor();

	UFUNCTION(BlueprintCallable, Category = "ElementColor")
	FVector2D ColorToPosiotion(float InDis, float Theta);

	void DestoryColorPalette();

private:
	UFUNCTION()
	FEventReply MouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnOKClick();
	UFUNCTION()
	void OnCancelClick();


private:
	FLinearColor OldColor;

	UUI_ColorPalette* ColorPalette;
};

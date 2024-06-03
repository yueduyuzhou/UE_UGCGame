// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_ColorPalette.generated.h"

class UButton;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOKClickDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelClickDelegate);

UCLASS()
class UGCGAME_API UUI_ColorPalette : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		class UImage* ColorWheel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		class UImage* ColorSelect;

	UPROPERTY(meta = (BindWidget))
		UButton* OK;

	UPROPERTY(meta = (BindWidget))
		UButton* Cancel;

	UPROPERTY(BlueprintAssignable, Category = "CustomDelegate")
		FOnOKClickDelegate OKClickDelegate;

	UPROPERTY(BlueprintAssignable, Category = "CustomDelegate")
		FOnCancelClickDelegate CancelClickDelegate;

public:
	virtual void NativeConstruct() override;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
		void OnOKButtonClicked();
	UFUNCTION()
		void OnCancelButtonClicked();

private:
	bool bIsMouseDown;
};

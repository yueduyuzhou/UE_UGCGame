// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_ColorPalette.generated.h"

class UImage;
class UButton;
class UVerticalBox;
class AElementBase;


/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOKClickDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelClickDelegate);

UCLASS()
class UGCGAME_API UUI_ColorPalette : public UUI_Base
{
	friend class UUI_DetailColor;

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UImage* ColorWheel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UImage* ColorSelect;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UImage * ShowColor;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UImage* TopImage;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* VerBox;

	UPROPERTY(meta = (BindWidget))
		UButton* OK;

	UPROPERTY(meta = (BindWidget))
		UButton* Cancel;

	UPROPERTY(BlueprintAssignable, Category = "CustomDelegate")
		FOnOKClickDelegate OKClickDelegate;

	UPROPERTY(BlueprintAssignable, Category = "CustomDelegate")
		FOnCancelClickDelegate CancelClickDelegate;

private:
	UUI_ColorPalette();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
		void OnOKButtonClicked();
	UFUNCTION()
		void OnCancelButtonClicked();

public:
	void InitColorSelectPosition(const FVector2D& InPosition);

	FLinearColor GetCurrentColor();

	float GetH();
	float GetS();
	float GetV();

	void StartUpdateColor();
	void StopUpdateColor();

	void RegisterElement(AElementBase* InElement);

private:
	bool bIsChangeColor;
	bool bIsFollowMouse;

	bool bIsUpdateColor;

	FVector2D LastMousePosition;
	FVector2D DragOffset;

	FVector2D SelectPos;
	FVector2D CenterPos;

	float V;

	AElementBase* SelectElement;
};

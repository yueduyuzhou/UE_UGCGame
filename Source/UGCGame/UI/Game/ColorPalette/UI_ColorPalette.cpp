// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ColorPalette.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"

void UUI_ColorPalette::NativeConstruct()
{
	Super::NativeConstruct();

	bIsMouseDown = false;

	if (OK && Cancel)
	{
		OK->OnClicked.AddDynamic(this, &UUI_ColorPalette::OnOKButtonClicked);
		Cancel->OnClicked.AddDynamic(this, &UUI_ColorPalette::OnCancelButtonClicked);
	}
}

FReply UUI_ColorPalette::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ColorWheel && ColorWheel->IsHovered())
	{
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

		if (LocalMousePosition.X >= 0.f && LocalMousePosition.X < 300.f &&
			LocalMousePosition.Y >= 0.f && LocalMousePosition.Y < 300.f)
		{
			bIsMouseDown = true;
		}

		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UUI_ColorPalette::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsMouseDown)
	{
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

		if (LocalMousePosition.X >= 0.f && LocalMousePosition.X < 300.f &&
			LocalMousePosition.Y >= 0.f && LocalMousePosition.Y < 300.f)
		{
			Cast<UCanvasPanelSlot>(ColorSelect->Slot)->SetPosition(LocalMousePosition);
		}
		
		return FReply::Handled();
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UUI_ColorPalette::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsMouseDown = false;

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UUI_ColorPalette::OnOKButtonClicked()
{
	//执行OK点击代理
	OKClickDelegate.Broadcast();
};

void UUI_ColorPalette::OnCancelButtonClicked()
{
	//执行Cancel点击代理
	CancelClickDelegate.Broadcast();
};
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ColorPalette.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../../Element/BuildElement.h"
#include "Blueprint/WidgetTree.h"

UUI_ColorPalette::UUI_ColorPalette()
{
	bIsChangeColor = false;
	bIsFollowMouse = false;

	bIsUpdateColor = false;

	LastMousePosition = FVector2D::ZeroVector;
	DragOffset = FVector2D::ZeroVector;

	SelectPos = FVector2D::ZeroVector;
	CenterPos = FVector2D(137.5f, 137.5f);

	V = 1.f;
}

void UUI_ColorPalette::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsUpdateColor)
	{
		//更新SelectColor位置
		if (UCanvasPanelSlot * TmpSlot = Cast<UCanvasPanelSlot>(ColorSelect->Slot))
		{
			SelectPos = TmpSlot->GetPosition();
		}

		FLinearColor TmpColor = GetCurrentColor();
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, FString::Printf(TEXT("[class UUI_ColorPalette]: NativeTick, R = %f, G = %f, B = %f"), TmpColor.R, TmpColor.G, TmpColor.B));
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, FString::Printf(TEXT("[class UUI_ColorPalette]: NativeTick, SelectPos.X = %f, SelectPos.Y = %f"), SelectPos.X, SelectPos.Y));

		//设置展示的颜色
		ShowColor->SetColorAndOpacity(GetCurrentColor());

		//设置Element颜色
		if (ABuildElement * BElement = Cast<ABuildElement>(SelectElement))
		{
			BElement->SetElementMeshColor(GetCurrentColor());
		}
	}
}

void UUI_ColorPalette::NativeConstruct()
{
	Super::NativeConstruct();

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
		bIsChangeColor = true;

		return FReply::Handled();
	}

	if (TopImage && TopImage->IsHovered())
	{
		LastMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		if (UCanvasPanelSlot * CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
		{
			DragOffset = CanvasSlot->GetPosition() - LastMousePosition;
		}
		bIsFollowMouse = true;
	}

	return FReply::Handled();
}

FReply UUI_ColorPalette::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsChangeColor)
	{
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%f : %f"), LocalMousePosition.X, LocalMousePosition.Y));

		//TODO:BUG
		if (LocalMousePosition.X >= 0.f && LocalMousePosition.X < 275.f &&
			LocalMousePosition.Y >= 25.f && LocalMousePosition.Y < 300.f)
		{
			LocalMousePosition.Y -= 25.f;
			//LocalMousePosition += FVector2D(872.f, 400.f);
	
			Cast<UCanvasPanelSlot>(ColorSelect->Slot)->SetPosition(LocalMousePosition);
		}

		return FReply::Handled();
	}

	if (bIsFollowMouse)
	{
		if (UCanvasPanelSlot * CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
		{
			FVector2D NewPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) + DragOffset;
			CanvasSlot->SetPosition(NewPosition);
		}
	}

	return FReply::Handled();
}

FReply UUI_ColorPalette::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsChangeColor = false;
	bIsFollowMouse = false;

	return FReply::Handled();
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

void UUI_ColorPalette::InitColorSelectPosition(const FVector2D& InPosition)
{
	if (UCanvasPanelSlot * TmpSlot = Cast<UCanvasPanelSlot>(ColorSelect->Slot))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("[class UUI_ColorPalette]: InitColorSelectPosition"));
		TmpSlot->SetPosition(InPosition);
	}
}

FLinearColor UUI_ColorPalette::GetCurrentColor()
{
	return UKismetMathLibrary::HSVToRGB(GetH(), GetS(), GetV());
}

float UUI_ColorPalette::GetH()
{
	FVector2D TmpV = UKismetMathLibrary::Normal2D(SelectPos - CenterPos);
	//TmpV += FVector2D(-1.f, 0.f);
	float TmpA = UKismetMathLibrary::DegAtan2(TmpV.Y, TmpV.X);
	return FMath::Fmod(TmpA + 360.f, 360.f);
}

float UUI_ColorPalette::GetS()
{
	//FVector2D TmpV = UKismetMathLibrary::Normal2D();
	float TmpA = UKismetMathLibrary::VSize2D(SelectPos - CenterPos);
	return UKismetMathLibrary::MapRangeClamped(TmpA, 0.f, 150.f, 0.f, 1.f);
}

float UUI_ColorPalette::GetV()
{
	return UKismetMathLibrary::FClamp(V, 0.f, 1.f);
}

void UUI_ColorPalette::StartUpdateColor()
{
	bIsUpdateColor = true;
}

void UUI_ColorPalette::StopUpdateColor()
{
	bIsUpdateColor = false;
}

void UUI_ColorPalette::RegisterElement(AElementBase* InElement)
{
	SelectElement = InElement;
}
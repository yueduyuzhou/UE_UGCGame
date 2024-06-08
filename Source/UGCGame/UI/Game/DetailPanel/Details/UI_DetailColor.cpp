// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DetailColor.h"
#include "Components/Image.h"
#include "../../../../Element/BuildElement.h"
#include "../../../../UGCGamePlayerController.h"
#include "../../../../UGCGameHUD.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../ColorPalette/UI_ColorPalette.h"

void UUI_DetailColor::NativeConstruct()
{
	Super::NativeConstruct();

	if (Color)
	{
		Color->OnMouseButtonDownEvent.BindUFunction(this, TEXT("MouseButtonDown"));
	}
}

void UUI_DetailColor::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Color->SetColorAndOpacity(GetElementColor());
}

void UUI_DetailColor::BindElementProperty(const EEditDetailType& InType, AElementBase* InElement)
{
	RegisterElement(InType, InElement);


}

void UUI_DetailColor::RegisterElement(const EEditDetailType& InType, AElementBase* InElement)
{
	Super::RegisterElement(InType, InElement);
}

void UUI_DetailColor::SetElementColor(FLinearColor NewColor)
{
	if (ABuildElement * BElement = Cast<ABuildElement>(SelectElement))
	{
		BElement->SetElementMeshColor(NewColor);
	}
}

FLinearColor UUI_DetailColor::GetElementColor()
{
	if (ABuildElement * BElement = Cast<ABuildElement>(SelectElement))
	{
		return BElement->GetElementMeshColor();
	}
	return FLinearColor();
}

FVector2D UUI_DetailColor::ColorToPosiotion(float InDis, float Theta)
{
	float ThetaRad = FMath::DegreesToRadians(Theta);

	float Bx = 137.5f + InDis * FMath::Cos(ThetaRad);
	float By = 137.5f + InDis * FMath::Sin(ThetaRad);

	return FVector2D(Bx, By);
}

FEventReply UUI_DetailColor::MouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	OldColor = GetElementColor();

	ColorPalette = CreateWidget<UUI_ColorPalette>(GetWorld(), ColorPaletteClass);
	if (ColorPalette)
	{
		if (AUGCGamePlayerController * MyUGCPC = GetWorld()->GetFirstPlayerController<AUGCGamePlayerController>())
		{
			if (AUGCGameHUD* MyUGCHUD = MyUGCPC->GetHUD<AUGCGameHUD>())
			{
				if (UUI_MainScreen * MainScreen = MyUGCHUD->GetMainScreen())
				{
					MainScreen->AddChildToMainPanel(ColorPalette);
				}
			}
		}

		//绑定按钮代理
		ColorPalette->OKClickDelegate.AddDynamic(this, &UUI_DetailColor::OnOKClick);
		ColorPalette->CancelClickDelegate.AddDynamic(this, &UUI_DetailColor::OnCancelClick);

		//设置SelectColor初始Position为当前颜色对应的Position
		float H, S, V, A;
		UKismetMathLibrary::RGBToHSV(OldColor, H, S, V, A);
		FVector2D TmpPosotion = ColorToPosiotion(S, H);
		ColorPalette->InitColorSelectPosition(TmpPosotion);

		//注册Elemet到调色板
		ColorPalette->RegisterElement(SelectElement);
		//ColorPalette开始更新Element颜色
		ColorPalette->StartUpdateColor();
	}

	return FEventReply();
}

void UUI_DetailColor::OnOKClick()
{
	//停止更新Element颜色
	ColorPalette->StopUpdateColor();
	ColorPalette->RemoveFromParent();
	
}

void UUI_DetailColor::OnCancelClick()
{
	//停止更新Element颜色
	ColorPalette->StopUpdateColor();
	ColorPalette->RemoveFromParent();
	SetElementColor(OldColor);
}
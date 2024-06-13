// Fill out your copyright notice in the Description page of Project Settings.

#include "HypermarketPlayerController.h"
#include "HypermarketCharacterBase.h"
#include "DrawDebugHelpers.h"

AHypermarketPlayerController::AHypermarketPlayerController()
	:bIsRotatingCharacter(false)
	, LastMousePosition(FVector2D::ZeroVector)
	, ControlledActor(nullptr)
{
	bShowMouseCursor = true;

}

void AHypermarketPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AHypermarketPlayerController::OnMouseLeftButtonDown);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AHypermarketPlayerController::OnMouseLeftButtonUp);
	InputComponent->BindAxis("MouseX", this, &AHypermarketPlayerController::OnMouseMoveX);
}

void AHypermarketPlayerController::OnMouseLeftButtonDown()
{
	// 获取鼠标位置
	float LocationX, LocationY;
	if (GetMousePosition(LocationX, LocationY))
	{
		FVector2D ScreenPosition(LocationX, LocationY);

		// 将屏幕位置转换为世界位置和方向
		FVector WorldLocation;
		FVector WorldDirection;
		if (DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection))
		{
			// 设置光线追踪参数
			FVector Start = WorldLocation;
			FVector End = Start + (WorldDirection * 10000.0f); // 光线长度

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this->GetPawn()); // 忽略玩家自身

			// 执行光线追踪
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

			// 如果光线追踪命中了某个Actor
			if (bHit)
			{
				if (HitResult.bBlockingHit)
				{
					AActor* HitActor = HitResult.GetActor();
					if (AHypermarketCharacterBase * HCharacter = Cast<AHypermarketCharacterBase>(HitActor))
					{
						bIsRotatingCharacter = true;
						ControlledActor = HCharacter;
						GetMousePosition(LastMousePosition.X, LastMousePosition.Y);
					}
				}
			}

			// 可选：绘制调试线
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 1.0f);
		}
	}
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	
}

void AHypermarketPlayerController::OnMouseLeftButtonUp()
{
	bIsRotatingCharacter = false;
}

void AHypermarketPlayerController::OnMouseMoveX(float Value)
{
	if (bIsRotatingCharacter && ControlledActor)
	{
		FVector2D CurrentMousePosition;
		GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);

		float DeltaX = CurrentMousePosition.X - LastMousePosition.X;

		FRotator NewRotation = ControlledActor->GetActorRotation();
		NewRotation.Yaw += DeltaX * (-0.2f); // 调整旋转速度

		ControlledActor->SetActorRotation(NewRotation);

		LastMousePosition = CurrentMousePosition;
	}
}

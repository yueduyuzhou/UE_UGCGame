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
	// ��ȡ���λ��
	float LocationX, LocationY;
	if (GetMousePosition(LocationX, LocationY))
	{
		FVector2D ScreenPosition(LocationX, LocationY);

		// ����Ļλ��ת��Ϊ����λ�úͷ���
		FVector WorldLocation;
		FVector WorldDirection;
		if (DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection))
		{
			// ���ù���׷�ٲ���
			FVector Start = WorldLocation;
			FVector End = Start + (WorldDirection * 10000.0f); // ���߳���

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this->GetPawn()); // �����������

			// ִ�й���׷��
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

			// �������׷��������ĳ��Actor
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

			// ��ѡ�����Ƶ�����
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
		NewRotation.Yaw += DeltaX * (-0.2f); // ������ת�ٶ�

		ControlledActor->SetActorRotation(NewRotation);

		LastMousePosition = CurrentMousePosition;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UGCGamePlayerState.h"

AUGCGamePlayerController::AUGCGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AUGCGamePlayerController::GetMouseLocationAndDrection(FVector& OutWorldPosition, FVector& OutWorldDirection)
{
	// ��ȡ���λ��
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);

	// ��Ļλ��ת��Ϊ����ռ�
	FVector WorldLocation, WorldDirection;
	FVector2D ScreenPosition(MousePosition.X, MousePosition.Y);
	if (UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldLocation, WorldDirection))
	{
		OutWorldPosition = WorldLocation;
		OutWorldDirection = WorldDirection;
	}
}

void AUGCGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AUGCGamePlayerController::OnRightMouseButtonDown);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &AUGCGamePlayerController::OnRightMouseButtonUp);
	
	InputComponent->BindAction("DeleteButton", IE_Released, this, &AUGCGamePlayerController::OnDeleteButtonUp);
}

void AUGCGamePlayerController::OnRightMouseButtonDown()
{
	//�����ڷ������ϻ�ȡ��Element�Ŀ���
	if (AUGCGamePlayerState * MyPlayerSatte = GetPlayerState<AUGCGamePlayerState>())
	{
		FVector TraceStart, Direction;
		GetMouseLocationAndDrection(TraceStart, Direction);
		MyPlayerSatte->TryGetElementControlOnServer(TraceStart, Direction);
	}
}

void AUGCGamePlayerController::OnRightMouseButtonUp()
{
	//�ڷ������ϻ�黹Element�Ŀ���Ȩ
	if (AUGCGamePlayerState * MyPlayerSatte = GetPlayerState<AUGCGamePlayerState>())
	{
		MyPlayerSatte->TryReturnElementControlOnServer();
	}
}

void AUGCGamePlayerController::OnDeleteButtonUp()
{
	//֪ͨ������ɾ�������е�Element
	if (AUGCGamePlayerState * MyPlayerState = GetPlayerState<AUGCGamePlayerState>())
	{
		MyPlayerState->TryDeleteControlElementOnServer();
	}
}

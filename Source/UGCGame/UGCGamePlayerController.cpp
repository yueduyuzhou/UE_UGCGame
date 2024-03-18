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
	// 获取鼠标位置
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);

	// 屏幕位置转换为世界空间
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
	//尝试在服务器上获取该Element的控制
	if (AUGCGamePlayerState * MyPlayerSatte = GetPlayerState<AUGCGamePlayerState>())
	{
		FVector TraceStart, Direction;
		GetMouseLocationAndDrection(TraceStart, Direction);
		MyPlayerSatte->TryGetElementControlOnServer(TraceStart, Direction);
	}
}

void AUGCGamePlayerController::OnRightMouseButtonUp()
{
	//在服务器上获归还Element的控制权
	if (AUGCGamePlayerState * MyPlayerSatte = GetPlayerState<AUGCGamePlayerState>())
	{
		MyPlayerSatte->TryReturnElementControlOnServer();
	}
}

void AUGCGamePlayerController::OnDeleteButtonUp()
{
	//通知服务器删除控制中的Element
	if (AUGCGamePlayerState * MyPlayerState = GetPlayerState<AUGCGamePlayerState>())
	{
		MyPlayerState->TryDeleteControlElementOnServer();
	}
}

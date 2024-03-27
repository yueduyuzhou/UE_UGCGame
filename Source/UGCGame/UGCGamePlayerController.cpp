// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UGCGamePlayerState.h"
#include "ThreadManage.h"
#include "UGCGamePawn.h"
#include "UGCGamePlayerState.h"
#include "Common/MethodUnit.h"
//#include "Sockets.h"
//#include "SocketSubsystem.h"
//#include "Interfaces/IPv4/IPv4Address.h"

AUGCGamePlayerController::AUGCGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AUGCGamePlayerController::GetMouseLocationAndDrection(FVector& OutWorldPosition, FVector& OutWorldDirection)
{
	//获取鼠标位置
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);

	//屏幕位置转换为世界空间
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
	
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AUGCGamePlayerController::OnLeftMouseButtonDown);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AUGCGamePlayerController::OnLeftMouseButtonUp);

	InputComponent->BindAction("DeleteButton", IE_Released, this, &AUGCGamePlayerController::OnDeleteButtonUp);

	InputComponent->BindAction("TransformationForTranslation", IE_Released, this, &AUGCGamePlayerController::TransformationForTranslation);
	InputComponent->BindAction("TransformationForRotation", IE_Released, this, &AUGCGamePlayerController::TransformationForRotation);
	InputComponent->BindAction("TransformationForScale", IE_Released, this, &AUGCGamePlayerController::TransformationForScale);

	InputComponent->BindAxis("MoveForward", this, &AUGCGamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUGCGamePlayerController::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &AUGCGamePlayerController::MoveUp);
	InputComponent->BindAxis("Turn", this, &AUGCGamePlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &AUGCGamePlayerController::LookUpAtRate);
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

void AUGCGamePlayerController::OnLeftMouseButtonDown()
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->OnLeftMousePressed();
	}
}

void AUGCGamePlayerController::OnLeftMouseButtonUp()
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->OnLeftMouseReleassed();
	}
}

void AUGCGamePlayerController::OnDeleteButtonUp()
{
	if (AUGCGamePlayerState* MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		MyPlayerState->TryDeleteControlElementOnServer();
	}
}

void AUGCGamePlayerController::MoveForward(float Value)
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->MoveForward(Value);
	}
}

void AUGCGamePlayerController::MoveRight(float Value)
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->MoveRight(Value);
	}
}

void AUGCGamePlayerController::MoveUp(float Value)
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->MoveUp(Value);
	}
}

void AUGCGamePlayerController::TurnAtRate(float Rate)
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->TurnAtRate(Rate);
	}
}

void AUGCGamePlayerController::LookUpAtRate(float Rate)
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->LookUpAtRate(Rate);
	}
}

void AUGCGamePlayerController::TransformationForTranslation()
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->TransformationForTranslation();
	}
}

void AUGCGamePlayerController::TransformationForRotation()
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->TransformationForRotation();
	}
}

void AUGCGamePlayerController::TransformationForScale()
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->TransformationForScale();
	}
}
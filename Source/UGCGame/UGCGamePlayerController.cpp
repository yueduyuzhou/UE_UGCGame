// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UGCGamePlayerState.h"
#include "ThreadManage.h"
#include "UGCGamePawn.h"
#include "UGCGamePlayerState.h"
#include "Common/MethodUnit.h"
#include "UGCGameState.h"
//#include "Sockets.h"
//#include "SocketSubsystem.h"
//#include "Interfaces/IPv4/IPv4Address.h"

AUGCGamePlayerController::AUGCGamePlayerController()
	:bIsEditing(true)
	, bIsEditingDetail(false)
	, AngleRotationRate(1.0f)
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

void AUGCGamePlayerController::EnableUIOnlyInput(UUserWidget* InWidgetToFocus)
{
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(InWidgetToFocus->TakeWidget());
	//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
}

void AUGCGamePlayerController::EnableGameAndUIInput()
{
	FInputModeGameAndUI InputMode;

	SetInputMode(InputMode);
}

void AUGCGamePlayerController::SetEditingDetail(bool inbIsEditingDetail)
{
	bIsEditingDetail = inbIsEditingDetail;
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("[class AUGCGamePlayerController]: SetEditingDetail, bIsEditingDetail = %d"), bIsEditingDetail));
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

	InputComponent->BindAction("MultiSelect", IE_Released, this, &AUGCGamePlayerController::MultiSelect);
	InputComponent->BindAction("DeMultiSelect", IE_Released, this, &AUGCGamePlayerController::DeMultiSelect);

	InputComponent->BindAxis("MoveForward", this, &AUGCGamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUGCGamePlayerController::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &AUGCGamePlayerController::MoveUp);
	InputComponent->BindAxis("Turn", this, &AUGCGamePlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &AUGCGamePlayerController::LookUpAtRate);
	InputComponent->BindAxis("CameraView", this, &AUGCGamePlayerController::MouseWheelCameraView);
}

void AUGCGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld * CurWorld = GetWorld())
	{
		if (CurWorld->WorldType == EWorldType::Game)			//Standalone
		{
			AngleRotationRate = 8.f;
		}
		else if (CurWorld->WorldType == EWorldType::PIE)		//New Editor Window
		{
			AngleRotationRate = 1.f;
		}
	}
}

void AUGCGamePlayerController::OnRightMouseButtonDown()
{
	bIsEditing = false;
	//尝试在服务器上获取该Element的控制
	/*if (AUGCGamePlayerState * MyPlayerSatte = GetPlayerState<AUGCGamePlayerState>())
	{
		FVector TraceStart, Direction;
		GetMouseLocationAndDrection(TraceStart, Direction);
		MyPlayerSatte->TryGetElementControlOnServer(TraceStart, Direction);
	}*/

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString(TEXT("[class AUGCGamePlayerController]: OnRightMouseButtonDown")));
}

void AUGCGamePlayerController::OnRightMouseButtonUp()
{
	bIsEditing = true;
	//归还Element的控制权
	if (AUGCGamePlayerState * MyPlayerSatte = GetPlayerState<AUGCGamePlayerState>())
	{
		MyPlayerSatte->TryReturnElementControl();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString(TEXT("[class AUGCGamePlayerController]: OnRightMouseButtonUp, Call TryReturnElementControl")));
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
	if (!bIsEditingDetail)
	{
		if (AUGCGameState * MyGameState = MethodUnit::GetGameState(GetWorld()))
		{
			if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
			{
				TArray<AElementBase*> Elems = MyPlayerPawn->GetSelectedElement();
				for (auto* Tmp : Elems)
				{
					MyGameState->SubSpawnData(Tmp->GetElementID());
				}
				MyPlayerPawn->ServerDeselectAll(true);
			}
		}
	}
}

void AUGCGamePlayerController::MoveForward(float Value)
{
	if (!IsEditing())
	{
		if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
		{
			MyPlayerPawn->MoveForward(Value);
		}
	}
}

void AUGCGamePlayerController::MoveRight(float Value)
{
	if (!IsEditing())
	{
		if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
		{
			MyPlayerPawn->MoveRight(Value);
		}
	}
}

void AUGCGamePlayerController::MoveUp(float Value)
{
	if (!IsEditing())
	{
		if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
		{
			MyPlayerPawn->MoveUp(Value);
		}
	}
}

void AUGCGamePlayerController::TurnAtRate(float Rate)
{
	if (!IsEditing())
	{
		if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
		{
			MyPlayerPawn->TurnAtRate(Rate * AngleRotationRate);
		}
	}
}

void AUGCGamePlayerController::LookUpAtRate(float Rate)
{
	if (!IsEditing())
	{
		if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
		{
			MyPlayerPawn->LookUpAtRate(Rate * AngleRotationRate);
		}
	}
}

void AUGCGamePlayerController::TransformationForTranslation()
{
	if (IsEditing())
	{
		if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
		{
			MyPlayerPawn->TransformationForTranslation();
		}
	}
}

void AUGCGamePlayerController::TransformationForRotation()
{
	if (IsEditing())
	{
		if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
		{
			MyPlayerPawn->TransformationForRotation();
		}
	}
}

void AUGCGamePlayerController::TransformationForScale()
{
	if (IsEditing())
	{
		if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
		{
			MyPlayerPawn->TransformationForScale();
		}
	}
}

void AUGCGamePlayerController::MultiSelect()
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->MultiSelect();
	}
}

void AUGCGamePlayerController::DeMultiSelect()
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->DeMultiSelect();
	}
}

void AUGCGamePlayerController::MouseWheelCameraView(float Value)
{
	if (AUGCGamePawn * MyPlayerPawn = GetPawn<AUGCGamePawn>())
	{
		MyPlayerPawn->MouseWheelCameraView(Value);
	}
}

void AUGCGamePlayerController::CaptureLobbyTexture(const FString& InMapName)
{
	LobbyTextureCapture.Initialize(GetWorld(), FVector(-1570.f, -1445.f, 100.f), FRotator(0.f, 45.f, 0.f), ECameraProjectionMode::Type::Perspective, FVector2D(2560.f, 1600.f));
	FString SavePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("LobbyTexture"), InMapName);
	LobbyTextureCapture.CaptureMiniMapImage(SavePath);
}
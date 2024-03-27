// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementBase.h"
#include "Net/UnrealNetwork.h"
#include "../Common/MethodUnit.h"
#include "../Common/UGCGameType.h"
#include "../UGCGamePlayerController.h"

// Sets default values
AElementBase::AElementBase()
	:bControlled(false),
	ControllerID(INDEX_NONE)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AElementBase::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called every frame
void AElementBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		if (MyPlayerState->TPlayerID == ControllerID)
		{
			if (AUGCGamePlayerController * MyPlayerController = MethodUnit::GetPlayerControllerByPlayerID(GetWorld(), ControllerID))
			{
				if (MyPlayerState->GetModifyType() == EElementModifyType::MODIFY_LOCATION)
				{
					FVector TraceStart, Direction;
					MyPlayerController->GetMouseLocationAndDrection(TraceStart, Direction);
					MyPlayerState->UpdateElementLocationOnServer(TraceStart, Direction);
				}
				else if(MyPlayerState->GetModifyType() == EElementModifyType::MODIFY_ROTATION)
				{
					float RotationX, RotationY;
					MyPlayerController->GetInputMouseDelta(RotationX, RotationY);
					//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%f %f"), RotationX, RotationY));
					MyPlayerState->UpdateElementRotationOnServer(RotationX, RotationY);
				}
			}
		}
	}
}

void AElementBase::SetElementID(const int32& InElementID)
{
	ID = InElementID;
}

void AElementBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AElementBase, ControllerID);
}

bool AElementBase::TakeControl(const int32& InControlID)
{
	if (InControlID != INDEX_NONE)
	{
		ControllerID = InControlID;
		bControlled = true;

		return true;
	}
	return false;
}

void AElementBase::ReturnControl()
{
	ControllerID = INDEX_NONE;
	bControlled = false;
}

void AElementBase::DestoryElement()
{
	Destroy();
}


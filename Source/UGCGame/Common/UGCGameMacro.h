// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define UPDATE_OLD_LOCATION(InDime)\
if (InElement)\
{\
	FVector OldLocation = InElement->GetActorLocation();\
	OldLocation.InDime = InValue;\
	InElement->SetActorLocation(OldLocation);\
}

#define UPDATE_OLD_ROTATION(InDime)\
if (InElement)\
{\
	FRotator OldRotation = InElement->GetActorRotation();\
	OldRotation.InDime = InValue;\
	InElement->SetActorRotation(OldRotation);\
}

#define UPDATE_OLD_SCALE(InDime)\
if (InElement)\
{\
	FVector OldScale = InElement->GetActorScale3D();\
	OldScale.InDime = InValue;\
	InElement->SetActorScale3D(OldScale);\
}

#define BIND_DELEGATE_SCALE(InDelegate, InDime)\
ParentDetailVector->InDelegate.BindLambda([](const float InValue, AElementBase * InElement)\
{UPDATE_OLD_SCALE(InDime)});

#define BIND_DELEGATE_ROTATION(InDelegate, InDime)\
ParentDetailVector->InDelegate.BindLambda([](const float InValue, AElementBase * InElement)\
{UPDATE_OLD_ROTATION(InDime)});

#define BIND_DELEGATE_LOCATION(InDelegate, InDime)\
ParentDetailVector->InDelegate.BindLambda([](const float InValue, AElementBase * InElement)\
{UPDATE_OLD_LOCATION(InDime)});
// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildElement.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

ABuildElement::ABuildElement()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	ElementMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElementMesh"));
	EffectCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EffectCollision"));

	SetRootComponent(RootScene);
	ElementMesh->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
	EffectCollision->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABuildElement::SetElementMesh(UStaticMesh* NewMesh)
{
	ElementMesh->SetStaticMesh(NewMesh);
}

void ABuildElement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(ABuildElement, ElementMesh, COND_None);
}
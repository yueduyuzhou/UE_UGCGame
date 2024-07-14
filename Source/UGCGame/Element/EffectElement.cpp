// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectElement.h"
//D:\DownLoad\UnrealEngine\UESourceCode\4.25\Engine\Source\Runtime\Engine\Classes\Engine\StaticMesh.h

AEffectElement::AEffectElement()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	VisiableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisiableMesh"));

	SetRootComponent(RootScene);
	VisiableMesh->SetupAttachment(RootComponent);
}

void AEffectElement::SetElementMeshVisibility(bool InbIsShow)
{
	VisiableMesh->SetVisibility(InbIsShow);
}


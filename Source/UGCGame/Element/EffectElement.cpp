// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectElement.h"
//D:\DownLoad\UnrealEngine\UESourceCode\4.25\Engine\Source\Runtime\Engine\Classes\Engine\StaticMesh.h

AEffectElement::AEffectElement()
{
	VisiableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisiableMesh"));
	VisiableMesh->SetupAttachment(RootComponent);
}

void AEffectElement::SetNotVisibilityMulticast_Implementation()
{
	VisiableMesh->SetVisibility(false);
}

void AEffectElement::MulticastDestroy_Implementation()
{
	Destroy();
}

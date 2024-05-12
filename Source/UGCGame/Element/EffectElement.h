// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementBase.h"
#include "EffectElement.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API AEffectElement : public AElementBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisiableMesh;

public:
	AEffectElement();

	UFUNCTION(NetMulticast, reliable)
	void SetNotVisibilityMulticast();

public:
	UFUNCTION(NetMulticast, reliable)
		void MulticastDestroy();
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* VisiableMesh;

public:
	AEffectElement();

	void SetElementMeshVisibility(bool InbIsShow);
};

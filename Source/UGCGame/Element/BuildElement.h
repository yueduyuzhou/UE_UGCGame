// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementBase.h"
#include "BuildElement.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API ABuildElement : public AElementBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ElementMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;

	/*Åö×²ºÐ×Ó*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* EffectCollision;

public:
	ABuildElement();

	void SetElementMesh(UStaticMesh* NewMesh);

	virtual void SetElementMeshColor(FLinearColor NewColor) override;

	virtual FLinearColor GetElementMeshColor() override;

	UFUNCTION(NetMulticast, reliable)
		void SetMeshColorMulticast(const FLinearColor& NewColor);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

};

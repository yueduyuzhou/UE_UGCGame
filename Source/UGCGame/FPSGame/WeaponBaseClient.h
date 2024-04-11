// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBaseClient.generated.h"

class USkeletalMeshComponent;

UCLASS()
class UGCGAME_API AWeaponBaseClient : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponMesh;

public:	
	AWeaponBaseClient();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

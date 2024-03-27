// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "TransformerPawn.h"
#include "ElementBase.generated.h"

UCLASS()
class UGCGAME_API AElementBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AElementBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetMouseLocationInWorld();

public:
	bool TakeControl(const int32& InControlID);
	void ReturnControl();
	void DestoryElement();

public:
	FORCEINLINE const int32 GetElementID() { return ID; }
	void SetElementID(const int32& InElementID);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:
	bool bControlled;

	UPROPERTY(Replicated)
	int32 ControllerID;

private:
	int32 ID;


};

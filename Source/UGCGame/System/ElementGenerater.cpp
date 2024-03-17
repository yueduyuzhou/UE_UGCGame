// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGenerater.h"
#include "../Common/MethodUnit.h"


// Sets default values
AElementGenerater::AElementGenerater()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AElementGenerater::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AElementGenerater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

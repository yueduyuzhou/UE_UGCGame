// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UGCGameHUD.generated.h"

class UUI_MainScreen;

/**
 * 
 */
UCLASS()
class UGCGAME_API AUGCGameHUD : public AHUD
{
	GENERATED_BODY()

	AUGCGameHUD();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	UUI_MainScreen* GetMainScreen();

private:
	TSubclassOf<UUI_MainScreen> MainScreenClass;
	UUI_MainScreen* MainScreen;
};

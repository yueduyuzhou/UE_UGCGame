// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HypermarketHUD.generated.h"

class UUI_MainScreen;

/**
 * 
 */
UCLASS()
class UGCGAME_API AHypermarketHUD : public AHUD
{
	GENERATED_BODY()
	
	AHypermarketHUD();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		UUI_MainScreen* GetMainScreen();

	TArray<UWidget*> GetMainScreenChildrens();

private:
	TSubclassOf<UUI_MainScreen> MainScreenClass;
	UUI_MainScreen* MainScreen;
	
};

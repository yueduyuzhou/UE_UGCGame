// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSGameHUD.generated.h"

class UUI_FPSMainScreen;


/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSGameHUD : public AHUD
{
	GENERATED_BODY()
	
	AFPSGameHUD();

	virtual void BeginPlay() override;

private:
	TSubclassOf<UUI_FPSMainScreen> MainScreenClass;
	UUI_FPSMainScreen* MainScreen;

};

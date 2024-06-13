// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class UUI_MainScreen;

/**
 * 
 */
UCLASS()
class UGCGAME_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	ALobbyHUD();

	virtual void BeginPlay() override;

public:
	TArray<UWidget*> GetMainScreenChildrens();

private:
	TSubclassOf<UUI_MainScreen> MainScreenClass;
	UUI_MainScreen* MainScreen;
};

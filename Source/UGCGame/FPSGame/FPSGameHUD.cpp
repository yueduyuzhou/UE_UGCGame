// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameHUD.h"
#include "UI/UI_FPSMainScreen.h"

AFPSGameHUD::AFPSGameHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_FPSMainScreen> UIMainScreen_Class(TEXT("/Game/BP/FPS/UI/BP_FPSMainScreen"));
	MainScreenClass = UIMainScreen_Class.Class;
}

void AFPSGameHUD::BeginPlay()
{
	Super::BeginPlay();

	//Ìí¼ÓÆÁÄ»UI
	if (MainScreenClass)
	{
		MainScreen = CreateWidget<UUI_FPSMainScreen>(GetWorld(), MainScreenClass);
		MainScreen->AddToPlayerScreen(2);
	}
}

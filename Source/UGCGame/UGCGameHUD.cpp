// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGameHUD.h"
#include "UI/UI_MainScreen.h"

AUGCGameHUD::AUGCGameHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_MainScreen> UIMainScreen_Class(TEXT("/Game/UI/UGC/BP_MainScreen"));
	MainScreenClass = UIMainScreen_Class.Class;
}

void AUGCGameHUD::BeginPlay()
{
	Super::BeginPlay();

	//Ìí¼ÓÆÁÄ»UI
	if (MainScreenClass)
	{
		MainScreen = CreateWidget<UUI_MainScreen>(GetWorld(), MainScreenClass);
		MainScreen->AddToPlayerScreen(2);
	}
}

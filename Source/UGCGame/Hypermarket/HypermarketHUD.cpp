// Fill out your copyright notice in the Description page of Project Settings.


#include "HypermarketHUD.h"
#include "UGCGame/UI/UI_MainScreen.h"

AHypermarketHUD::AHypermarketHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_MainScreen> UIMainScreen_Class(TEXT("/Game/UI/Character/WBP_HypermarketMainScreen"));
	MainScreenClass = UIMainScreen_Class.Class;
}

void AHypermarketHUD::BeginPlay()
{
	Super::BeginPlay();

	//Ìí¼ÓÆÁÄ»UI
	if (MainScreenClass)
	{
		MainScreen = CreateWidget<UUI_MainScreen>(GetWorld(), MainScreenClass);
		MainScreen->AddToPlayerScreen(2);
	}
}

UUI_MainScreen* AHypermarketHUD::GetMainScreen()
{
	return MainScreen;
}

TArray<UWidget*> AHypermarketHUD::GetMainScreenChildrens()
{
	if (MainScreen)
	{
		return MainScreen->GetMainPanelAllChildren();
	}
	return TArray<UWidget*>();
}

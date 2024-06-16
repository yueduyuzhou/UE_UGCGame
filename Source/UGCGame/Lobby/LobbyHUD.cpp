// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyHUD.h"
#include "../UI/UI_MainScreen.h"

ALobbyHUD::ALobbyHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_MainScreen> UIMainScreen_Class(TEXT("/Game/UI/Lobby/BP_LobbyMainScreen"));
	MainScreenClass = UIMainScreen_Class.Class;
}

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainScreenClass)
	{
		MainScreen = CreateWidget<UUI_MainScreen>(GetWorld(), MainScreenClass);
		MainScreen->AddToPlayerScreen(2);
	}
}

TArray<UWidget*> ALobbyHUD::GetMainScreenChildrens()
{
	if (MainScreen)
	{
		return (MainScreen->GetMainPanelAllChildren());
	}
	return TArray<UWidget*>();
}

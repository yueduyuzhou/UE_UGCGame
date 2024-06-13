// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MainScreen.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UUI_MainScreen::AddChildToMainPanel(UWidget* InChild)
{
	if (UCanvasPanelSlot * TmpSlot = MainPanel->AddChildToCanvas(InChild))
	{
		TmpSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
	}

}

TArray<UWidget*> UUI_MainScreen::GetMainPanelAllChildren()
{
	if (MainPanel)
	{
		return MainPanel->GetAllChildren();
	}
	return TArray<UWidget*>();
}

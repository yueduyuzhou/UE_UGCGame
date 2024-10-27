// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/Core/UI_Base.h"
#include "UI_MainScreen.generated.h"

class UCanvasPanel;
class UUI_QuitPanel;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_MainScreen : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
		UCanvasPanel* MainPanel;
	
	//UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	//	UUI_QuitPanel* QuitPanel;

public:
	void AddChildToMainPanel(UWidget* InChild);
	TArray<UWidget*> GetMainPanelAllChildren();
};

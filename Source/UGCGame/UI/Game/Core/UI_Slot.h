// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_Slot.generated.h"

class UImage;
class UButton;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Slot : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UButton* ClickButton;

	UPROPERTY(meta = (BindWidget))
		UImage* Icon;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void OnClickedWidget() {}
	UFUNCTION(BlueprintCallable)
	virtual void OnReleasedClickedWidget() {}
};

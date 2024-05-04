// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UGCGame/UI/Core/UI_Base.h"
#include "UI_NetSetting.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_NetSetting : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SetName;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SetValue;

	UPROPERTY(meta = (BindWidget))
		UButton* LeftButton;

	UPROPERTY(meta = (BindWidget))
		UButton* RightButton;

	UFUNCTION()
	void LeftButtonClick();
	UFUNCTION()
	void RightButtonClick();

	void UpdateSetValue();

protected:
	virtual void NativeConstruct();

public:
	UFUNCTION(BlueprintCallable)
	FText GetCurrentValue();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Index;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FText> SetValues;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FText SettingName;
};

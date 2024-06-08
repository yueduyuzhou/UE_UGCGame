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

DECLARE_DELEGATE(FNetSettingDelegate);

UCLASS()
class UGCGAME_API UUI_NetSetting : public UUI_Base
{
	friend class UUI_PlayerList;

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

	void SetValues(const TArray<FString>& InValues);

public:
	/************************************
	*	Delegate
	************************************/
	FNetSettingDelegate InitValueDelegate;
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Index;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FText> Values;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FText SettingName;
};

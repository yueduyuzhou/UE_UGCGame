// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UGCGame/UI/Core/UI_Base.h"
#include "UI_ChatFrame.generated.h"

class UVerticalBox;
class UButton;
class UEditableText;
class UUI_MassageText;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_ChatFrame : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UVerticalBox* MassageContent;

	UPROPERTY(meta = (BindWidget))
		UButton* SendButton;

	UPROPERTY(meta = (BindWidget))
		UEditableText* InputMassage;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUI_MassageText> MassageTextClass;

protected:
	virtual void NativeConstruct() override;

public:
	void AddMassageToContent(const FString& InMsg);

private:
	UFUNCTION()
	void SendButtonClick();
};

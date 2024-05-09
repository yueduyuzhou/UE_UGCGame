// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UGCGame/UI/Core/UI_Base.h"
#include "UI_MassageText.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_MassageText : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Player;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Massage;

public:
	void SetPlayer(const FString& InPlayer);
	void SetMassage(const FString& InMsg);

};

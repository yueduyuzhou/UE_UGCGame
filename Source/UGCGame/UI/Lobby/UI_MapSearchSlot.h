// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_MapSlot.h"
#include "FindSessionsCallbackProxy.h"
#include "UI_MapSearchSlot.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_MapSearchSlot : public UUI_MapSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PingMs;

	UPROPERTY(BlueprintReadOnly)
	FBlueprintSessionResult SessionRes;

public:
	virtual void OnClickedWidget();

public:
	void SetInfo(FBlueprintSessionResult InSessionRes);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void JoinSessionBP();

protected:
	virtual void NativeConstruct();
};

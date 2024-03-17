// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Base.generated.h"

class AUGCGamePlayerState;
class AUGCGameState;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();

public:
	AUGCGamePlayerState* GetPlayerState();
	AUGCGameState* GetGameState();

	void SetElementID(const int32& InElementID);
	FORCEINLINE const int32 GetElementID() { return ElementID; }

private:
	int32 ElementID;
};

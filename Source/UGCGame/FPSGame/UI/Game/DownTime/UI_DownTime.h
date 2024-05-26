// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_FPSBase.h"
#include "UI_DownTime.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_DownTime : public UUI_FPSBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Time;

	UUI_DownTime();

public:
	void StartDownTime(const float& InDownTime);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void RegisterToPlayerController();

private:
	bool bStartDownTime;
	float DownTime;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_FPSBase.h"
#include "UI_MiniMap.generated.h"

class AFPSGamePlayerController;
class UImage;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_MiniMap : public UUI_FPSBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* MapImage;

	UPROPERTY(meta = (BindWidget))
		UImage* PlayerArrow;

	void RegisterToPlayerController();

	UUI_MiniMap();

public:
	void UpdateMiniMapImage(UTexture2D* InTexture);

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	AFPSGamePlayerController* LocalPlayerController;
};

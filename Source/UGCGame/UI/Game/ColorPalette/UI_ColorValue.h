// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UGCGame/Common/UGCGameType.h"
#include "UI_ColorValue.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_ColorValue : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		class UEditableText* Value;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		class UProgressBar* ColorValueBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HSVName;

	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateValue(float InVal);

private:
	UPROPERTY(EditAnywhere)
	EHSVType HSVType;

	float MaxValue;
};

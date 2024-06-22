// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_DetailPanelSlot.generated.h"

class UCanvasPanel;
class UUI_DetailsPanel;
class UUI_DetailVector;
class UUI_DetailEnum;
class UUI_DetailColor;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_DetailPanelSlot : public UUI_Base
{
	GENERATED_BODY()

	friend class UUI_DetailsPanel;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUI_DetailVector> DetailVectorClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUI_DetailEnum> DetailEnumClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUI_DetailColor> DetailColorClass;

	UUI_DetailPanelSlot();

public:
	void UpdateDetailChild(const EEditDetailType& InType, AElementBase* InElement);

private:
	AElementBase* SelectElement;
};

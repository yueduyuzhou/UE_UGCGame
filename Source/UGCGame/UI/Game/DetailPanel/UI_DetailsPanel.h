// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_DetailsPanel.generated.h"

class AElementBase;
class UVerticalBox;
class UUI_DetailPanelSlot;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_DetailsPanel : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* Details;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUI_DetailPanelSlot> DetailPanelSlotClass;

protected:
	virtual void NativeConstruct();

public:
	void UpdateDetailsPanel();

	void SetSelectElement(AElementBase* InElement);

private:
	AElementBase* SelectElement;
};

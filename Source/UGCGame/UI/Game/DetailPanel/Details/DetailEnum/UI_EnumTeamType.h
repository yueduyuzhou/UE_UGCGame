// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_DetailEnum.h"
#include "UI_EnumTeamType.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_EnumTeamType : public UUI_DetailEnum
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();

	virtual void PopulateComboBox() override;

	void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

public:
	virtual void SetEnumName() override;
};

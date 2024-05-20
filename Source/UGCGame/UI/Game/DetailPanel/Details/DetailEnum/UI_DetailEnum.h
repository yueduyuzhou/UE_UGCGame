// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../UI_DetailsBase.h"
#include "UI_DetailEnum.generated.h"

class UTextBlock;
class UComboBoxString;
class AElementBase;
enum class EEditDetailType : uint8;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_DetailEnum : public UUI_DetailsBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Name;

	UPROPERTY(meta = (BindWidget))
		UComboBoxString* EnumSelect;

	UPROPERTY(EditAnywhere)
		TArray<FString> Options;

protected:
	virtual void NativeConstruct();

	/*****************
	*	Subclass
	*****************/
	virtual void PopulateComboBox();

	UFUNCTION()
		virtual void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

public:
	virtual void SetEnumName();

	virtual void BindElementProperty(const EEditDetailType& InType, AElementBase* InElement) override;

	virtual void RegisterElement(const EEditDetailType& InType, AElementBase* InElement) override;
};

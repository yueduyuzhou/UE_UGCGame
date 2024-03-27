// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "../../../Common/UGCGameType.h"
#include "UI_Item.generated.h"

class UImage;
class UEditableText;
class UCheckBox;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_Item : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* Icon;

	UPROPERTY(meta = (BindWidget))
		UEditableText* InputText;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* UsedCheckBox;

	UPROPERTY(EditDefaultsOnly)
		EElementModifyType ModifyType;

public:
	void SetModifyType(const EElementModifyType& InModifyType);

protected:
	virtual void NativeConstruct();

private:
	UFUNCTION(BlueprintCallable)
	void OnTextCommit(const FText& InText, ETextCommit::Type InCommitMethod);

	UFUNCTION(BlueprintCallable)
	void OnCheckedStateChange(bool InbIsChecked);
};

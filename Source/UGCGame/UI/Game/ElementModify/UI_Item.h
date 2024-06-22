// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "../../../Common/UGCGameType.h"
#include "RuntimeTransformer/Public/TransformerPawn.h"
#include "UI_Item.generated.h"

class UImage;
class UCheckBox;
class UEditableText;
class UUI_DetailVector;
class FDelegateHandle;
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
		ETransformationType ModifyType;
	
	UPROPERTY(EditAnywhere)
		EVectorDime Dime;

	UUI_Item();

public:
	void SetModifyType(const ETransformationType& InModifyType);
	void SetUsedCheckState(const ECheckBoxState& InCheckedState);
	void SetIsSnappingValue(bool InIsSnappingValue);
	void SetVectorText(const float InValue);
	
	void RegisterParentDetailVector(UUI_DetailVector* InParent);

	void BindProperty(const EEditDetailType& InType, AElementBase* InElement);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION(BlueprintCallable)
	void OnTextCommit(const FText& InText, ETextCommit::Type InCommitMethod);

	UFUNCTION(BlueprintCallable)
	void OnCheckedStateChange(bool InbIsChecked);

	void BindDelegate();

private:
	/*为顶部面板时为true*/
	bool bIsSnappingValue;

	UUI_DetailVector* ParentDetailVector;

	FDelegateHandle ChangeModifyTypeDelegateHandle;
};

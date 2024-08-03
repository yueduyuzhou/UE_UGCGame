// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Item.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "../../../UGCGamePawn.h"
#include "../../../Common/MethodUnit.h"
#include "../../../Common/UGCGameMacro.h"
#include "../DetailPanel/Details/UI_DetailVector.h"
#include "UGCGame/Element/ElementBase.h"
#include "Delegates/IDelegateInstance.h"

UUI_Item::UUI_Item()
	:bIsSnappingValue(true)
	, ParentDetailVector(nullptr)
{
}

void UUI_Item::SetModifyType(const ETransformationType& InModifyType)
{
	ModifyType = InModifyType;
}

void UUI_Item::SetUsedCheckState(const ECheckBoxState& InCheckedState)
{
	UsedCheckBox->SetCheckedState(InCheckedState);
}

void UUI_Item::SetIsSnappingValue(bool InIsSnappingValue)
{
	UE_LOG(LogTemp, Display, TEXT("[class UUI_Item]: SetIsSnappingValue, InIsSnappingValue = %d"), InIsSnappingValue);
	bIsSnappingValue = InIsSnappingValue;
	if (!InIsSnappingValue)
	{
		UsedCheckBox->SetIsEnabled(false);
	}
}

void UUI_Item::SetVectorText(const float InValue)
{
	if (InputText)
	{
		FString ValueString = FString::SanitizeFloat(InValue);
		FText ValueText = FText::FromString(ValueString);
		InputText->SetHintText(ValueText);
	}
}

void UUI_Item::RegisterParentDetailVector(UUI_DetailVector* InParent)
{
	if (InParent)
	{
		UE_LOG(LogTemp, Display, TEXT("[class UUI_Item]: RegisterParentDetailVector, InParent = %s"), *InParent->GetName());
		ParentDetailVector = InParent;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_Item]: SetIsSnappingValue, InParent Is Null"));
	}
}

void UUI_Item::BindProperty(const EEditDetailType& InType, AElementBase* InElement)
{
	if (!InElement)
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_Item]: Element Is NULL"));
		return;
	}

	if (ParentDetailVector)
	{
		switch (InType)
		{
			case EEditDetailType::DETAIL_LOCATION:
			{
				if (Dime == EVectorDime::X)
				{
					BIND_DELEGATE_LOCATION(BindElementLocationZDelegate, X)
				}
				else if (Dime == EVectorDime::Y)
				{
					BIND_DELEGATE_LOCATION(BindElementLocationZDelegate, Y)
				}
				else if (Dime == EVectorDime::Z)
				{
					BIND_DELEGATE_LOCATION(BindElementLocationZDelegate, Z)
				}
				break;
			}
			case EEditDetailType::DETAIL_ROTATION:
			{
				if (Dime == EVectorDime::X)
				{
					BIND_DELEGATE_ROTATION(BindElementRotationXDelegate, Roll)
				}
				else if (Dime == EVectorDime::Y)
				{
					BIND_DELEGATE_ROTATION(BindElementRotationYDelegate, Pitch)
				}
				else if (Dime == EVectorDime::Z)
				{
					BIND_DELEGATE_ROTATION(BindElementRotationZDelegate, Yaw)
				}
				break;
			}
			case EEditDetailType::DETAIL_SCALE:
			{
				if (Dime == EVectorDime::X)
				{
					BIND_DELEGATE_SCALE(BindElementScaleXDelegate, X)
				}
				else if (Dime == EVectorDime::Y)
				{
					BIND_DELEGATE_SCALE(BindElementScaleYDelegate, Y)
				}
				else if (Dime == EVectorDime::Z)
				{
					BIND_DELEGATE_SCALE(BindElementScaleZDelegate, Z)
				}
				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_Item]: BindProperty, ParentDetailVector Is Null"));
	}
}

void UUI_Item::NativeConstruct()
{
	Super::NativeConstruct();

	InputText->OnTextChanged.AddDynamic(this, &UUI_Item::OnTextChange);
	InputText->OnTextCommitted.AddDynamic(this, &UUI_Item::OnTextCommit);

	UsedCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_Item::OnCheckedStateChange);

	BindDelegate();
}

void UUI_Item::NativeDestruct()
{
	Super::NativeDestruct();

	DebindDelegate();
}

void UUI_Item::OnTextChange(const FText& InText)
{
	//更新Controller编辑状态
	if (AUGCGamePlayerController* UGCPC = MethodUnit::GetPlayerController(GetWorld()))
	{
		UGCPC->SetEditingDetail(true);
	}
}

void UUI_Item::OnTextCommit(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (InCommitMethod == ETextCommit::OnEnter || InCommitMethod == ETextCommit::OnUserMovedFocus)
	{
		if (!InText.IsEmpty() && InText.ToString().IsNumeric())
		{
			if (bIsSnappingValue)
			{
				//修改对齐参数
				if (AUGCGamePawn * MyPawn = MethodUnit::GetUGCPlayerPawn(GetWorld()))
				{
					MyPawn->SetSnappingValue(ModifyType, FCString::Atof(*InText.ToString()));
				}
			}
			else if (ParentDetailVector)	//Parent为DetailVector时，文本更新时调用
			{
				if (InText.ToString().IsNumeric())
				{
					ParentDetailVector->UpdateVector(Dime, FCString::Atof(*InText.ToString()));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[class UUI_Item]: OnCommitChange InText Is Null Or Illegal Input"));
		}

		//更新Controller编辑状态
		if (AUGCGamePlayerController * UGCPC = MethodUnit::GetPlayerController(GetWorld()))
		{
			UGCPC->SetEditingDetail(false);
		}
	}
}

void UUI_Item::OnCheckedStateChange(bool InbIsChecked)
{
	if (bIsSnappingValue)
	{
		if (InbIsChecked)
		{
			if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
			{
				if (AUGCGamePawn * MyPlayerPawn = MethodUnit::GetUGCPlayerPawn(GetWorld()))
				{
					MyPlayerPawn->SetTransformationType(ModifyType);
					MyPlayerState->ChangeModifyTypeDelegate.Broadcast(ModifyType);
				}
			}
		}
	}
	else
	{

	}
}

void UUI_Item::BindDelegate()
{
	//绑定
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		ChangeModifyTypeDelegateHandle = MyPlayerState->ChangeModifyTypeDelegate.AddLambda([&](const ETransformationType & InModifyType)
			{
				if (ModifyType != InModifyType)
				{
					SetUsedCheckState(ECheckBoxState::Unchecked);
				}
				else
				{
					SetUsedCheckState(ECheckBoxState::Checked);
				}
			});
	}
}

void UUI_Item::DebindDelegate()
{
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		MyPlayerState->ChangeModifyTypeDelegate.Remove(ChangeModifyTypeDelegateHandle);
	}
}

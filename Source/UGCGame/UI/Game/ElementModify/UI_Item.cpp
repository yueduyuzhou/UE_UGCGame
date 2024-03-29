// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Item.h"
#include "Components/CheckBox.h"
#include "../../../UGCGamePawn.h"
#include "Components/EditableText.h"
#include "../../../Common/MethodUnit.h"

void UUI_Item::SetModifyType(const ETransformationType& InModifyType)
{
	ModifyType = InModifyType;
}

void UUI_Item::SetUsedCheckState(const ECheckBoxState& InCheckedState)
{
	UsedCheckBox->SetCheckedState(InCheckedState);
}

void UUI_Item::NativeConstruct()
{
	Super::NativeConstruct();

	InputText->OnTextCommitted.AddDynamic(this, &UUI_Item::OnTextCommit);
	UsedCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_Item::OnCheckedStateChange);

	BindDelegate();
}

void UUI_Item::OnTextCommit(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (InText.ToString().IsNumeric())
	{
		//修改对齐参数
		if (AUGCGamePawn * MyPawn = MethodUnit::GetUGCPlayerPawn(GetWorld()))
		{
			MyPawn->SetSnappingValue(ModifyType, FCString::Atoi(*InText.ToString()));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Illegal Input"));
	}
}

void UUI_Item::OnCheckedStateChange(bool InbIsChecked)
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

void UUI_Item::BindDelegate()
{
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		MyPlayerState->ChangeModifyTypeDelegate.AddLambda([&](const ETransformationType & InModifyType)
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
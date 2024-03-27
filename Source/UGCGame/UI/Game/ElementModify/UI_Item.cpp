// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Item.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "../../../Common/MethodUnit.h"

void UUI_Item::SetModifyType(const EElementModifyType& InModifyType)
{
	ModifyType = InModifyType;
}

void UUI_Item::NativeConstruct()
{
	Super::NativeConstruct();

	InputText->OnTextCommitted.AddDynamic(this, &UUI_Item::OnTextCommit);
	UsedCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_Item::OnCheckedStateChange);

	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		MyPlayerState->ChangeModifyTypeDelegate.AddLambda([&](const EElementModifyType& InModifyType)
			{
				if (ModifyType != InModifyType)
				{
					UsedCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
				}
			});
	}
}

void UUI_Item::OnTextCommit(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (InText.ToString().IsNumeric())
	{
		//请求服务器修改属性
		if (AUGCGamePlayerState* MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
		{
			MyPlayerState->RequestChangeElementModifyValueOnServer(FCString::Atoi(*InText.ToString()), ModifyType);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Illegal Input"));
	}
}

void UUI_Item::OnCheckedStateChange(bool InbIsChecked)
{
	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		if (InbIsChecked)
		{
			MyPlayerState->ChangeModifyTypeDelegate.Broadcast(ModifyType);
			MyPlayerState->RequestChangeElementModifyOnServer(ModifyType);
		}
	}
}

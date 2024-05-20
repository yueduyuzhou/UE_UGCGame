// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EnumTeamType.h"
#include "UGCGame/Common/MethodUnit.h"

void UUI_EnumTeamType::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_EnumTeamType::PopulateComboBox()
{
	if (EnumSelect)
	{
		EnumSelect->ClearOptions();

		for (const FString& EnumName : Options)
		{
			EnumSelect->AddOption(EnumName);
		}

		FString TypeStr = MethodUnit::TeamTypeToString(SelectElement->GetTeamType());
		for (int32 i = 0; i < Options.Num(); i++)
		{
			if (Options[i] == TypeStr) 
			{
				EnumSelect->SetSelectedIndex(i);
			}
		}
	}
}

void UUI_EnumTeamType::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "TeamRed")
	{
		SelectElement->SetTeamType(ETeamType::TEAM_RED);
	}
	else if (SelectedItem == "TeamBlue")
	{
		SelectElement->SetTeamType(ETeamType::TEAM_BLUE);
	}
}
void UUI_EnumTeamType::SetEnumName()
{
	Name->SetText(FText::FromString(FString(TEXT("TeamType"))));
}
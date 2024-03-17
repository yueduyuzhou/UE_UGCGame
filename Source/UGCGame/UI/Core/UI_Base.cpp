// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Base.h"
#include "../../Common/MethodUnit.h"

void UUI_Base::NativeConstruct()
{
	Super::NativeConstruct();

	ElementID = INDEX_NONE;
}

AUGCGamePlayerState* UUI_Base::GetPlayerState()
{
	return MethodUnit::GetPlayerState(GetWorld());
}

AUGCGameState* UUI_Base::GetGameState()
{
	return MethodUnit::GetGameState(GetWorld());
}

void UUI_Base::SetElementID(const int32& InElementID)
{
	ElementID = InElementID;
}

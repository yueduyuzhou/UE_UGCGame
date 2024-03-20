// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Slot.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UUI_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	ClickButton->OnClicked.AddDynamic(this, &UUI_Slot::OnClickedWidget);
}

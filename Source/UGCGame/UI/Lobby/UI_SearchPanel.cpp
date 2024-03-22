// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SearchPanel.h"
#include "ThreadManage.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "../../Common/MethodUnit.h"
#include "../../Lobby/LobbyPlayerController.h"
//#include "OnlineSubsystem.h"
//#include "Interfaces/OnlineSessionInterface.h"
//#include "OnlineSessionSettings.h"

void UUI_SearchPanel::NativeConstruct()
{
	Super::NativeConstruct();

	FindMap->OnClicked.AddDynamic(this, &UUI_SearchPanel::OnFindMapClick);
}

void UUI_SearchPanel::OnFindMapClick()
{
	TArray<FBlueprintSessionResult> SessionRes = FindSessionBP(10, true);

	MapList->ClearChildren();
	for (auto& Tmp : SessionRes)
	{
		if (UUI_MapSearchSlot * SlotWidget = CreateWidget<UUI_MapSearchSlot>(GetWorld(), MapSearchSlotClass))
		{
			if (UPanelSlot * PanelSlot = MapList->AddChild(Cast<UWidget>(SlotWidget)))
			{
				SlotWidget->SetInfo(Tmp);
			}
		}
	}
}

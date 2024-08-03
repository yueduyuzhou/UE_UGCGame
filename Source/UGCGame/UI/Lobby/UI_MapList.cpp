// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MapList.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/EditableText.h"
#include "Components/Border.h"
#include "../../Common/MethodUnit.h"
#include "../../System/GameMapManage.h"
#include "../../UGCGameInstance.h"
#include "../../Lobby/LobbyPlayerController.h"

void UUI_MapList::NativeConstruct()
{
	Super::NativeConstruct();

	CreateMap->OnClicked.AddDynamic(this, &UUI_MapList::OnCreateMapClick);

	UpdateMapList();
}

void UUI_MapList::UpdateMapList()
{
	MapList->ClearChildren();

	if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
	{
		TArray<FString> Maps = MyPlayerState->GetMapList();

		for (auto& Tmp : Maps)
		{
			if (UUI_MapSlot * SlotWidget = CreateWidget<UUI_MapSlot>(GetWorld(), MapSlotClass))
			{
				if (UPanelSlot * PanelSlot = MapList->AddChild(Cast<UWidget>(SlotWidget)))
				{
					SlotWidget->SetMapList(this);
					SlotWidget->MapName->SetText(FText::FromString(Tmp));
				}
			}
		}
	}
}

void UUI_MapList::UpdateBackGround(const FString& InMapName)
{
	FString SavePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("LobbyTexture"), InMapName);
	if (UTexture2D * BGTexture = FMinimapCapture::LoadBMPImage(SavePath + FString(TEXT("00000.bmp"))))
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(BGTexture);
		BackGround->SetBrush(Brush);
	}
}

void UUI_MapList::OnCreateMapClick()
{
	FString MapName = CreateMapName->GetText().ToString();
	if (!MapName.IsEmpty())
	{
		if (UUGCGameInstance * GameInstance = GetWorld()->GetGameInstance<UUGCGameInstance>())
		{
			GameInstance->LoadMapName = MapName;
			UGameMapManage::Get()->CreateGameMap(GetWorld());
		}
	}
	else
	{
		
	}
	//CreateSessionBP(3, true);
}

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
#include "../../Common/ServerManage/ServerManage.h"
#include "ThreadManage.h"

TMap<int32, FString> UUI_MapList::MapIDToName;
TMap<FString, int32> UUI_MapList::NameToMapID;
FOnUGCMapInfoDelegate UUI_MapList::OnUGCMapInfoDelegate;

void UUI_MapList::NativeConstruct()
{
	Super::NativeConstruct();

	CreateMap->OnClicked.AddDynamic(this, &UUI_MapList::OnCreateMapClick);

	OnUGCMapInfoDelegate.AddUObject(this, &UUI_MapList::UpdateMapList);

	FServerManage::Get()->AddCallback<FUGC_MAP_INFO_RESPONSE>(SP_D2C_UGC_MAP_INFO_RESPONSE, UUI_MapList::OnUGCMapInfo);
	FUGC_MAP_INFO_REQUEST args;
	FServerManage::Get()->Send<FUGC_MAP_INFO_REQUEST>(SP_C2D_UGC_MAP_INFO_REQUEST, &args);
}

void UUI_MapList::NativeDestruct()
{
	FServerManage::Get()->RemoveCallback<FUGC_MAP_INFO_RESPONSE>(SP_D2C_UGC_MAP_INFO_RESPONSE, UUI_MapList::OnUGCMapInfo);
	Super::NativeDestruct();
}

void UUI_MapList::UpdateMapList()
{
	if (MapIDToName.Num() > 0)
	{
		MapList->ClearChildren();

		if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
		{
			//TArray<FString> Maps = MyPlayerState->GetMapList();
			//获取地图名字
			TArray<FString> Maps;
			for (auto& Tmp : MapIDToName)
			{
				Maps.Add(Tmp.Value);
			}

			//生成Slot
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
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.3f, [&]()
			{
				UpdateMapList();
			});
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
	if (!MapName.IsEmpty() && !NameToMapID.Contains(MapName))
	{
		if (UUGCGameInstance * GameInstance = GetWorld()->GetGameInstance<UUGCGameInstance>())
		{
			GameInstance->LoadMapName = MapName;
			UGameMapManage::Get()->CreateGameMap(GetWorld());
		}
	}
	//CreateSessionBP(3, true);
}

void UUI_MapList::OnUGCMapInfo(FUGC_MAP_INFO_RESPONSE InData)
{
	MapIDToName.Empty();

	int32 Len = InData.MapIDs.Num();
	for (int32 i = 0; i < Len; i++)
	{
		MapIDToName.Add(InData.MapIDs[i], InData.MapNames[i]);
		NameToMapID.Add(InData.MapNames[i], InData.MapIDs[i]);
	}

	OnUGCMapInfoDelegate.Broadcast();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerListSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../../../Lobby/LobbyPlayerController.h"

void UUI_PlayerListSlot::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerID = INDEX_NONE;

	PlayerButton->OnClicked.AddDynamic(this, &UUI_PlayerListSlot::PlayerButtonClick);
}

void UUI_PlayerListSlot::SetPlayerButtonEnable(const bool InIsEnable)
{
	PlayerButton->SetIsEnabled(InIsEnable);
}

void UUI_PlayerListSlot::SetButtonText(const FString& InButtonText)
{
	ButtonText->SetText(FText::FromString(InButtonText));
}

void UUI_PlayerListSlot::SetPlayerName(const FString& InPlayerName)
{
	PlayerName->SetText(FText::FromString(InPlayerName));
}

int32 UUI_PlayerListSlot::GetPlayerIDFromName()
{
	return FCString::Atoi(*PlayerName->GetText().ToString());
}

void UUI_PlayerListSlot::SetPlayerID(const int32& InPlayerID)
{
	PlayerID = InPlayerID;
}

int32 UUI_PlayerListSlot::GetPlayerID()
{
	if (APlayerController * MyPC = GetWorld()->GetFirstPlayerController())
	{
		if (ALobbyPlayerController * LobbyPC = Cast<ALobbyPlayerController>(MyPC))
		{
			return LobbyPC->PlayerID;
		}
	}
	return INDEX_NONE;
}

ENetRole UUI_PlayerListSlot::GetLocalRole()
{
	if (APlayerController * MyPC = GetWorld()->GetFirstPlayerController())
	{
		return MyPC->GetLocalRole();
	}
	return ENetRole::ROLE_None;
}

void UUI_PlayerListSlot::PlayerButtonClick()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (GetPlayerID() == PlayerID)
		{
			//服务器的本地玩家
		}
		else
		{
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(It->Get()))
				{
					if (MyPC->PlayerID == PlayerID)
					{
						MyPC->ServerCallClientQuit();
						break;
					}
				}
			}
		}
	}
	else
	{
		if (GetPlayerID() == PlayerID)
		{
			//客户端的本地玩家
		}
		else
		{
			//客户端的远端玩家
		}
	}
	//Server:踢出房间
}

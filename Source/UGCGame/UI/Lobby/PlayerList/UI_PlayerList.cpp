// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerList.h"
#include "UI_PlayerListSlot.h"
#include "../../../Common/UGCGameType.h"
#include "../../../Lobby/LobbyGameState.h"
#include "../../../Lobby/LobbyPlayerController.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ThreadManage.h"
#include "../../../Lobby/LobbyPlayers/LobbyPlayersGameMode.h"

void UUI_PlayerList::NativeConstruct()
{
	Super::NativeConstruct();
	if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		MyPC->SetPlayerList(this);
	}

	RedSelectButton->OnClicked.AddDynamic(this, &UUI_PlayerList::RedSelectButtonClick);
	BlueSelectButton->OnClicked.AddDynamic(this, &UUI_PlayerList::BlueSelectButtonClick);
	StartGame->OnClicked.AddDynamic(this, &UUI_PlayerList::StartGameButtonClick);

	if (GetLocalRole() == ROLE_Authority)
	{
		StartGame->SetIsEnabled(true);
	}
	else
	{
		StartGame->SetIsEnabled(false);
	}
}

void UUI_PlayerList::AddNewPlayer(const ETeamType& InTeamType, const int32& InPlayerID)
{
	if (!GetWorld()->IsPendingKill())
	{
		if (UUI_PlayerListSlot * SlotWidget = CreateWidget<UUI_PlayerListSlot>(GetWorld(), PlayerListSlotClass))
		{
			if (InTeamType == ETeamType::TEAM_RED)
			{
				if (RedPlayers)
				{
					if (UPanelSlot * PanelSlot = RedPlayers->AddChild(Cast<UWidget>(SlotWidget)))
					{
						SlotWidget->SetPlayerName(FString::FromInt(InPlayerID));
					}
				}
			}
			else if (InTeamType == ETeamType::TEAM_BLUE)
			{
				if (BluePlayers)
				{
					if (UPanelSlot * PanelSlot = BluePlayers->AddChild(Cast<UWidget>(SlotWidget)))
					{
						SlotWidget->SetPlayerName(FString::FromInt(InPlayerID));
					}
				}
			}

			if (GetLocalRole() == ROLE_Authority)
			{
				if (GetPlayerID() == InPlayerID)
				{
					SlotWidget->SetButtonText(FString(TEXT("Self")));
				}
				else
				{
					SlotWidget->SetButtonText(FString(TEXT("Kick")));
				}
			}
			else
			{
				if (GetPlayerID() == InPlayerID)
				{
					SlotWidget->SetButtonText(FString(TEXT("Self")));
				}
				else
				{
					SlotWidget->SetButtonText(FString(TEXT("Other")));
					SlotWidget->SetPlayerButtonEnable(false);
				}
			}
			SlotWidget->SetPlayerID(InPlayerID);
		}
	}
}

void UUI_PlayerList::RemovePlayer(const int32& InPlayerID)
{
	TArray<UWidget*> AllWidgets = RedPlayers->GetAllChildren();
	for (auto& Tmp : AllWidgets)
	{
		if (UUI_PlayerListSlot * BoxSlot = Cast<UUI_PlayerListSlot>(Tmp))
		{
			if (InPlayerID == BoxSlot->GetPlayerIDFromName())
			{
				RedPlayers->RemoveChild(Tmp);
				return;
			}
		}
	}

	AllWidgets = BluePlayers->GetAllChildren();
	for (auto& Tmp : AllWidgets)
	{
		if (UUI_PlayerListSlot * BoxSlot = Cast<UUI_PlayerListSlot>(Tmp))
		{
			if (InPlayerID == BoxSlot->GetPlayerIDFromName())
			{
				BluePlayers->RemoveChild(Tmp);
				return;
			}
		}
	}
}

void UUI_PlayerList::UpdatePlayerList(const TArray<FPlayerNetData>& InPlayersDatas)
{
	if (RedPlayers && BluePlayers)
	{
		RedPlayers->ClearChildren();
		BluePlayers->ClearChildren();
		//if (ALobbyGameState * MyGameState = GetWorld()->GetGameState<ALobbyGameState>())
		{
			for (auto& Tmp : InPlayersDatas)
			{
				AddNewPlayer(Tmp.Team, Tmp.PlayerID);
			}
		}
	}
}

ENetRole UUI_PlayerList::GetLocalRole()
{
	if (APlayerController * MyPC = GetWorld()->GetFirstPlayerController())
	{
		return MyPC->GetLocalRole();
	}
	return ENetRole::ROLE_None;
}

bool UUI_PlayerList::IsAuthority()
{
	return GetLocalRole() == ROLE_Authority;
}

int32 UUI_PlayerList::GetPlayerID()
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

void UUI_PlayerList::RedSelectButtonClick()
{
	if (APlayerController * MyPC = GetWorld()->GetFirstPlayerController())
	{
		if (ALobbyPlayerController * LobbyPC = Cast<ALobbyPlayerController>(MyPC))
		{
			FPlayerNetData TmpPlayerData;
			TmpPlayerData.PlayerID = LobbyPC->PlayerID;
			TmpPlayerData.Team = ETeamType::TEAM_RED;
			LobbyPC->ServerCallClientUpdateLocalPlayerData(TmpPlayerData);
			LobbyPC->PlayerListChangeOnServer(TmpPlayerData);
		}
	}
}

void UUI_PlayerList::BlueSelectButtonClick()
{
	if (APlayerController * MyPC = GetWorld()->GetFirstPlayerController())
	{
		if (ALobbyPlayerController * LobbyPC = Cast<ALobbyPlayerController>(MyPC))
		{
			FPlayerNetData TmpPlayerData;
			TmpPlayerData.PlayerID = LobbyPC->PlayerID;
			TmpPlayerData.Team = ETeamType::TEAM_BLUE;
			LobbyPC->ServerCallClientUpdateLocalPlayerData(TmpPlayerData);
			LobbyPC->PlayerListChangeOnServer(TmpPlayerData);
		}
	}
}

void UUI_PlayerList::StartGameButtonClick()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (UWorld * CurWorld = GetWorld())
		{
			GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
				{
					FString Command = FString::Printf(TEXT("ServerTravel GameTemplateMap"));
					UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *Command);
				});
		}
	}
	else
	{

	}
}

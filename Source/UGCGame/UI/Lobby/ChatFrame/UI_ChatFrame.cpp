// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ChatFrame.h"
#include "UI_MassageText.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "../../../Lobby/LobbyPlayerController.h"

void UUI_ChatFrame::NativeConstruct()
{
	Super::NativeConstruct();
	if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		MyPC->SetChatFrame(this);
	}

	SendButton->OnClicked.AddDynamic(this, &UUI_ChatFrame::SendButtonClick);
}

void UUI_ChatFrame::AddMassageToContent(const FString& InMsg)
{
	if (MassageContent)
	{
		if (ALobbyPlayerController * MyPC = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			//创建消息UI,添加到InputMassage
			if (UUI_MassageText * SlotWidget = CreateWidget<UUI_MassageText>(GetWorld(), MassageTextClass))
			{
				SlotWidget->SetPlayer(FString::FromInt(MyPC->PlayerID));
				SlotWidget->SetMassage(InMsg);
				MassageContent->AddChild(Cast<UWidget>(SlotWidget));
			}
		}
	}
}

void UUI_ChatFrame::SendButtonClick()
{
	FString Msg = InputMassage->GetText().ToString();
	InputMassage->SetText(FText::FromString(FString(TEXT(""))));
	if (!Msg.IsEmpty())
	{
		if (APlayerController * MyPC = GetWorld()->GetFirstPlayerController())
		{
			if (ALobbyPlayerController * MyLobbyPC = Cast<ALobbyPlayerController>(MyPC))
			{
				MyLobbyPC->AddMassageOnServer(Msg);
			}
		}
	}
}


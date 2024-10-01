// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_LogginPanel.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "UGCGame/Common/ServerManage/ServerManage.h"
#include "Kismet/GameplayStatics.h"

FOnLoginDelegate UUI_LogginPanel::OnLoginDelegate;

void UUI_LogginPanel::NativeConstruct()
{
	Super::NativeConstruct();

	OKBtn->OnClicked.AddDynamic(this, &UUI_LogginPanel::OnOKBtnClick);
	CanelBtn->OnClicked.AddDynamic(this, &UUI_LogginPanel::OnCanelBtnClick);
	
	OnLoginDelegate.AddUObject(this, &UUI_LogginPanel::OpenLobbyMap);

	FServerManage::Get()->AddCallback<FLOGIN_REP>(SP_D2C_LOGIN_REP, LoginReq);
}

void UUI_LogginPanel::NativeDestruct()
{
	Super::NativeDestruct();

	FServerManage::Get()->RemoveCallback<FLOGIN_REP>(SP_D2C_LOGIN_REP);
}

void UUI_LogginPanel::OnOKBtnClick()
{
	FLOGIN_REQ InData = FLOGIN_REQ(AccountEditTxt->GetText().ToString(), PasswordEditTxt->GetText().ToString());
	FServerManage::Get()->Send<FLOGIN_REQ>(SP_C2D_LOGIN_REQ, &InData);
}

void UUI_LogginPanel::OnCanelBtnClick()
{
	//
}

void UUI_LogginPanel::LoginReq(FLOGIN_REP InData)
{
	if (InData.IsSuccess && OnLoginDelegate.IsBound())
	{
		OnLoginDelegate.Broadcast();
	}
}

void UUI_LogginPanel::OpenLobbyMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")));
}

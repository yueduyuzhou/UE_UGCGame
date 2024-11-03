// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_LogginPanel.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "UGCGame/Common/ServerManage/ServerManage.h"
#include "../../Common/PlayerModule/PlayerModule.h"
#include "../../System/GameMapManage.h"

FOnLoginDelegate UUI_LogginPanel::OnLoginDelegate;

void UUI_LogginPanel::NativeConstruct()
{
	Super::NativeConstruct();

	//永远最开始位置
	InitModules();

	OKBtn->OnClicked.AddDynamic(this, &UUI_LogginPanel::OnOKBtnClick);
	CanelBtn->OnClicked.AddDynamic(this, &UUI_LogginPanel::OnCanelBtnClick);
	
	OnLoginDelegate.AddUObject(this, &UUI_LogginPanel::OpenLobbyMap);

	FServerManage::Get()->AddCallback<FLOGIN_REP>(SP_D2C_LOGIN_REP, UUI_LogginPanel::LoginReq);
}

void UUI_LogginPanel::NativeDestruct()
{
	Super::NativeDestruct();

	FServerManage::Get()->RemoveCallback<FLOGIN_REP>(SP_D2C_LOGIN_REP, UUI_LogginPanel::LoginReq);
}

void UUI_LogginPanel::OnOKBtnClick()
{
	FLOGIN_REQ InData = FLOGIN_REQ(AccountEditTxt->GetText().ToString(), PasswordEditTxt->GetText().ToString());
	
	if (CheckLoginInfo(InData))
	{
		FServerManage::Get()->Send<FLOGIN_REQ>(SP_C2D_LOGIN_REQ, &InData);
	}
}

void UUI_LogginPanel::OnCanelBtnClick()
{
	//
}

void UUI_LogginPanel::LoginReq(FLOGIN_REP InData)
{
	if (InData.IsSuccess && OnLoginDelegate.IsBound())
	{
		OnLoginDelegate.Broadcast(InData);
	}
}

void UUI_LogginPanel::OpenLobbyMap(const FLOGIN_REP& InData)
{
	//初始化玩家信息
	UPlayerModule::Get()->InitPlayerInfo(InData);

	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")));
}

bool UUI_LogginPanel::CheckLoginInfo(const FLOGIN_REQ& InData)
{
	//TODO:检测输入合法
	return true;
}

void UUI_LogginPanel::InitModules()
{
	UPlayerModule::Get();
	UGameMapManage::Get();
	FServerManage::Get();
}

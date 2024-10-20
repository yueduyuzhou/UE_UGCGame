// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_QuitPanel.h"
#include "Components/Button.h"
#include "UGCGame/Common/ServerManage/ServerManage.h"
#include "Kismet/KismetSystemLibrary.h"

FOnQuitDelegate UUI_QuitPanel::OnQuitDelegate;

void UUI_QuitPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (SureButton && CancelButton)
	{
		SureButton->OnClicked.AddDynamic(this, &UUI_QuitPanel::OnSureButtonClicked);
		CancelButton->OnClicked.AddDynamic(this, &UUI_QuitPanel::OnCancelButtonClicked);
	}

	OnQuitDelegate.AddUObject(this, &UUI_QuitPanel::OnQuitGame);

	FServerManage::Get()->AddCallback<FQUIT_REP>(SP_D2C_QUIT_REP, UUI_QuitPanel::QuitReq);
}

void UUI_QuitPanel::NativeDestruct()
{
	Super::NativeDestruct();

	FServerManage::Get()->RemoveCallback<FQUIT_REP>(SP_D2C_QUIT_REP, UUI_QuitPanel::QuitReq);
}

void UUI_QuitPanel::OnSureButtonClicked()
{
	//发送退出请求
	FQUIT_REQ InData;
	FServerManage::Get()->Send(SP_C2D_QUIT_REQ, &InData);
}

void UUI_QuitPanel::OnCancelButtonClicked()
{

}

void UUI_QuitPanel::QuitReq(FQUIT_REP InData)
{
	if (InData.IsSuccess && OnQuitDelegate.IsBound())
	{
		OnQuitDelegate.Broadcast(InData);
	}
}

void UUI_QuitPanel::OnQuitGame(const FQUIT_REP& InData)
{
	if (InData.IsSuccess)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	}
	else
	{
		//TODO：退出失败通知
	}
}

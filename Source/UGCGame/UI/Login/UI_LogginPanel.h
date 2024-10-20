// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UGCGame/UI/Core/UI_Base.h"
#include "UI_LogginPanel.generated.h"

class UEditableText;
class UButton;
struct FLOGIN_REP;
struct FLOGIN_REQ;

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoginDelegate, const FLOGIN_REP&);

UCLASS()
class UGCGAME_API UUI_LogginPanel : public UUI_Base
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		UEditableText* AccountEditTxt;

	UPROPERTY(meta = (BindWidget))
		UEditableText* PasswordEditTxt;

	UPROPERTY(meta = (BindWidget))
		UButton* OKBtn;

	UPROPERTY(meta = (BindWidget))
		UButton* CanelBtn;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnOKBtnClick();
	UFUNCTION()
	void OnCanelBtnClick();

	static void LoginReq(FLOGIN_REP InData);

	void OpenLobbyMap(const FLOGIN_REP& InData);
	bool CheckLoginInfo(const FLOGIN_REQ& InData);
	void InitModules();

private:
	static FOnLoginDelegate OnLoginDelegate;
};

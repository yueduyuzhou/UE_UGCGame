// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UGCGame/UI/Core/UI_Base.h"
#include "UI_QuitPanel.generated.h"

class UButton;
struct FQUIT_REP;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuitDelegate, const FQUIT_REP&);

UCLASS()
class UGCGAME_API UUI_QuitPanel : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UButton* SureButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
		void OnSureButtonClicked();
	UFUNCTION()
		void OnCancelButtonClicked();

	static void QuitReq(FQUIT_REP InData);

	void OnQuitGame(const FQUIT_REP& InData);

private:
	static FOnQuitDelegate OnQuitDelegate;
};

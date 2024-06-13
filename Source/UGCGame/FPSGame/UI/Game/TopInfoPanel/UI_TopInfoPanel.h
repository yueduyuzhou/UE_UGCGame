// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_FPSBase.h"
#include "UGCGame/Common/UGCGameType.h"
#include "UI_TopInfoPanel.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_TopInfoPanel : public UUI_FPSBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BlueKillText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* RedKillText;

	UUI_TopInfoPanel();

protected:
	virtual void NativeConstruct() override;

private:
	void RegisterToPlayerController();

public:
	void UpdateKillText(const ETeamType& InKilTeamType);

private:
	int32 BlueKillCount;
	int32 RedKillCount;
};

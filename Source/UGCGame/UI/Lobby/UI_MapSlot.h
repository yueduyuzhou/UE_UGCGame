// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Game/Core/UI_Slot.h"
#include "UI_MapSlot.generated.h"

class UButton;
class UTextBlock;
class UUI_MapList;

/**
 * 
 */
UCLASS()
class UGCGAME_API UUI_MapSlot : public UUI_Slot
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* MapName;
public:
	virtual void OnClickedWidget() override;

protected:
	virtual void NativeConstruct() override;

	// 悬停事件处理函数
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

public:
	void SetMapList(UUI_MapList* InMapList);

private:
	UUI_MapList* MapList = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "UI_MapSlot.h"
#include "UI_MapList.generated.h"

class UVerticalBox;
class UButton;
class UEditableText;
class UBorder;
struct FUGC_MAP_INFO_RESPONSE;

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnUGCMapInfoDelegate);

UCLASS()
class UGCGAME_API UUI_MapList : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* MapList;

	UPROPERTY(meta = (BindWidget))
		UButton* CreateMap;

	UPROPERTY(meta = (BindWidget))
		UEditableText* CreateMapName;

	UPROPERTY(meta = (BindWidget))
		UBorder* BackGround;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_MapSlot> MapSlotClass;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void UpdateMapList();

public:
	void UpdateBackGround(const FString& InMapName);

protected:
	UFUNCTION(BlueprintCallable)
	void OnCreateMapClick();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CreateSessionBP(int32 InMax, bool InUseLAN);

	/*-------------------------------协议回调--------------------------------*/
private:
	static void OnUGCMapInfo(FUGC_MAP_INFO_RESPONSE InData);

private:
	static FOnUGCMapInfoDelegate OnUGCMapInfoDelegate;

	static TMap<int32, FString> MapIDToName;
};

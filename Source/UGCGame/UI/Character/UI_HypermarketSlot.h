// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Table/HypermarketTable.h"
#include "UGCGame/UI/Game/Core/UI_Slot.h"
#include "UI_HypermarketSlot.generated.h"

class UImage;
class UButton;
class UTextBlock;
class AHypermarketCharacterBase;
class UUI_Hypermarket;

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FHypermarketDelegate, const int32&)

UCLASS()
class UGCGAME_API UUI_HypermarketSlot : public UUI_Slot
{
	friend class UUI_Hypermarket;
	friend class UUI_Backpack;

	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SlotGold;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SlotName;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* EquippedName;

public:
	UUI_HypermarketSlot();

protected:
	virtual void NativeConstruct() override;

	virtual void OnClickedWidget();

public:
	void SetSlotName(const FString& InName);
	void SetSlotGold(const int32 InGold);
	void SetEquippedName(const FString& InName);

	/*更新Slot信息*/
	void UpdateSlot(const FHypermarketTable* InTable);

	void SetHyperTableID(const int32& InID);
	FORCEINLINE const int32& GetHyperTableID() { return HyperTableID; }

protected:
	int32 HyperTableID;

	AHypermarketCharacterBase* HyperChracter;
	FHypermarketDelegate HypermarketDelegate;
};

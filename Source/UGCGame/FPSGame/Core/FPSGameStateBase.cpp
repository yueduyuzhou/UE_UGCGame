// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameStateBase.h"
#include "../../Table/FPSGameConfig.h"
#include "FPSGameModeBase.h"

AFPSGameStateBase::AFPSGameStateBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Game_Config(TEXT("/Game/Table/FPSGameConfig"));
	GameCfgPtr = Game_Config.Object;
}

FFPSGameConfig* AFPSGameStateBase::GetGameConfigTemplate(const int32& InID)
{
	if (const TArray<FFPSGameConfig*> * SlotData = GetGameConfigsTemplate())
	{
		for (auto* Tmp : (*SlotData))
		{
			if (Tmp->ID == InID)
			{
				return Tmp;
			}
		}
	}
	return nullptr;
}

FFPSGameConfig* AFPSGameStateBase::GetGameConfigTemplate()
{
	if (AFPSGameModeBase * FPSGMBase = Cast<AFPSGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		const int32& ModeID = FPSGMBase->GetFPSMode();
		const TArray<FFPSGameConfig*>* SlotData = GetGameConfigsTemplate();
		//UE_LOG(LogTemp, Display, TEXT("[class AFPSGameStateBase::GetGameConfigTemplate]£ºModeID = %d"), ModeID);

		if (ModeID != INDEX_NONE && SlotData)
		{
			for (auto* Tmp : (*SlotData))
			{
				//UE_LOG(LogTemp, Display, TEXT("[class AFPSGameStateBase::GetGameConfigTemplate]£ºTmp->ID = %d"), Tmp->ID);
				for (auto& Tmp2 : Tmp->DefaultWeapon)
					UE_LOG(LogTemp, Display, TEXT("[class AFPSGameStateBase::GetGameConfigTemplate]£ºItemId = %d"), Tmp2);
				if (Tmp->ID == ModeID)
				{
					return Tmp;
				}
			}
		}
	}
	return nullptr;
}

TArray<FFPSGameConfig*>* AFPSGameStateBase::GetGameConfigsTemplate()
{
	if (!CacheGameCfgs.Num())
	{
		if (GameCfgPtr)
		{
			GameCfgPtr->GetAllRows(TEXT("FPS Game Config"), CacheGameCfgs);
		}
	}
	return &CacheGameCfgs;
}

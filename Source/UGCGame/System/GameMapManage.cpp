#include "GameMapManage.h"
#include "../Common/MethodUnit.h"
#include "../UGCGameState.h"
#include "Kismet/GameplayStatics.h"
#include "../SaveData/MapSaveData.h"
#include "../Element/ElementBase.h"

FGameMapManage::FGameMapManage()
{
}

void FGameMapManage::OpenMap(UWorld* InWorld, FName InOpenMapName)
{
	UGameplayStatics::OpenLevel(InWorld, FName(TEXT("TemplateMap")));
	
	LoadMapDataAndSpawn(InOpenMapName.ToString());
}

void FGameMapManage::CreateGameMap(UWorld* InWorld)
{
	UGameplayStatics::OpenLevel(InWorld, FName(TEXT("TemplateMap")));
}

void FGameMapManage::QuitGameMap()
{

}

void FGameMapManage::SaveGameMap(UWorld* InWorld)
{
	UMapSaveData* SaveGameInstance = Cast<UMapSaveData>(UGameplayStatics::CreateSaveGameObject(UMapSaveData::StaticClass()));
	if (SaveGameInstance)
	{
		if (AUGCGameState * MyGameState = MethodUnit::GetGameState(InWorld))
		{
			const TMap<int32, AElementBase*>& MapData = MyGameState->GetMapDatas();
			if (MapData.Num())
			{
				for (auto& Tmp : MapData)
				{
					SaveGameInstance->SaveMapData.Add(Tmp.Key, TPair<FVector, FRotator>(Tmp.Value->GetActorLocation(), Tmp.Value->GetActorRotation()));
				}
			}
		}

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SaveMapData"), 0);
	}
}

void FGameMapManage::LoadMapDataAndSpawn(const FString& InSlotName)
{
	UMapSaveData* SaveMapData = Cast<UMapSaveData>(UGameplayStatics::LoadGameFromSlot(InSlotName, 0));
	if (SaveMapData)
	{
		//Éú³ÉElements
	}
}

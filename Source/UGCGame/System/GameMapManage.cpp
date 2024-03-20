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
	
	//TODO
	LoadMapDataAndSpawn(InOpenMapName.ToString(), InWorld);
}

void FGameMapManage::CreateGameMap(UWorld* InWorld)
{
	UGameplayStatics::OpenLevel(InWorld, FName(TEXT("TemplateMap")));
}

void FGameMapManage::QuitGameMap(UWorld* InWorld)
{
	//TODO
	UGameplayStatics::OpenLevel(InWorld, FName(TEXT("ThirdPersonExampleMap")));
}

void FGameMapManage::QuitAndSaveMap(UWorld* InWorld)
{
	SaveGameMap(InWorld);
	QuitGameMap(InWorld);
}

void FGameMapManage::SaveGameMap(UWorld* InWorld)
{
	TArray<AActor*> Elements;
	UGameplayStatics::GetAllActorsOfClass(InWorld, AElementBase::StaticClass(), Elements);

	UMapSaveData* SaveGameInstance = Cast<UMapSaveData>(UGameplayStatics::CreateSaveGameObject(UMapSaveData::StaticClass()));
	if (SaveGameInstance)
	{
		if (Elements.Num())
		{
			for (auto& Tmp : Elements)
			{
				if (AElementBase * Elem = Cast<AElementBase>(Tmp))
				{
					if (SaveGameInstance->SaveMapData.Contains(Elem->GetElementID()))
					{
						SaveGameInstance->SaveMapData[Elem->GetElementID()].Add(TPair<FVector, FRotator>(Elem->GetActorLocation(), Elem->GetActorRotation()));
					}
					else
					{
						SaveGameInstance->SaveMapData.Add(Elem->GetElementID());
						SaveGameInstance->SaveMapData[Elem->GetElementID()].Add(TPair<FVector, FRotator>(Elem->GetActorLocation(), Elem->GetActorRotation()));
					}
					Elem->DestoryElement();
				}
			}
		}

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SaveMapData"), 0);
	}
}

void FGameMapManage::LoadMapDataAndSpawn(const FString& InSlotName, UWorld* InWorld)
{
	if (AUGCGameState * MyGameState = MethodUnit::GetGameState(InWorld))
	{
		UMapSaveData* SaveMapData = Cast<UMapSaveData>(UGameplayStatics::LoadGameFromSlot(InSlotName, 0));
		if (SaveMapData)
		{
			//Éú³ÉElements
			for (auto& Tmp : SaveMapData->SaveMapData)
			{
				if (const FElementAttribute * ElementAttr = MyGameState->GetElementAttributeTemplate(Tmp.Key))
				{
					for (auto& Info : Tmp.Value)
					{
						InWorld->SpawnActor<AElementBase>(ElementAttr->ElementClass, Info.Key, Info.Value);
					}
				}
			}
		}
	}
}

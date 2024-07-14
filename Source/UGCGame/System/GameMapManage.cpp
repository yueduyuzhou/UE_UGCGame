#include "GameMapManage.h"
#include "../UGCGameState.h"
#include "../UGCGameInstance.h"
#include "../SaveData/MapSaveData.h"
#include "../Element/ElementBase.h"
#include "../Element/BuildElement.h"
#include "../Lobby/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../Element/EffectElement.h"
#include "ThreadManage.h"


TSharedPtr<UGameMapManage> UGameMapManage::GameMapManage = nullptr;

TSharedRef<UGameMapManage> UGameMapManage::Get()
{
	if (!GameMapManage.IsValid())
	{
		GameMapManage = MakeShareable(new UGameMapManage);
	}

	return GameMapManage.ToSharedRef();
}

void UGameMapManage::Destroy()
{
	if (GameMapManage.IsValid())
	{
		GameMapManage = nullptr;
	}
}

void UGameMapManage::OpenMapForFPS(UWorld* InWorld, FString InOpenMapName)
{
	if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(InWorld->GetGameInstance()))
	{
		// 操作数据
		MyGameInstance->LoadMapName = InOpenMapName;

		UGameplayStatics::OpenLevel(InWorld, FName(TEXT("GameTemplateMap")), true, TEXT("listen"));
	}
}

void UGameMapManage::OpenMapForUGC(UWorld* InWorld, FString InOpenMapName)
{
	if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(InWorld->GetGameInstance()))
	{
		// 操作数据
		MyGameInstance->LoadMapName = InOpenMapName;

		UGameplayStatics::OpenLevel(InWorld, FName(TEXT("TemplateMap")), true);
	}
}

void UGameMapManage::CreateGameMap(UWorld* InWorld)
{
	UGameplayStatics::OpenLevel(InWorld, TEXT("/Game/Maps/TemplateMap"), true, TEXT("listen"));
}

void UGameMapManage::QuitGameMap(UWorld* InWorld)
{
	UGameplayStatics::OpenLevel(InWorld, FName(TEXT("/Game/Maps/Lobby")));
}

void UGameMapManage::QuitAndSaveMap(UWorld* InWorld)
{
	SaveGameMap(InWorld);
	QuitGameMap(InWorld);
}

void UGameMapManage::SaveGameMap(UWorld* InWorld)
{
	if (UUGCGameInstance * MyGameInstance = InWorld->GetGameInstance<UUGCGameInstance>())
	{
		TArray<AActor*> Elements;
		UGameplayStatics::GetAllActorsOfClass(InWorld, AElementBase::StaticClass(), Elements);

		UMapSaveData* SaveGameInstance = Cast<UMapSaveData>(UGameplayStatics::CreateSaveGameObject(UMapSaveData::StaticClass()));
		if (SaveGameInstance)
		{
			if (Elements.Num())
			{
				//保存游戏数据（TODO:TO BDServer）
				for (auto& Tmp : Elements)
				{
					if (AElementBase * Elem = Cast<AElementBase>(Tmp))
					{
						SaveGameInstance->Elements.Add(
							FElemInfo(
								Elem->GetElementID(), 
								Elem->GetActorLocation(), 
								Elem->GetActorRotation(),
								Elem->GetActorScale3D(),
								Elem->GetTeamType(),
								Elem->GetElementMeshColor()));
						
						UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage] : SaveGameMap, ElementID = %d"), Elem->GetElementID());
					}
				}
			}
			//保存游戏数据（TODO:TO BDServer）
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, MyGameInstance->LoadMapName, 0);
		}
	}
}

void UGameMapManage::LoadMapDataAndSpawnForFPS(const FString& InSlotName, UWorld* InWorld)
{
	LoadMapDataAndSpawn(InSlotName, InWorld, false);
}

void UGameMapManage::LoadMapDataAndSpawnForUGC(const FString& InSlotName, UWorld* InWorld)
{
	LoadMapDataAndSpawn(InSlotName, InWorld, true);
}

void UGameMapManage::LoadMapDataAndSpawn(const FString& InSlotName, UWorld* InWorld, bool InbShowEffectMesh)
{
	if (InWorld)
	{
		if (AUGCGameState * MyGameState = MethodUnit::GetGameState(InWorld))
		{
			//从文件中加载属性
			if (UMapSaveData * SaveMapData = Cast<UMapSaveData>(UGameplayStatics::LoadGameFromSlot(InSlotName, 0)))
			{
				if (SaveMapData->Elements.Num() > 0)
				{
					//生成Elements BUG
					for (auto& Tmp : SaveMapData->Elements)
					{
						//从表中读取每个Element的属性
						if (const FElementAttribute * ElementAttr = MyGameState->GetElementAttributeTemplate(Tmp.ElementID))
						{
							//生成对应Element
							if (AElementBase * MewElement = InWorld->SpawnActor<AElementBase>(ElementAttr->ElementClass, Tmp.Location, Tmp.Rotation))
							{
								//设置保存的属性
								MewElement->SetElementID(Tmp.ElementID);
								MewElement->SetActorScale3D(Tmp.Scale);
								MewElement->SetTeamType(Tmp.TeamType);

								//TODO:分情况处理
								if (ABuildElement * BElement = Cast<ABuildElement>(MewElement))
								{
									BElement->SetElementMesh(ElementAttr->ElementMeth);
									BElement->SetMeshColorMulticast(Tmp.Color);
								}
								else if (AEffectElement * EElement = Cast<AEffectElement>(MewElement))
								{
									EElement->SetElementMeshVisibility(InbShowEffectMesh);
								}

								//记录生成数量
								MyGameState->AddSpawnData(Tmp.ElementID);
							}
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("[class UGameMapManage] : GetElementAttributeTemplate fail, ElementID = %d"), Tmp.ElementID);
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("[class UGameMapManage] : SaveMapData->Elements Is Empty"));
					return;
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[class UGameMapManage] : SaveMapData Is Null"));
				return;
			}
		}
	}
}

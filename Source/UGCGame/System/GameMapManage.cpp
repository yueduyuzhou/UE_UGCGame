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

void UGameMapManage::OpenMap(UWorld* InWorld, FString InOpenMapName)
{
	if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(InWorld->GetGameInstance()))
	{
		// 操作数据
		MyGameInstance->LoadMapName = InOpenMapName;

		UGameplayStatics::OpenLevel(InWorld, FName(TEXT("GameTemplateMap")), true, TEXT("listen"));
	}
}

void UGameMapManage::CreateGameMap(UWorld* InWorld)
{
	UGameplayStatics::OpenLevel(InWorld, TEXT("/Game/Maps/TemplateMap"), true, TEXT("listen"));
	
	//PIE运行游戏时调用servertravel会没有效果，在打包后的游戏中运行一切正常
	//InWorld->ServerTravel(TEXT("/Game/Maps/TemplateMap"));
}

void UGameMapManage::QuitGameMap(UWorld* InWorld)
{
	//TODO
	//PIE运行游戏时调用servertravel会没有效果，在打包后的游戏中运行一切正常
	//InWorld->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap"));
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
					}
				}
			}
			//保存游戏数据（TODO:TO BDServer）
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, MyGameInstance->LoadMapName, 0);
		}
	}
}

void UGameMapManage::LoadMapDataAndSpawn(const FString& InSlotName, UWorld* InWorld)
{
	if (InWorld)
	{
		if (AUGCGameState * MyGameState = MethodUnit::GetGameState(InWorld))
		{
			//UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage] : Load Map Data And Spawn"));
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
								MewElement->SetActorScale3D(Tmp.Scale);
								MewElement->SetTeamType(Tmp.TeamType);

								//TODO:分情况处理
								if (ABuildElement * BElement = Cast<ABuildElement>(MewElement))
								{
									BElement->SetElementMesh(ElementAttr->ElementMeth);
									BElement->SetElementMeshColor(Tmp.Color);
								}
								else if (AEffectElement * EElement = Cast<AEffectElement>(MewElement))
								{
									EElement->SetNotVisibilityMulticast();
								}
							}
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

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
#include "../Common/ServerManage/ServerManage.h"

TSharedPtr<UGameMapManage> UGameMapManage::GameMapManage = nullptr;
TMap<FString, int32> UGameMapManage::NameToMapID;
TArray<FElemInfo> UGameMapManage::TmpElements;

UGameMapManage::UGameMapManage()
{
	//获取MapList信息
	FServerManage::Get()->AddCallback<FUGC_MAP_INFO_RESPONSE>(SP_D2C_UGC_MAP_INFO_RESPONSE, UGameMapManage::OnUGCMapInfo);
	FUGC_MAP_INFO_REQUEST args1;
	FServerManage::Get()->Send<FUGC_MAP_INFO_REQUEST>(SP_C2D_UGC_MAP_INFO_REQUEST, &args1);
}

TSharedRef<UGameMapManage> UGameMapManage::Get()
{
	if (!GameMapManage.IsValid())
	{
		GameMapManage = MakeShareable(new UGameMapManage());
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

		//UMapSaveData* SaveGameInstance = Cast<UMapSaveData>(UGameplayStatics::CreateSaveGameObject(UMapSaveData::StaticClass()));
		//if (SaveGameInstance)
		//{
		//	if (Elements.Num())
		//	{
		//		//保存游戏数据（TODO:TO BDServer）
		//		for (auto& Tmp : Elements)
		//		{
		//			if (AElementBase * Elem = Cast<AElementBase>(Tmp))
		//			{
		//				SaveGameInstance->Elements.Add(
		//					FElemInfo(
		//						Elem->GetElementID(), 
		//						Elem->GetActorLocation(), 
		//						Elem->GetActorRotation(),
		//						Elem->GetActorScale3D(),
		//						Elem->GetTeamType(),
		//						Elem->GetElementMeshColor()));
		//				
		//				UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage] : SaveGameMap, ElementID = %d"), Elem->GetElementID());
		//			}
		//		}
		//	}
		//	//保存游戏数据（TODO:TO BDServer）
		//	UGameplayStatics::SaveGameToSlot(SaveGameInstance, MyGameInstance->LoadMapName, 0);
		//}

		FUGC_SAVE_MAP_INFO_REQ InData;
		if (NameToMapID.Contains(MyGameInstance->LoadMapName))
		{
			InData.MapID = NameToMapID[MyGameInstance->LoadMapName];
		}
		else
		{
			InData.MapID = INDEX_NONE;	
		}
		InData.MapName = MyGameInstance->LoadMapName;

		//保存游戏数据到 BDServer
		for (auto& Tmp : Elements)
		{
			if (AElementBase * Elem = Cast<AElementBase>(Tmp))
			{
				InData.IntData.Add(Elem->GetElementID());
				InData.IntData.Add(static_cast<int32>(Elem->GetTeamType()));

				InData.FloatData.Add(Elem->GetActorLocation().X);
				InData.FloatData.Add(Elem->GetActorLocation().Y);
				InData.FloatData.Add(Elem->GetActorLocation().Z);
				InData.FloatData.Add(Elem->GetActorRotation().Pitch);
				InData.FloatData.Add(Elem->GetActorRotation().Yaw);
				InData.FloatData.Add(Elem->GetActorRotation().Roll);
				InData.FloatData.Add(Elem->GetActorScale3D().X);
				InData.FloatData.Add(Elem->GetActorScale3D().Y);
				InData.FloatData.Add(Elem->GetActorScale3D().Z);
				InData.FloatData.Add(Elem->GetElementMeshColor().R);
				InData.FloatData.Add(Elem->GetElementMeshColor().G);
				InData.FloatData.Add(Elem->GetElementMeshColor().B);
				InData.FloatData.Add(Elem->GetElementMeshColor().A);
				
				//UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage] : SaveGameMap, ElementID = %d"), Elem->GetElementID());
			}
		}

		FServerManage::Get()->Send<FUGC_SAVE_MAP_INFO_REQ>(SP_C2D_UGC_SAVE_MAP_INFO_REQ, &InData);

	}
}

void UGameMapManage::LoadMapDataAndSpawnForFPS(const FString& InSlotName, UWorld* InWorld)
{
	if (NameToMapID.Num() > 0)
	{
		//获取element信息并生成
		FServerManage::Get()->AddCallback<FUGC_MAP_ELEMENT_INFO_RESPONSE>(SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, UGameMapManage::OnUGCElementInfo);
		FUGC_MAP_ELEMENT_INFO_REQUEST args(NameToMapID[InSlotName]);
		FServerManage::Get()->Send<FUGC_MAP_ELEMENT_INFO_REQUEST>(SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, &args);

		LoadMapDataAndSpawn(InSlotName, InWorld, false);
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&, InWorld]()
			{
				LoadMapDataAndSpawnForFPS(InSlotName, InWorld);
			});
	}
}

void UGameMapManage::LoadMapDataAndSpawnForUGC(const FString& InSlotName, UWorld* InWorld)
{
	if (NameToMapID.Num() > 0)
	{
		//获取element信息并生成
		FServerManage::Get()->AddCallback<FUGC_MAP_ELEMENT_INFO_RESPONSE>(SP_D2C_UGC_MAP_ELEMENT_INFO_RESPONSE, UGameMapManage::OnUGCElementInfo);
		FUGC_MAP_ELEMENT_INFO_REQUEST args(NameToMapID[InSlotName]);
		FServerManage::Get()->Send<FUGC_MAP_ELEMENT_INFO_REQUEST>(SP_C2D_UGC_MAP_ELEMENT_INFO_REQUEST, &args);
	
		LoadMapDataAndSpawn(InSlotName, InWorld, true);
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&, InWorld]()
			{
				LoadMapDataAndSpawnForUGC(InSlotName, InWorld);
			});
	}
}

const TArray<FString>& UGameMapManage::GetMapNameArray()
{
	MapNames.Empty();

	for (auto& Tmp : NameToMapID)
	{
		MapNames.Add(Tmp.Key);
	}

	return MapNames;
}

void UGameMapManage::OnUGCElementInfo(FUGC_MAP_ELEMENT_INFO_RESPONSE InData)
{
	TmpElements.Empty();

	int32 IntIdx = 0, FloatIdx = 0;
	int32 IntLen = InData.IntData.Num();
	int32 FloatLen = InData.FloatData.Num();
	for (; IntIdx < IntLen && FloatIdx < FloatLen; IntIdx += 2, FloatIdx += 13)
	{
		FElemInfo Data;
		Data.ElementID = InData.IntData[IntIdx];
		Data.TeamType = (ETeamType)InData.IntData[IntIdx + 1];
		Data.Location = FVector(InData.FloatData[FloatIdx], InData.FloatData[FloatIdx + 1], InData.FloatData[FloatIdx + 2]);
		Data.Rotation = FRotator(InData.FloatData[FloatIdx + 3], InData.FloatData[FloatIdx + 4], InData.FloatData[FloatIdx + 5]);
		Data.Scale = FVector(InData.FloatData[FloatIdx + 6], InData.FloatData[FloatIdx + 7], InData.FloatData[FloatIdx + 8]);
		Data.Color = FLinearColor(InData.FloatData[FloatIdx + 9], InData.FloatData[FloatIdx + 10], InData.FloatData[FloatIdx + 11], InData.FloatData[FloatIdx + 12]);
	
		TmpElements.Add(Data);
	}
}

void UGameMapManage::OnUGCMapInfo(FUGC_MAP_INFO_RESPONSE InData)
{
	NameToMapID.Empty();

	int32 Len = InData.MapIDs.Num();
	for (int32 i = 0; i < Len; i++)
	{
		NameToMapID.Add(InData.MapNames[i], InData.MapIDs[i]);
	}
}

void UGameMapManage::LoadMapDataAndSpawn(const FString& InSlotName, UWorld* InWorld, bool InbShowEffectMesh)
{
	if (TmpElements.Num() > 0)
	{
		if (InWorld)
		{
			if (AUGCGameState * MyGameState = MethodUnit::GetGameState(InWorld))
			{
				//生成Elements BUG
				for (auto& Tmp : TmpElements)
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

				TmpElements.Empty();
			}

			////从文件中加载属性
			//if (UMapSaveData * SaveMapData = Cast<UMapSaveData>(UGameplayStatics::LoadGameFromSlot(InSlotName, 0)))
			//{
			//	if (SaveMapData->Elements.Num() > 0)
			//	{

			//	}
			//	else
			//	{
			//		UE_LOG(LogTemp, Error, TEXT("[class UGameMapManage] : SaveMapData->Elements Is Empty"));
			//		return;
			//	}
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Error, TEXT("[class UGameMapManage] : SaveMapData Is Null"));
			//	return;
			//}
		}
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&, InWorld]()
			{
				LoadMapDataAndSpawn(InSlotName, InWorld, InbShowEffectMesh);
			});
	}
}

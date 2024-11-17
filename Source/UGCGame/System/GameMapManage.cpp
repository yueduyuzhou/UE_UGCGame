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
#include "../FPSGame/Core/FPSGameModeBase.h"
#include "../FPSGame/FPSGameGameState.h"

TSharedPtr<UGameMapManage> UGameMapManage::GameMapManage = nullptr;
TMap<FString, int32> UGameMapManage::NameToMapID;
TArray<FElemInfo> UGameMapManage::TmpElements;

UGameMapManage::UGameMapManage()
	:TmpCount(0)
{
	//��ȡMapList��Ϣ
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
		// ��������
		MyGameInstance->LoadMapName = InOpenMapName;

		UGameplayStatics::OpenLevel(InWorld, FName(TEXT("GameTemplateMap")), true, TEXT("listen"));
	}
}

void UGameMapManage::OpenMapForUGC(UWorld* InWorld, FString InOpenMapName)
{
	if (UUGCGameInstance * MyGameInstance = Cast<UUGCGameInstance>(InWorld->GetGameInstance()))
	{
		// ��������
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
		//		//������Ϸ���ݣ�TODO:TO BDServer��
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
		//	//������Ϸ���ݣ�TODO:TO BDServer��
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

		//������Ϸ���ݵ� BDServer
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
		//��ȡelement��Ϣ������
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
		//��ȡelement��Ϣ������
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
				TmpCount = 0;
				//����Elements BUG
				for (auto& Tmp : TmpElements)
				{
					//�ӱ��ж�ȡÿ��Element������
					if (const FElementAttribute * ElementAttr = MyGameState->GetElementAttributeTemplate(Tmp.ElementID))
					{
						//���ɶ�ӦElement
						if (AElementBase * MewElement = InWorld->SpawnActor<AElementBase>(ElementAttr->ElementClass, Tmp.Location, Tmp.Rotation))
						{
							//���ñ��������
							MewElement->SetElementID(Tmp.ElementID);
							MewElement->SetActorScale3D(Tmp.Scale);
							MewElement->SetTeamType(Tmp.TeamType);

							//TODO:���������
							if (ABuildElement * BElement = Cast<ABuildElement>(MewElement))
							{
								BElement->SetElementMesh(ElementAttr->ElementMeth);
								BElement->SetMeshColorMulticast(Tmp.Color);
							}
							else if (AEffectElement * EElement = Cast<AEffectElement>(MewElement))
							{
								EElement->SetElementMeshVisibility(InbShowEffectMesh);
							}

							//��¼��������
							TmpCount++;
							MyGameState->AddSpawnData(Tmp.ElementID);
							//UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::LoadMapDataAndSpawn] : SpawnActor Id = %d"), Tmp.ElementID);
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("[class UGameMapManage] : GetElementAttributeTemplate fail, ElementID = %d"), Tmp.ElementID);
					}
				}

				bool Success = RecycleCheckSpwnedElementsByMode(InWorld);

				//֪ͨGameMode��ϷMap�Ѿ�׼����
				if (TmpCount >= TmpElements.Num() && Success)
				{
					if (AFPSGameGameMode * FPSGM = Cast<AFPSGameGameMode>(MyGameState->AuthorityGameMode))
					{
						FPSGM->ChangeMapReadyState(true);
					}
				}
				else
				{
					UE_LOG(
						LogTemp, 
						Display, 
						TEXT("[class UGameMapManage::LoadMapDataAndSpawn] : Spawn %s , Element Count = %d, "), 
						Success ? TEXT("Success") : TEXT("Fail"), 
						TmpCount);
				}
				TmpElements.Empty();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::LoadMapDataAndSpawn] : Current TmpElements Is Empty, So We Need Delay Call This Function[LoadMapDataAndSpawn] Again"));
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&, InWorld]()
			{
				LoadMapDataAndSpawn(InSlotName, InWorld, InbShowEffectMesh);
			});
	}
}

bool UGameMapManage::RecycleCheckSpwnedElementsByMode(UWorld* InWorld)
{
	int32 RecycleNum = 10, RecycleCount = 0;
	bool SpwnSuccess = false;

	if (AFPSGameStateBase * FPSGS = Cast<AFPSGameStateBase>(InWorld->GetGameState()))
	{
		if (FFPSGameConfig * Cfg = FPSGS->GetGameConfigTemplate())
		{
			//UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::RecycleCheckSpwnedElementsByMode] : 1"));

			//ѭ�����
			while (RecycleCount < RecycleNum)
			{
				//UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::RecycleCheckSpwnedElementsByMode] : 2"));

				//1.�ռ�
				TMap<int32, int32> EleDic;
				TArray<AActor*> Elements;
				UGameplayStatics::GetAllActorsOfClass(InWorld, AElementBase::StaticClass(), Elements);
				for (auto& Tmp : Elements)
				{
					if (AElementBase * Elem = Cast<AElementBase>(Tmp))
					{
						int32 Key = Elem->GetElementID();
						if (EleDic.Contains(Key))
							EleDic[Key]++;
						else
							EleDic.Add(Key, 1);
					}
				}

				/*UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::RecycleCheckSpwnedElementsByMode] : 3"));
				for(auto& Tmp : EleDic)
					UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::RecycleCheckSpwnedElementsByMode] : %d : %d"), Tmp.Key, Tmp.Value);*/


				//2.���
				SpwnSuccess = true;
				for (auto& Tmp : Cfg->EssentialEle)
				{
					int32 SpawnNum = 0;
					if (EleDic.Contains(Tmp.Key))
					{
						if (EleDic[Tmp.Key] < Tmp.Value)
						{
							SpawnNum = Tmp.Value - EleDic[Tmp.Key];
						}
					}
					else
					{
						SpawnNum = Tmp.Value;
					}

					if (SpawnNum)
					{
						//UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::RecycleCheckSpwnedElementsByMode] : EleDic[Tmp.Key] = %d Tmp.Value = %d, SpawnNum = %d"), EleDic[Tmp.Key], Tmp.Value, SpawnNum);
						SpawnElementBy(SpawnNum, Tmp.Key, InWorld);
						SpwnSuccess = false;
					}
				}

				//UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::RecycleCheckSpwnedElementsByMode] : 4"));


				//3.�ж�
				if (SpwnSuccess) { break; }
				RecycleCount++;

				//UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::RecycleCheckSpwnedElementsByMode] : 5 RecycleCount = %d"), RecycleCount);

			}
		}
	}
	
	//UE_LOG(LogTemp, Display, TEXT("[class UGameMapManage::RecycleCheckSpwnedElementsByMode] : 6 SpwnSuccess = %d"), SpwnSuccess ? 1 : 0);

	return SpwnSuccess;
}

void UGameMapManage::SpawnElementBy(const int32& InNum, const int32& InEleID, UWorld* InWorld)
{
	if (AUGCGameState * MyGameState = MethodUnit::GetGameState(InWorld))
	{
		if (const FElementAttribute * ElementAttr = MyGameState->GetElementAttributeTemplate(InEleID))
		{
			for (int32 i = 0; i < InNum; i++)
			{
				if (AElementBase * MewElement = InWorld->SpawnActor<AElementBase>(ElementAttr->ElementClass, FVector::ZeroVector, FRotator::ZeroRotator))
				{
					//���ñ��������
					MewElement->SetElementID(InEleID);
					MewElement->SetActorScale3D(FVector(1.f));
					MewElement->SetTeamType(ETeamType::TEAM_RED);

					//TODO:���������
					if (ABuildElement * BElement = Cast<ABuildElement>(MewElement))
					{
						BElement->SetElementMesh(ElementAttr->ElementMeth);
						BElement->SetMeshColorMulticast(FLinearColor::Black);
					}
					else if (AEffectElement * EElement = Cast<AEffectElement>(MewElement))
					{
						EElement->SetElementMeshVisibility(true);
					}

					//��¼��������
					TmpCount++;
					MyGameState->AddSpawnData(InEleID);
				}
			}
		}
	}
}
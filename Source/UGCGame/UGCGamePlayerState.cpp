// Fill out your copyright notice in the Description page of Project Settings.


#include "UGCGamePlayerState.h"
#include "UGCGameState.h"
#include "Data/MisData.h"
#include "Common/MethodUnit.h"
#include "Table/SlotTable.h"
#include "ThreadManage.h"
#include "Engine/World.h"
#include "Element/BuildElement.h"
#include "Element/ElementBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/GameMapManage.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyPlayerController.h"
#include "SaveData/MapListSaveData.h"
#include "UGCGameInstance.h"
#include "Common/UGCGameType.h"
#include "Net/UnrealNetwork.h"

AUGCGamePlayerState::AUGCGamePlayerState()
	:TPlayerID(INDEX_NONE)
	, ControlElement(nullptr)
	, GridSize(10.f)
	, AngleSize(10.f)
	, RotationSpeed(10.f)
	, CurModifyType(ETransformationType::TT_Translation)
{
}

void AUGCGamePlayerState::GetInventorySlotNetPackage(const ESlotType& InType, TArray<int32>& InKeys)
{
	if (GetWorld())
	{
		if (AUGCGameState * MyGameState = MethodUnit::GetGameState(GetWorld()))
		{
			if (const TArray<FSlotTable*> * SlotData = MyGameState->GetSlotTablesTemplate())
			{
				for (auto* Tmp : (*SlotData))
				{
					if (Tmp->ContainsType(InType) && !InKeys.Contains(Tmp->ID))
					{
						InKeys.Add(Tmp->ID);
					}
				}
			}
		}
	}
}

void AUGCGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUGCGamePlayerState, CurModifyType);
}

void AUGCGamePlayerState::UpdateElementLocation(const FVector& InMouseLocation, const FVector& InMouseDirection)
{
	if (ControlElement)
	{
		//射线长度
		float RayLength = 10000.0f;

		//射线终点
		FVector EndLocation = InMouseLocation + InMouseDirection * RayLength;

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(ControlElement); // 忽略自身

		//绘制射线
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), InMouseLocation, EndLocation, FColor::Green, false, 2.0f);

		//发射射线
		if (GetWorld()->LineTraceSingleByChannel(HitResult, InMouseLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
		{
			//网格对齐移动
			ControlElement->SetActorLocation(SnapToGrid(HitResult.Location, (float)GridSize));
		}
	}
}

void AUGCGamePlayerState::UpdateElementRotation(const float& InRotationX, const float& InRotationY)
{
	if (ControlElement)
	{
		FRotator CurrentRotation = ControlElement->GetActorRotation();
		CurrentRotation.Yaw += InRotationX * RotationSpeed;
		CurrentRotation.Pitch += InRotationY * RotationSpeed;

		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, CurrentRotation.ToString());

		ControlElement->SetActorRotation(SnapToGridRotation(CurrentRotation, AngleSize));
	}
}

void AUGCGamePlayerState::TryReturnElementControl()
{
	if (ControlElement)
	{
		ControlElement->ReturnControl();
		ControlElement = nullptr;
	}
}

void AUGCGamePlayerState::SpawnElement(const int32& InPlayerID, const int32& InElementID)
{
	if (AUGCGameState * MyGameState = MethodUnit::GetGameState(GetWorld()))
	{
		if (const FElementAttribute * ElementAttr = MyGameState->GetElementAttributeTemplate(InElementID))
		{
			const int32 SpawnCount = MyGameState->GetSpawnCountByID(InElementID);
			if (ElementAttr->SpanwLimit == -1 || SpawnCount < ElementAttr->SpanwLimit)
			{
				if (AElementBase * MewElement = GetWorld()->SpawnActor<AElementBase>(ElementAttr->ElementClass, FVector::ZeroVector, FRotator::ZeroRotator))
				{
					//获取控制权
					ControlElement = MewElement;
					ControlElement->TakeControl(InPlayerID);

					//待改进
					ControlElement->SetElementID(InElementID);

					//TODO:分情况处理
					if (ABuildElement * Element = Cast<ABuildElement>(ControlElement))
					{
						Element->SetElementMesh(ElementAttr->ElementMeth);
					}

					//记录生成数量
					MyGameState->AddSpawnData(InElementID);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("[class AUGCGamePlayerState]: The number of elements with ElementID == %d has reached the Spawnlimit == %d"), InElementID, ElementAttr->SpanwLimit));
				UE_LOG(LogTemp, Error, TEXT("[class AUGCGamePlayerState]: The number of elements with ElementID == %d has reached the Spawnlimit == %d"), InElementID, ElementAttr->SpanwLimit);
			}
		}
	}
}

FVector AUGCGamePlayerState::SnapToGrid(const FVector& InOldPosition, const float& InGridSize)
{
	FVector SnappedPosition;
	SnappedPosition.X = FMath::RoundToFloat(InOldPosition.X / InGridSize) * InGridSize;
	SnappedPosition.Y = FMath::RoundToFloat(InOldPosition.Y / InGridSize) * InGridSize;
	SnappedPosition.Z = FMath::RoundToFloat(InOldPosition.Z / InGridSize) * InGridSize;
	return SnappedPosition;
}

FRotator AUGCGamePlayerState::SnapToGridRotation(const FRotator & InOldRotation, const float& InAngleSize)
{
	FRotator SnappedRotation = FRotator(
		FMath::RoundToFloat(InOldRotation.Pitch / InAngleSize) * InAngleSize,
		FMath::RoundToFloat(InOldRotation.Yaw / InAngleSize) * InAngleSize,
		FMath::RoundToFloat(InOldRotation.Roll / InAngleSize) * InAngleSize
	);
	return SnappedRotation;
}

TArray<FString> AUGCGamePlayerState::GetMapList()
{
	//TODO：从DBServer获取
	UMapListSaveData* SaveMapData = Cast<UMapListSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("MapList"), 0));
	if (SaveMapData)
	{
		return SaveMapData->Maps;
	}

	return TArray<FString>();
}

void AUGCGamePlayerState::SetModifyType(const ETransformationType & InModifyType)
{
	CurModifyType = InModifyType;
}

bool AUGCGamePlayerState::SaveMapName(const FString & InMapName)
{
	if (UUGCGameInstance * MyGameInstance = GetWorld()->GetGameInstance<UUGCGameInstance>())
	{
		MyGameInstance->LoadMapName = InMapName;
		UMapListSaveData* SaveGameInstance = Cast<UMapListSaveData>(UGameplayStatics::CreateSaveGameObject(UMapListSaveData::StaticClass()));
		if (SaveGameInstance)
		{
			TArray<FString> Maps = GetMapList();

			if (!Maps.Contains(InMapName))
			{
				Maps.Add(InMapName);
				SaveGameInstance->Maps = Maps;
				UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MapList"), 0);
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

void AUGCGamePlayerState::InitPlayerData(const int32& InPlayerID)
{
	TPlayerID = InPlayerID;
}

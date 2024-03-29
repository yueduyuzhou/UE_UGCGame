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

void AUGCGamePlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
		{
			//��ȡInventorySlotID
			TArray<int32> SlotKeys;
			GetInventorySlotNetPackage(SlotKeys);
			//�����ͻ���Inventory��ʼ��
			ServerCallClientInitInventory(SlotKeys);
		});
	}
}

void AUGCGamePlayerState::GetInventorySlotNetPackage(TArray<int32>& InKeys)
{
	if (GetWorld())
	{
		if (AUGCGameState * MyGameState = MethodUnit::GetGameState(GetWorld()))
		{
			if (const TArray<FSlotTable*> * SlotData = MyGameState->GetSlotTablesTemplate())
			{
				for (auto* Tmp : (*SlotData))
				{
					if (!InKeys.Contains(Tmp->ID))
					{
						InKeys.Add(Tmp->ID);
					}
				}
			}
		}
	}
}

void AUGCGamePlayerState::ServerCallClientInitInventory_Implementation(const TArray<int32>& InKeys)
{
	if (InitSlotDelegate.IsBound())
	{
		InitSlotDelegate.Broadcast(InKeys);
	}
}

void AUGCGamePlayerState::ServerCallClientInitPlayerData_Implementation(const int32& InPlayerID)
{
	TPlayerID = InPlayerID;
}

void AUGCGamePlayerState::RequestSaveAndQuitOnServer_Implementation()
{
	UGameMapManage::Get()->QuitAndSaveMap(GetWorld());
}

void AUGCGamePlayerState::RequestSpawnElementOnServer_Implementation(const int32& InPlayerID, const int32& InElementID)
{
	SpawnElement(InPlayerID, InElementID);
}

void AUGCGamePlayerState::UpdateElementLocationOnServer_Implementation(const FVector& InMouseLocation, const FVector& InMouseDirection)
{
	if (ControlElement)
	{
		//���߳���
		float RayLength = 10000.0f; 

		//�����յ�
		FVector EndLocation = InMouseLocation + InMouseDirection * RayLength;

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(ControlElement); // ��������

		//��������
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), InMouseLocation, EndLocation, FColor::Green, false, 2.0f);

		//��������
		if (GetWorld()->LineTraceSingleByChannel(HitResult, InMouseLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
		{
			//��������ƶ�
			ControlElement->SetActorLocation(SnapToGrid(HitResult.Location, (float)GridSize));
		}
	}
}

void AUGCGamePlayerState::UpdateElementRotationOnServer_Implementation(const float& InRotationX, const float& InRotationY)
{
	if (ControlElement)
	{
		FRotator CurrentRotation = ControlElement->GetActorRotation();
		CurrentRotation.Yaw += InRotationX * RotationSpeed;
		CurrentRotation.Pitch += InRotationY * RotationSpeed;

		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, CurrentRotation.ToString());
		// Set the new rotation to the actor
		ControlElement->SetActorRotation(SnapToGridRotation(CurrentRotation, AngleSize));
	}
}

void AUGCGamePlayerState::TryReturnElementControlOnServer_Implementation()
{
	if (ControlElement)
	{
		ControlElement->ReturnControl();
		ControlElement = nullptr;
	}
}

void AUGCGamePlayerState::TryGetElementControlOnServer_Implementation(const FVector& InMouseLocation, const FVector& InMouseDirection)
{
	//���߳���
	float RayLength = 1000.0f; 

	//�����յ�
	FVector EndLocation = InMouseLocation + InMouseDirection * RayLength;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	//��������
	if (GetWorld()->LineTraceSingleByChannel(HitResult, InMouseLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		if (AElementBase* TmpElement = Cast<AElementBase>(HitResult.Actor))
		{
			ControlElement = TmpElement;
			ControlElement->TakeControl(TPlayerID);
		}
	}
}

void AUGCGamePlayerState::RequestChangeElementModifyValueOnServer_Implementation(const int32& InValue, const EElementModifyType& InModifyType)
{
	if (InModifyType == EElementModifyType::MODIFY_LOCATION)
	{
		GridSize = InValue;
	}
	else if (InModifyType == EElementModifyType::MODIFY_ROTATION)
	{
		AngleSize = InValue;
	}
}

void AUGCGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUGCGamePlayerState, CurModifyType);
}

void AUGCGamePlayerState::SpawnElement(const int32& InPlayerID, const int32& InElementID)
{
	if (AUGCGameState * MyGameState = MethodUnit::GetGameState(GetWorld()))
	{
		if (const FElementAttribute * ElementAttr = MyGameState->GetElementAttributeTemplate(InElementID))
		{
			if (AElementBase * MewElement = GetWorld()->SpawnActor<AElementBase>(ElementAttr->ElementClass, FVector::ZeroVector, FRotator::ZeroRotator))
			{
				ControlElement = MewElement;
				ControlElement->TakeControl(InPlayerID);

				//���Ľ�
				ControlElement->SetElementID(InElementID);

				//TODO:���������
				if (ABuildElement * Element = Cast<ABuildElement>(ControlElement))
				{
					Element->SetElementMesh(ElementAttr->ElementMeth);
				}
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

FRotator AUGCGamePlayerState::SnapToGridRotation(const FRotator& InOldRotation, const float& InAngleSize)
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
	UMapListSaveData* SaveMapData = Cast<UMapListSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("MapList"), 0));
	if (SaveMapData)
	{
		return SaveMapData->Maps;
	}

	return TArray<FString>();
}

void AUGCGamePlayerState::SetModifyType(const ETransformationType& InModifyType)
{
	CurModifyType = InModifyType;
}

bool AUGCGamePlayerState::SaveMapName(const FString& InMapName)
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

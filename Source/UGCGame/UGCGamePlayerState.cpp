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

void AUGCGamePlayerState::RequestSpawnElementOnServer_Implementation(const int32& InPlayerID, const int32& InElementID)
{
	if (AUGCGameState * MyGameState = MethodUnit::GetGameState(GetWorld()))
	{
		if (const FElementAttribute * ElementAttr = MyGameState->GetElementAttributeTemplate(InElementID))
		{
			if (AElementBase* MewElement = GetWorld()->SpawnActor<AElementBase>(ElementAttr->ElementClass, FVector::ZeroVector, FRotator::ZeroRotator))
			{
				ControlElement = MewElement;
				ControlElement->TakeControl(InPlayerID);

				//TODO:���������
				if (ABuildElement * Element = Cast<ABuildElement>(ControlElement))
				{
					Element->SetElementMesh(ElementAttr->ElementMeth);
				}
			}
		}
	}
}

void AUGCGamePlayerState::UpdateElementLocationOnServer_Implementation(const FVector& InMouseLocation, const FVector& InMouseDirection)
{
	if (ControlElement)
	{
		//���߳���
		float RayLength = 1000.0f; 

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
			//TODO:��������ƶ�
			ControlElement->SetActorLocation(HitResult.Location);
		}
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
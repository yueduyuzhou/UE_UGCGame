// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Settings.h"
#include "Components/Button.h"
#include "../../../Common/MethodUnit.h"
#include "../../../System/GameMapManage.h"
#include "../../../UGCGameInstance.h"

void UUI_Settings::OnClickedWidget()
{
	if (UUGCGameInstance * GameInstance = GetWorld()->GetGameInstance<UUGCGameInstance>())
	{
		if (AUGCGamePlayerState * MyPlayerState = MethodUnit::GetPlayerState(GetWorld()))
		{
			if (MyPlayerState->GetLocalRole() == ROLE_Authority)
			{
				//ע�ᵽ��ͼ�б�
				/*if (!MyPlayerState->SaveMapName(GameInstance->LoadMapName))
				{
					UE_LOG(LogTemp, Error, TEXT("[class UUI_Settings] : OnClickedWidget, The Map Name Already Exists"));
				}*/

				//��ͼLobby��
				if (AUGCGamePlayerController * UGCPC = MethodUnit::GetPlayerController(GetWorld()))
				{
					UGCPC->CaptureLobbyTexture(GameInstance->LoadMapName);
				}

				//�����ͼ����
				UGameMapManage::Get()->QuitAndSaveMap(GetWorld());
			}
		}
	}
}

void UUI_Settings::NativeConstruct()
{
	Super::NativeConstruct();
}
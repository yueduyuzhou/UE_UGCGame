// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/Game/UI_Crosshair.h"
#include "UI/Game/MiniMap/UI_MiniMap.h"
#include "UI/Game/DownTime/UI_DownTime.h"
#include "UI/Game/TopInfoPanel/UI_TopInfoPanel.h"
#include "UGCGame/Common/UGCGameType.h"
#include "UGCGame/Common/RenderingUtils.h"
#include "FPSGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	friend class AFPSGameGameMode;

	AFPSGamePlayerController();

	virtual void SetupInputComponent() override;

private:
	void OnLeftMouseButtonDown();
	void OnLeftMouseButtonUp();

	void OnRightMouseButtonDown();
	void OnRightMouseButtonUp();

	void Jump();

	void OnLeftShiftButtonDown();
	void OnLeftShiftButtonUp();

	void AmmoReload();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

public:
	/***********************************************************************
	*	RPC
	***********************************************************************/
	UFUNCTION(client, reliable)
		void ServerCallClientSendPlayerData();

	UFUNCTION(client, reliable)
		void ServerCallClientUpdateMiniMap(const FString& InMapName);

	UFUNCTION(client, reliable)
		void ServerCallClientUpdateDownTime(const int32& InDownTime);
	
	UFUNCTION(client, reliable)
		void ServerCallClientUpdateKillText(const ETeamType& InTeamType);

	/*”Œœ∑Ω· ¯*/
	UFUNCTION(client, reliable)
		void ServerCallClientEndGame(const ETeamType& InWinTeam, const TArray<FFPSPlayerInfo>& InPlayerInfos);
	
	UFUNCTION(server, reliable)
		void SendPlayerDataToServer(const FPlayerNetData& InPlayerData);

public:
	/***********************************************************************
	*	
	***********************************************************************/
	void SpawnPlayerCharacter(UClass* InCharacterClass, const FTransform& InTransform);

	void ControllerCharacterDeath(AActor* InDamageType);

	/***********************************************************************
	*	Weapon
	***********************************************************************/
	void PlayerCameraShake(TSubclassOf<UCameraShake> InCameraShake);
	void CrosshairRecoil();

	void UpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo);
	void UpdateHealth(const float& InHealth, const float& InMaxHealth);

	/***********************************************************************
	*	Blueprint
	***********************************************************************/
	UFUNCTION(BlueprintImplementableEvent)
		void EndGame(const ETeamType& InWinTeam, const TArray<FFPSPlayerInfo>& InPlayerInfos);

	UFUNCTION(BlueprintCallable)
		bool IsAuthority();

public:
	UUI_Crosshair* CrosshairUI;
	UUI_MiniMap* MiniMapUI;
	UUI_DownTime* DownTimeUI;
	UUI_TopInfoPanel* TopInfoPanelUI;

private:
	ETeamType TeamType;
	int32 PlayerID;

	FMinimapCapture MapCapture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float ReSpawnTime;
};

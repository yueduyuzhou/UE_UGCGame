// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameGameMode.generated.h"

class AFPSGameCharacterBase;
class AFPSGamePlayerController;
class AEE_SpawnPoint;
struct FPlayerNetData;
struct FFPSPlayerInfo;
enum class ETeamType : uint8;

/**
 * 
 */
UCLASS()
class UGCGAME_API AFPSGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameGameMode();

private:
	/***************
	*	GameCharacter
	***************/
	//生成游戏角色
	void SpawnPlayerCharacters();

	/***************
	*	Game
	***************/
	/*开始倒计时*/
	void InitDownTime();

	/*通知所有玩家开始倒计时*/
	void AllPlayerUpdateDownTime(const int32& InDownTime);

	/*游戏结束*/
	void EndGame();

	/*通知所有玩家游戏结束*/
	UFUNCTION(BlueprintCallable)
	void AllClientEndGame(const ETeamType& InWinTeam, const TArray<FFPSPlayerInfo>& InPlayerInfos);

public:

	/*处理游戏角色死亡*/
	void GameCharacterDeath(const int32& InKillerID, const int32& InKilledID);

	/*通知所有玩家更新MiniMap*/
	void AllPlayerUpdateMiniMap();

	/*通知所有玩家更新MiniMap*/
	void AllPlayerUpdateKillText(const ETeamType& InTeamType);

	/***************
	*	属性
	***************/
	const FTransform GetNextSpawnTransform(const FPlayerNetData& InPlayerData);

	/*获取本地PlayerController*/
	AFPSGamePlayerController* GetLocalPlayerController();

	//AFPSGamePlayerController* GetPlayerControllerByPlayerID(const int32& InPlayerID);
	UClass* GetCharacterClass(const ETeamType& InType);

	void AddSpawnCount();
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetSpawnCount() { return PlayerSpawnCount; }

	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFPSGameCharacterBase> RedCharacterClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AFPSGameCharacterBase> BlueCharacterClass;

	TArray<AEE_SpawnPoint*> RedSpawnPoints;
	TArray<AEE_SpawnPoint*> BlueSpawnPoints;
	int32 RedIndex;
	int32 BlueIndex;

	int32 PlayerSpawnCount;

	bool bStartDownTime;
	float DownTime;
};

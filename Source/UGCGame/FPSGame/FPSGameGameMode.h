// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameGameMode.generated.h"

class AFPSGameCharacterBase;
class AFPSGamePlayerController;
class AEE_SpawnPoint;
struct FPlayerNetData;

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
	void SpawnPlayerCharacters();

public:
	const FTransform GetNextSpawnTransform(const FPlayerNetData& InPlayerData);

	//AFPSGamePlayerController* GetPlayerControllerByPlayerID(const int32& InPlayerID);
	UClass* GetCharacterClass(const ETeamType& InType);

	void AddSpawnCount();
	FORCEINLINE const int32& GetSpawnCount() { return PlayerSpawnCount; }

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer);

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
};

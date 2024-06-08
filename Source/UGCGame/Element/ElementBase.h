// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "TransformerPawn.h"
#include "ElementBase.generated.h"

enum class EEditDetailType : uint8;
enum class ETeamType : uint8;
class UUI_DetailVector;

UCLASS()
class UGCGAME_API AElementBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElementBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetMouseLocationInWorld();

public:
	/****************
	*	
	****************/
	bool TakeControl(const int32& InControlID);
	void ReturnControl();
	void DestoryElement();

	void UpdateVectorUI();

	void RegisterDetailVectorByType(UUI_DetailVector* InUI);

	/****************
	*	template
	****************/
	template<typename T>
	bool GetMemberVariableByName(const FString& VarName, T& OutValue);

	/****************
	*	
	****************/
	FORCEINLINE const ETeamType& GetTeamType() { return TeamType; }
	void SetTeamType(const ETeamType& InTeamType) { TeamType = InTeamType; }

	FORCEINLINE const int32 GetElementID() { return ID; }
	void SetElementID(const int32& InElementID);

	const TArray<EEditDetailType>& GetEditDetails();

	UFUNCTION(BlueprintCallable, Category = "ElementColor")
	virtual void SetElementMeshColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable, Category = "ElementColor")
	virtual FLinearColor GetElementMeshColor();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:
	bool bControlled;

	UPROPERTY(Replicated)
	int32 ControllerID;

private:
	int32 ID;

	UUI_DetailVector* LocationUI;
	UUI_DetailVector* RotationUI;
	UUI_DetailVector* ScaleUI;

	UPROPERTY(EditAnywhere)
	TArray<EEditDetailType> EditDetails;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "EffectData")
	ETeamType TeamType;
};

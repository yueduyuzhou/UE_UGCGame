#include "MethodUnit.h"

namespace MethodUnit
{
	AUGCGameState* GetGameState(UWorld* InWorld)
	{
		if (InWorld)
		{
			return InWorld->GetGameState<AUGCGameState>();
		}
		return nullptr;
	}

	AUGCGamePlayerState* GetPlayerState(UWorld* InWorld)
	{
		if (InWorld)
		{
			return InWorld->GetFirstPlayerController()->GetPlayerState<AUGCGamePlayerState>();
		}
		return nullptr;
	}

	AUGCGamePawn* GetUGCPlayerPawn(UWorld* InWorld)
	{
		if (AUGCGamePlayerState * MyPlayerState = GetPlayerState(InWorld))
		{
			return MyPlayerState->GetPawn<AUGCGamePawn>();
		}
		return nullptr;
	}

	AUGCGamePlayerController* GetPlayerControllerByPlayerID(UWorld* InWorld, int32& InPlayerID)
	{
		if (InWorld)
		{
			// 获取所有PlayerController
			for (FConstPlayerControllerIterator It = InWorld->GetPlayerControllerIterator(); It; ++It)
			{
				if (AUGCGamePlayerController * PlayerController = Cast<AUGCGamePlayerController>(It->Get()))
				{
					if (AUGCGamePlayerState * NewPlayerState = PlayerController->GetPlayerState<AUGCGamePlayerState>())
					{
						if (NewPlayerState->TPlayerID == InPlayerID)
						{
							return PlayerController;
						}
					}
				}
			}
		}
		return nullptr;
	}

	const FString EditDetailTypeToString(const EEditDetailType& InType)
	{
		switch (InType)
		{
			case EEditDetailType::DETAIL_LOCATION:
			{
				return FString(TEXT("DETAIL_LOCATION"));
			}
			case EEditDetailType::DETAIL_ROTATION:
			{
				return FString(TEXT("DETAIL_ROTATION"));
			}
			case EEditDetailType::DETAIL_SCALE:
			{
				return FString(TEXT("DETAIL_SCALE"));
			}
			case EEditDetailType::DETAIL_ENUM_TEAMTYPE:
			{
				return FString(TEXT("DETAIL_ENUM_TEAMTYPE"));
			}
		}
		return FString();
	}

	const FString TeamTypeToString(const ETeamType& InType)
	{
		switch (InType)
		{
			case ETeamType::TEAM_RED:
			{
				return FString(TEXT("TeamRed"));
			}
			case ETeamType::TEAM_BLUE:
			{
				return FString(TEXT("TeamBlue"));
			}
		}
		return FString();
	}
	
}
#include "MethodUnit.h"

namespace MethodUnit
{
	AUGCGameState* GetGameState(UWorld* InWorld)
	{
		return InWorld->GetGameState<AUGCGameState>();
	}

	AUGCGamePlayerState* GetPlayerState(UWorld* InWorld)
	{
		return InWorld->GetFirstPlayerController()->GetPlayerState<AUGCGamePlayerState>();
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
	
}
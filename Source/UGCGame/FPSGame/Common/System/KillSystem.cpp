#include "KillSystem.h"

void FKillSystem::AddKiller(const int32& InPlayerID)
{
	if (!IsExistPlayer(InPlayerID))
	{
		Players.Add(FKillMode(InPlayerID));
	}

}

bool FKillSystem::IsExistPlayer(const int32& InPlayerID)
{
	for (auto& Tmp : Players)
	{
		if (Tmp.Killer == InPlayerID)
		{
			return true;
		}
	}
	return false;
}

void FKillSystem::Tick(float DeltaSeconds)
{
	TArray<FKill> Removes;
	for (auto& Tmp : CurrentKillState)
	{
		Tmp.KillTime -= DeltaSeconds;
		if (Tmp.KillTime <= 0.f)
		{
			Removes.Add(Tmp);
		}
	}

	for (auto& Tmp : Removes)
	{
		CurrentKillState.Remove(Tmp);
	}
}

void FKillSystem::Kill(const int32& InKillerPlayerID, const int32& InKilledPlayerID)
{
	//添加击杀状态
	AddKiller(InKillerPlayerID, InKilledPlayerID);

	uint8 CallKillFuncCount = 0;

	for (auto& Tmp : CurrentKillState)
	{
		if (Tmp.CurrentKiller == InKillerPlayerID)
		{
			Tmp.KillNumber++;

			//连续击杀
			switch (Tmp.KillNumber)
			{
				/*case 1:
				{
					break;
				}*/
			case 2:
			{
				DoubleKills.Add(Tmp.CurrentKiller);
				DoubkeKillsFunc(InKillerPlayerID, InKilledPlayerID);
				CallKillFuncCount = 1;
				break;
			}
			case 3:
			{
				TripleKills.Add(Tmp.CurrentKiller);
				TripleKillsFunc(InKillerPlayerID, InKilledPlayerID);
				CallKillFuncCount = 1;
				break;
			}
			case 4:
			{
				QuadraKills.Add(Tmp.CurrentKiller);
				QuadraKillsFunc(InKillerPlayerID, InKilledPlayerID);
				CallKillFuncCount = 1;
				break;
			}
			case 5:
			{
				PentaKills.Add(Tmp.CurrentKiller);
				PentaKillsFunc(InKillerPlayerID, InKilledPlayerID);
				CallKillFuncCount = 1;
				break;
			}
			}

			//一血
			if (FirstBlood == INDEX_NONE)
			{
				FirstBlood = InKillerPlayerID;
				FirstBloodFunc(InKillerPlayerID, InKilledPlayerID);
				CallKillFuncCount = 1;
			}

			//无人能挡 超神
			for (auto& TmpPlayer : Players)
			{
				if (TmpPlayer.Killer == InKillerPlayerID)
				{
					TmpPlayer.KillNumber++;

					if (CallKillFuncCount == 0)
					{
						if (TmpPlayer.KillNumber >= 3 && TmpPlayer.KillNumber < 6)
						{
							Invincible.Add(InKillerPlayerID);
							InvincibleFunc(InKillerPlayerID, InKilledPlayerID);
							CallKillFuncCount = 1;
						}
						else if (TmpPlayer.KillNumber >= 6)
						{
							Legendary.Add(InKillerPlayerID);
							LegendaryFunc(InKillerPlayerID, InKilledPlayerID);
							CallKillFuncCount = 1;
						}
					}

					break;
				}
			}

			if (CallKillFuncCount == 0)
			{
				KillFunc(InKillerPlayerID, InKilledPlayerID);
			}

			break;
		}
	}

	AceFunc(InKillerPlayerID, InKilledPlayerID);
}

void FKillSystem::Death(const int32& InKilledPlayerID)
{
	for (auto& Tmp : Players)
	{
		if (Tmp.Killer == InKilledPlayerID)
		{
			Tmp.KillNumber = 0;
			Tmp.Killed = INDEX_NONE;
			break;
		}
	}

	for (auto& Tmp : CurrentKillState)
	{
		if (Tmp.CurrentKiller == InKilledPlayerID)
		{
			Tmp.KillTime = 0.f;
			break;
		}
	}
}

void FKillSystem::AddKiller(const int32& InKillerPlayerID, const int32& InKilledPlayerID)
{
	if (CurrentKillState.Contains(InKillerPlayerID))
	{
		for (auto& Tmp : CurrentKillState)
		{
			if (Tmp.CurrentKiller == InKillerPlayerID)
			{
				Tmp.ResetKillTime();
				break;
			}
		}
	}
	else
	{
		CurrentKillState.Add(FKill(InKillerPlayerID, InKilledPlayerID));
	}


}

FKillSystem::FKillSystem()
	:FirstBlood(INDEX_NONE)
{
}

FKillSystem::FKillMode::FKillMode(const int32& InKiller)
	: KillNumber(0),
	Killer(InKiller),
	Killed(INDEX_NONE)
{
}

FKillSystem::FKill::FKill(const int32& InKillerPlayerID, const int32& InKilledPlayerID)
	:CurrentKiller(InKillerPlayerID),
	CurrentKilled(InKilledPlayerID),
	KillNumber(0)
{
	ResetKillTime();
}

void FKillSystem::FKill::ResetKillTime()
{
	KillTime = 20.f;
}
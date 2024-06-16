// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSaveData.h"

bool UPlayerSaveData::ItemContains(const int32& InID)
{
	for (auto& Tmp : Items)
	{
		if (Tmp.ItemID == InID)
		{
			return true;
		}
	}
	return false;
}
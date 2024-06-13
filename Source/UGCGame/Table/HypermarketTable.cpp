#include "HypermarketTable.h"

FHypermarketTable::FHypermarketTable()
	:FUGCGameTableBase()
{
}

bool FHypermarketTable::ContainsType(const EHypermarkType& InType)
{
	for (auto& Tmp : Types)
	{
		if (Tmp == InType) { return true; }
	}
	return false;
}

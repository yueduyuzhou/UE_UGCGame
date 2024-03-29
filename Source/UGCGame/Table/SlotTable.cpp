#include "SlotTable.h"

FSlotTable::FSlotTable()
	:FUGCGameTableBase()
{
}

bool FSlotTable::ContainsType(const ESlotType& InType)
{
	for (auto& Tmp : Types)
	{
		if (Tmp == InType) { return true; }
	}
	return false;
}

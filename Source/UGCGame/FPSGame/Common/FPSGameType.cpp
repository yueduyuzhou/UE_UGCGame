#include "FPSGameType.h"

void FAssisterQueue::AddAssister(int32 InAssisterID)
{
	if (IsMax()) { PopFront(); }

	Assisters[TailIndex] = InAssisterID;
	TailIndex = (TailIndex + 1) % LimitLength;
	CurLength++;
}

void FAssisterQueue::PopFront()
{
	if (!IsEmpty())
	{
		HeadIndex = (HeadIndex + 1) % LimitLength;
	}
	CurLength--;
}

void FAssisterQueue::PopTail()
{
	if (!IsEmpty())
	{
		TailIndex = (TailIndex - 1 + LimitLength) % LimitLength;
	}
	CurLength--;
}

int32 FAssisterQueue::Front()
{
	if (!IsEmpty())
	{
		return Assisters[HeadIndex];
	}
	return INDEX_NONE;
}

int32 FAssisterQueue::Tail()
{
	if (!IsEmpty())
	{
		return Assisters[(TailIndex - 1 + LimitLength) % LimitLength];
	}
	return INDEX_NONE;
}

void FAssisterQueue::ResetQueue()
{
	CurLength = HeadIndex = TailIndex = 0;
}

bool FAssisterQueue::IsMax()
{
	return CurLength >= LimitLength;
}

bool FAssisterQueue::IsEmpty()
{
	UE_LOG(LogTemp, Display, TEXT("[class FAssisterQueue] CurLength=%d"), CurLength);
	return CurLength <= 0;
}

int32 FAssisterQueue::GetLength()
{
	return CurLength;
}

void FAssisterQueue::PrintQueue()
{
	for (int32 i = 0; i < Assisters.Num(); i++)
	{
		UE_LOG(LogTemp, Display, TEXT("Assister%d : %d"), i, Assisters[i]);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Assister%d : %d"), i, Assisters[i]));
	}
}

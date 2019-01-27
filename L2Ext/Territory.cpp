#include "stdafx.h"
#include "Territory.h"

bool CTerritory::IsInisideRough(FVector *pos, bool checkHeight)
{
	typedef bool (*f)(CTerritory*, FVector*, bool);
	return f(0x7EF570L)(this, pos, checkHeight);
}

bool CTerritory::IsInside(FVector *pos, bool checkHeight)
{
	typedef bool (*f)(CTerritory*, FVector*, bool);
	return f(0x7EF6B0L)(this, pos, checkHeight);
}

FVector* CTerritory::PickRandomPos(FVector *pRandomPos)
{
	typedef FVector*(*f)(CTerritory*, FVector*);
	return f(0x7EF9B0L)(this, pRandomPos);
}
#pragma once
#include "MemoryObject.h"

//78h size
class CTerritory : public MemoryObject
{
	/* 18 */ LPVOID _unkn18;
	/* 20 */ LPVOID _unkn20;
	/* 28 */ LPVOID _unkn28;
	/* 30 */ LPVOID _unkn30;
	/* 38 */ LPVOID _unkn38;
	/* 40 */ vector<FVector*> vPoints;
	/* 60 */ INT32 minZ;
	/* 64 */ INT32 maxZ;
	/* 68 */ INT32 minX;
	/* 6C */ INT32 maxX;
	/* 70 */ INT32 minY;
	/* 74 */ INT32 maxY;

	bool IsInisideRough(FVector *pos, bool checkHeight);
	bool IsInside(FVector* pos, bool checkHeight);
	FVector* PickRandomPos(FVector *pRandomPos);
};
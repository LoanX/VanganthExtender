#pragma once

#include "CreatureSharedData.h"
#include "CPledgeSharedData.h"
#include "StaticObjectData.h"

class CSharedFactory
{
public:
	static void Initialize();
	static void ExtendCreatureSharedData();
	static void ExtendPledgeSharedData();
	static void ReallocateAbnormalTable();
	static void ReallocateQuests();
	static void CheckIndexesForCreature();
};

//File name  L2World_CreatureConstant
class CCreatureSharedFactory
{
public:
	/*000*/ HANDLE hCreatureData;	//handle to L2World_CreatureData
	/*008*/ CreatureSharedData* lpCreatureData;	//pointer to memory of L2World_CreatureData
	/*010*/ CIOCriticalSection _lock;
	/*040*/ HANDLE hCreatureConstant;
	/*048*/ LPVOID lpCreatureConstant;
	/*050 */ LPVOID unkn50;
	/*058 */ HANDLE hCreatureIndex;
	/*060 */ LPDWORD lpCreatureIndex;
	/*068 */ int firstIndex;
	/*06C */ int lastIndex;
	/*070 */ CIOCriticalSection** lpCenteredLock;
	/*078 */ int addedCount;
	/*07C */ int removedCount;
	/*080 */ int totalCount;
	/*084 */ int remainCount;
	CreatureSharedData* Allocate(CCreature *pCreature);
	BOOL Free(int serverIndex);
};

#define CRASH_MACRO LPBYTE lpCrashInfo = (LPBYTE)0x157A12D; (*lpCrashInfo) = 1;	LPDWORD lpNullPtr = NULL; (*lpNullPtr) = 1;
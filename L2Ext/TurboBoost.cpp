#include "StdAfx.h"

LPCRITICAL_SECTION lpSmartQueueLock;
LPCRITICAL_SECTION lpNpcEnterWorldPush;
LPCRITICAL_SECTION lpDeadlockCheckerLock;

LPVOID sub_66F2A0(LPVOID p1, LPVOID p2, DWORD n3, DWORD n4)
{
	typedef LPVOID (*f)(LPVOID, LPVOID, DWORD, DWORD);
	EnterCriticalSection(lpSmartQueueLock);
	LPVOID pMem = f(0x66F2A0L)(p1, p2, n3, n4);
	LeaveCriticalSection(lpSmartQueueLock);
	return pMem;
}

//void Push(int index, int x, int y, int z, int dir, int nSetDefaultParam, int nHP, int nMP)
void sub_6EC0B0(LPVOID pInstance, int index, int x, int y, int z, int dir, int nSetDefaultParam, int nHP, int nMP)
{
	EnterCriticalSection(lpNpcEnterWorldPush);
	typedef void (*f)(LPVOID, int, int, int, int, int, int, int, int);
	f(0x6EC0B0L)(pInstance, index, x, y, z, dir, nSetDefaultParam, nHP, nMP);
	LeaveCriticalSection(lpNpcEnterWorldPush);
}
UINT g_IODeadlockCheckerThreadId = 0;
UINT g_IODeadlockCheckerThreadIndex = 0;

void CIODeadlockChecker_TimerExpired(LPVOID param, int id)
{
	EnterCriticalSection(lpDeadlockCheckerLock);
	//g_Log.Add(CLog::Blue, "[%s] [%d] [%d] [%d]", __FUNCTION__, GetThreadIndex(), GetCurrentThreadId(), GetTickCount());
	DWORD threadId = GetCurrentThreadId();
	if(threadId != g_IODeadlockCheckerThreadId)
	{
		UINT threadIndex = GetThreadIndex();
		LPUINT lpLastTick = (LPUINT)0x39A8400L;
		g_Log.Add(CLog::Blue, "[IODeadlockChecker] Switching threadId[%d] -> [%d] index[%d] -> [%d] CurrentTick[%d] LastTick[%d]", g_IODeadlockCheckerThreadId, threadId, g_IODeadlockCheckerThreadIndex, threadIndex, (UINT)GetTickCount(), (*lpLastTick));
		g_IODeadlockCheckerThreadId = threadId;
		g_IODeadlockCheckerThreadIndex = threadIndex;
	}

	typedef void(*f)(LPVOID, int);
	f(0x626400L)(param, id);
	LeaveCriticalSection(lpDeadlockCheckerLock);
}

DWORD CIODeadlockChecker_GetDiff()
{
	DWORD diff = 0;
	EnterCriticalSection(lpDeadlockCheckerLock);
	LPUINT lpLastTick = (LPUINT)0x39A8400L;
	UINT currentTick = GetTickCount();
	UINT lastTick = (*lpLastTick);
	if(currentTick < INT_MAX && lastTick > INT_MAX)
	{
		//rolled back to 0
		(*lpLastTick) = currentTick;
		lastTick = currentTick;
	}

	diff = currentTick - lastTick;
	LeaveCriticalSection(lpDeadlockCheckerLock);
	//g_Log.Add(CLog::Blue, "[%s] Diff[%d]", __FUNCTION__, diff);
	return diff;
}

void CTurboBoost::Init()
{
	lpSmartQueueLock = NULL;
	lpNpcEnterWorldPush = NULL;
	
	lpDeadlockCheckerLock = new CRITICAL_SECTION;
	InitializeCriticalSection(lpDeadlockCheckerLock);
	g_HookManager.WriteMemoryDWORD(0xA03DC0, (DWORD)CIODeadlockChecker_TimerExpired);
	g_HookManager.WriteCall(0x623E73, CIODeadlockChecker_GetDiff, 1);
	g_HookManager.WriteMemoryBYTE(0x623E79, 0x90);	//cmp ecx, 60000 to cmp eax, 60000
	g_HookManager.WriteMemoryBYTE(0x623E7A, 0x3D);
	g_HookManager.WriteMemoryDWORD(0x623E7B, 80000);	//deadlock max time

	if(g_Config.L2ServerConfig.IsTrueBoostFix())
	{
		lpSmartQueueLock = new CRITICAL_SECTION;
		lpNpcEnterWorldPush = new CRITICAL_SECTION;
		InitializeCriticalSection(lpSmartQueueLock);
		InitializeCriticalSection(lpNpcEnterWorldPush);
		g_HookManager.WriteJump(0x6701B9, sub_66F2A0, 0);
		g_HookManager.WriteCall(0x6EC4BD, sub_6EC0B0, 0);
	}
}
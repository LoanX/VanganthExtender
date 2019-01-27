#include "stdafx.h"
#include "Timer.h"
#include "EraSystem.h"

void CTimer::Init()
{
	WriteQWORD(0x68A880, (UINT64)CTimer::TimerExpired);	//CIODeadlockChecker::TimerExpired
	WriteCall(0x47EFD5, CTimer::TimerDispatch, 4);
	WriteCall(0x47EFBA, CTimer::TimerDispatchEx, 4);
}

void CTimer::TimerExpired(LPVOID pInstance, int id)
{
	typedef void (*f)(LPVOID, int);
	f(0x48CBC0L)(pInstance, id);

	//Func is called every 5 seconds
	g_EraSystem.TimerExpired();
}

void CTimer::TimerDispatch(PUINT64 pObject, int id, int param, int paramEx)
{
	UINT64 vTable = 0;
	try
	{
		vTable = (*pObject);
		UINT64 address = vTable + 0x98;
		address = (*(PUINT64)address);
		typedef void(*f)(PUINT64, int, int, int);
		f((PVOID)address)(pObject, id, param, paramEx);
	}catch(...)
	{
		g_Log.Add(LogError, "[%s] Exception caught vTable[%x]", __FUNCTION__, vTable);
	}
}

//TimerExpired
void CTimer::TimerDispatchEx(PUINT64 pObject, int id)
{
	UINT64 vTable = 0;
	try
	{
		vTable = (*pObject);
		UINT64 address = vTable + 0x88;
		address = (*(PUINT64)address);
		typedef void(*f)(PUINT64, int);
		f((PVOID)address)(pObject, id);
	}catch(...)
	{
		g_Log.Add(LogError, "[%s] Exception caught vTable[%x]", __FUNCTION__, vTable);
	}
}
#pragma once
#include "Utils.h"

extern CEraSystem g_EraSystem;

class CTimer
{
public:
	static void Initialize()
	{
		WriteMemoryDWORD(0x68A880, (DWORD)TimerExpired);
	}
	static void TimerExpired(LPVOID pInstance, int id)
	{
		typedef void (*_f)(LPVOID, int);
		_f(0x48CBC0)(pInstance, id);

		//Func is called every 5 seconds
		g_EraSystem.TimerExpired();
	}
};
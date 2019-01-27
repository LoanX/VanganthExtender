#include "stdafx.h"
#include "BaseAction.h"
#include "EraSystem.h"

void CBaseAction::Init()
{
	WriteCall(0x557D6E, CBaseAction::FinishedProcessing);

	WriteNOP(0x4B2CF7, 5);
	WriteNOP(0x4BA51F, 0x32-0x1F);
	WriteNOP(0x4BA535, 5);
}

void CBaseAction::FinishedProcessing(LPVOID pLog, LogType type, const WCHAR *format, PWCHAR wEventName, DWORD Enabled, int Count, int MaxCount, int ElapsedSec)
{
	typedef void (*f)(LPVOID, LogType, const WCHAR*, PWCHAR, DWORD, int, int, int);
	f(0x4AAAE0L)(pLog, type, format, wEventName, Enabled, Count, MaxCount, ElapsedSec);
	
	if(g_EraSystem.IsEnabled())
	{
		int nEraIndex = g_EraSystem.GetEraIndex(wEventName);
		if(nEraIndex != -1)
		{
			g_Log.Add(LogBlue, "[%s] EventName[%S] Enabled[%d] EraIndex[%d]", __FUNCTION__, wEventName, Enabled, nEraIndex);
			g_EraSystem.ProcessingDone(nEraIndex, Enabled);
		}
	}
}
#pragma once
#include "Utils.h"
/*
	Up to 32 eras
*/
struct EraInfo
{
	DWORD nCurrentEra;
	DWORD nCount;
	DWORD nDuration;
	DWORD nLastChange;
	DWORD StartTime;
	DWORD UseMoon;
	DWORD MoonState[32];
	wchar_t EraNames[32][25];
};

class CEraSystem : private CThreadSafe
{
	EraInfo Data;
public:
	enum STATE
	{
		NOTHING,
		START,
		ERA_CHANGE
	};
	CEraSystem();
	void Initialize();
	int GetEraIndex(PWCHAR wName);
	void TimerExpired();
	void SetCurrentEra(int nEra);
	STATE ValidateEra();
	void ProcessingDone(int nEraIndex, bool Spawned);
};

static void FinishedProcessing(CLog *pLog, CLog::LogType Type, const wchar_t *format, PWCHAR wEventName, DWORD Enabled, int Count, int MaxCount, int ElapsedSec);

extern CEraSystem g_EraSystem;
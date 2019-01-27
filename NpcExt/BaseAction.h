#pragma once

class CBaseAction
{
public:
	static void Init();
	static void FinishedProcessing(LPVOID pLog, LogType type, const WCHAR *format, PWCHAR wEventName, DWORD Enabled, int Count, int MaxCount, int ElapsedSec);
};
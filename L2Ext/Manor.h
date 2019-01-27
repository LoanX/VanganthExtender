#pragma once

class CManor
{
public:
	static void Initialize();
	static void LogFix(CLog* pLog, CLog::LogType logType, PWCHAR wszSys, PWCHAR wszFormat, ...);
	static double GetHarvestingDice(double min, double max);
	static void AddCropToInventory(CCreature* pCreature, INT32 itemId, INT32 count, bool param1, INT32 param2, INT32 param3, INT32 param4);
	static CManor* GetManor(int manorId);
	void WriteLock(UINT line = __LINE__, const WCHAR* file = __FILEW__);
	void WriteUnlock(UINT line = __LINE__, const WCHAR* file = __FILEW__);
	void SaveNowManorInfo();
};
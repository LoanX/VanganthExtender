#include "StdAfx.h"

extern CCreature* g_Harvester[26];

void CManor::Initialize()
{
	g_HookManager.WriteCall( 0x6559D3,LogFix, 0);
	g_HookManager.WriteCall( 0x655A53,LogFix, 0);
	g_HookManager.WriteCall( 0x655AC2,LogFix, 0);
	g_HookManager.WriteCall( 0x657EAA,LogFix, 0);
	g_HookManager.WriteCall( 0x657F2C,LogFix, 0);
	g_HookManager.WriteCall( 0x657FA1,LogFix, 0);
	g_HookManager.WriteJump( 0x7D169B, (PVOID)0x7D16C3, 34); //Avoid 1 useless log when sowing null creature

	g_HookManager.WriteCall( 0x7D3BC9, CManor::GetHarvestingDice, 0);
	g_HookManager.WriteCall( 0x7D3CDD, CManor::AddCropToInventory, 0);

}

double CManor::GetHarvestingDice(double min, double max)
{
	typedef double (*f)(double, double);
	double dice = f(0x4434A0L)(min, max);

	UINT threadIndex = GetThreadIndex();
	if(threadIndex < 26)
	{
		User *pUser = g_Harvester[threadIndex]->GetUser();
		if(pUser->ValidUser())
		{
			dice /= pUser->pSD->pExData->SkillMod.bonusHarvesting;	//decrease dice -> increase chance
		}
	}

	return dice;
}

void CManor::AddCropToInventory(CCreature* pCreature, INT32 itemId, INT32 count, bool param1, INT32 param2, INT32 param3, INT32 param4)
{
	if(User *pUser = pCreature->GetUser())
	{
		double itemRate = pUser->pSD->pExData->SkillMod.bonusHarvesting;
		if(itemRate != 1.0)
		{
			double itemCount = static_cast<double>(count);
			itemCount *= itemRate;
			itemCount = floor(itemCount);
			count = static_cast<INT32>(itemCount);
		}
	}

	typedef void(*f)(CCreature*, INT32, INT32, bool, INT32, INT32, INT32);
	f(0x4EF130L)(pCreature, itemId, count, param1, param2, param3, param4);
}

void CManor::LogFix(CLog* pLog, CLog::LogType logType, PWCHAR wszSys, PWCHAR wszFormat, ...)
{
	typedef void (*_f)(CLog *, CLog::LogType, PWCHAR, va_list);
	_f f = (_f) 0x0064D340;
	va_list va;
	va_start(va, wszFormat);
	f(pLog, logType, wszFormat, va);
	va_end(va);
}

CManor* CManor::GetManor(int id)
{
	typedef CManor* (*f)(UINT64, int);
	return f(0x6589B0L)(0x6F91E60L, id);
}

void CManor::WriteLock(UINT line, const WCHAR *file)
{
	typedef void(*f)(CManor*, const WCHAR*, UINT);
	f(0x652A40L)(this, file, line);
}

void CManor::WriteUnlock(UINT line, const WCHAR *file)
{
	typedef void (*f)(CManor*, const WCHAR*, UINT);
	f(0x652BE0L)(this, file, line);
}

void CManor::SaveNowManorInfo()
{
	typedef void(*f)(CManor*);
	f(0x653590L)(this);
}
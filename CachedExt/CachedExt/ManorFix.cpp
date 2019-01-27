#include "CachedExt.h"
#include "ManorFix.h"
#include "Query.h"

UINT g_ManorLastChangedTime[16];
PWCHAR g_ManorLastChangedTimeS[16];

void CManorFix::Init()
{
	CachedExt::SetHook(0xE8, 0x448CCB, (DWORD)CManorFix::ExecuteSaveInfo, 0);
	CachedExt::SetHook(0xE8, 0x448A8B, (DWORD)CManorFix::BindSTime, 0);
	CachedExt::SetHook(0xE8, 0x448AB3, (DWORD)CManorFix::FetchLoadInfo, 0);
	
	for(UINT n=0;n<16;n++)
	{
		g_ManorLastChangedTime[n] = 0;
		g_ManorLastChangedTimeS[n] = 0;
	}

	CachedExt::WriteMemoryDWORD(0x5016A1, (DWORD)CManorFix::RequestSetLastResidenceTaxUpdate);
	CachedExt::WriteMemoryDWORD(0x501681, (DWORD)CManorFix::RequestLoadLastResidenceTaxUpdate);
}

bool CManorFix::RequestLoadLastResidenceTaxUpdate(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	UINT incomeUpdate = 0, taxChange = 0, manorReset = 0;


	CQuery query(L"SELECT [income_update_t], [tax_change_t], [manor_reset_t] FROM [castle_tax]");
	DBConn *pCon = new DBConn();

	pCon->Bind(&incomeUpdate);
	pCon->Bind(&taxChange);
	pCon->Bind(&manorReset);
	if(pCon->Execute(query.Get()))
	{
		pCon->Fetch();
	}

	delete pCon;

	g_Log.Add(CLog::Blue, "[%s] Sending last tax update data [%d][%d][%d]", __FUNCTION__, incomeUpdate, taxChange, manorReset);

	pQuerySocket->Send("cddd", 0xAC, incomeUpdate, taxChange, manorReset);

	return false;
}

bool CManorFix::RequestSetLastResidenceTaxUpdate(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	UINT incomeUpdate = 0, taxChange = 0, manorReset = 0;
	Disassemble(packet, "ddd", &incomeUpdate, &taxChange, &manorReset);

	CQuery query(L"EXEC lin_SetLastTaxUpdateEx ?, ?, ?");
	query.AddParam(incomeUpdate);
	query.AddParam(taxChange);
	query.AddParam(manorReset);

	DBConn *pCon = new DBConn();
	pCon->Execute(query.Get());
	delete pCon;
	g_Log.Add(CLog::Blue, "[%s] Saving last tax update [%d][%d][%d]", __FUNCTION__, incomeUpdate, taxChange, manorReset);
	return false;
}

bool CManorFix::FetchLoadInfo(DBConn *pCon)
{
	guard;
	bool ret = pCon->Fetch();

	if(ret)
	{
		UINT index = GetThreadIndex();
		if(index < 16)
		{
			UINT lastChanged = g_ManorLastChangedTime[index];
			g_ManorLastChangedTime[index] = 0;
			PWCHAR wLastChanged = g_ManorLastChangedTimeS[index];
			g_ManorLastChangedTimeS[index] = 0;
			if(wLastChanged)
			{
				//convert lastChanged uinix time to year-month-day hour::minute:second format
				if(lastChanged > 0)
				{
					tm tmTime;
					time_t raw = lastChanged;
					localtime_s(&tmTime, &raw);
					wstringstream dataStream;
					dataStream << tmTime.tm_year + 1900 << L"-" << tmTime.tm_mon + 1 << L"-" << tmTime.tm_mday << L" " << tmTime.tm_hour << L":" << tmTime.tm_min << L":" << tmTime.tm_sec;
					wcscpy(wLastChanged, dataStream.str().c_str());
				}else
				{
					wcscpy(wLastChanged, L"0000-00-00 00:00:00");
				}
				g_Log.Add(CLog::Blue, "[%s] converting time[%d] to [%S]", __FUNCTION__, lastChanged, wLastChanged);
			}

		}
	}

	unguard;
	return ret;
}

void CManorFix::BindSTime(DBConn *pCon, PWCHAR wBuff, int buffLen)
{
	guard;
	UINT index = GetThreadIndex();
	if(index < 16)
	{
		g_ManorLastChangedTimeS[index] = wBuff;
		pCon->Bind(&g_ManorLastChangedTime[index]);
	}
	unguard;
}



bool CManorFix::ExecuteSaveInfo(DBConn *pCon, const WCHAR *format, int manorId, int residenceId, int adenaSeedSell, int adenaCropBuy, int changeState, int year, int month, int day, int hour, int minute, int second)
{
	guard;
	//converting time to unix time format
	UINT changeTime = static_cast<UINT>(time(0));
	if(year > 0)
	{
		time_t rawtime = 0;
		time ( &rawtime );
		tm tmTime;
		localtime_s(&tmTime, &rawtime);
		tmTime.tm_year = year - 1900;
		tmTime.tm_mon = month - 1;
		tmTime.tm_mday = day;
		tmTime.tm_hour = hour;
		tmTime.tm_min = minute;
		tmTime.tm_sec = second;

		changeTime = static_cast<UINT>(mktime ( &tmTime ));
	}

	CQuery query(L"exec lin_SaveManorInfo ?, ?, ?, ?, ?, ?");
	query.AddParam(manorId);
	query.AddParam(residenceId);
	query.AddParam(adenaSeedSell);
	query.AddParam(adenaCropBuy);
	query.AddParam(changeState);
	query.AddParam(changeTime);

	g_Log.Add(CLog::Blue, "[%s] Saving Manor Info ManorId[%d] Residence[%d] SeedAdena[%d] CropAdena[%d] ChangeState[%d] ChangeTime[%d] (%d-%d-%d %d:%d:%d)", __FUNCTION__, manorId, residenceId, adenaSeedSell, adenaCropBuy, changeState, changeTime, year, month, day, hour, minute, second);

	bool ret = pCon->Execute(query.Get());
	unguard;
	return ret;
}
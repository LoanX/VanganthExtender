#pragma once
#include <windows.h>
#include "DBConn.h"
class CManorFix
{
public:
	static void Init();
	static void __thiscall BindSTime(DBConn *pCon, PWCHAR wBuff, int buffLen);
	static bool __thiscall FetchLoadInfo(DBConn *pCon);
	//exec lin_SaveManorInfo %d, %d, %d, %d, %d, %d-%d-%d %d:%d:%d	
	static bool ExecuteSaveInfo(DBConn *pCon, const WCHAR* format, int manorId, int residenceId, int adenaSeedSell, int adenaCropBuy, int changeState, int year, int month, int day, int hour, int minute, int second);
	static bool RequestSetLastResidenceTaxUpdate(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool RequestLoadLastResidenceTaxUpdate(CQuerySocket *pQuerySocket, const unsigned char* packet);
};
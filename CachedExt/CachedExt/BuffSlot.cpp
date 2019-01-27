#include "Utils.h"
#include "BuffSlot.h"
#include "DBConn.h"
#include "Query.h"

void CBuffSlot::Init()
{
	WriteMemoryDWORD(0x500E61, (DWORD)CBuffSlot::RequestSaveAbnormalStatus);
	WriteMemoryDWORD(0x500E81, (DWORD)CBuffSlot::RequestLoadAbnormalStatus);
}

bool __cdecl CBuffSlot::RequestSaveAbnormalStatus(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	UINT charId = 0, abnormalAmount = 0;
	packet = Disassemble(packet, "dd", &charId, &abnormalAmount);

	int abnormalData[ABNORMAL_SLOT_COUNT][4];
	memset(abnormalData, 0, sizeof(abnormalData));

	//Format: c ddb
	//b - dddd x nBuffAmount   / SkillID, Level, Seconds, SelfCast - sx lx dx cx
	if(abnormalAmount <= ABNORMAL_SLOT_COUNT)
	{
		for(UINT n=0; n<abnormalAmount; n++)
		{
			packet = Disassemble(packet, "dddd", &abnormalData[n][0], &abnormalData[n][1], &abnormalData[n][2], &abnormalData[n][3]);
		}
	}

	//prepare statement
	wstringstream stmp;
	stmp << L"EXEC lin_UpdateActiveSkill ?";
	for(UINT n=0;n<ABNORMAL_SLOT_COUNT;n++)
	{
		stmp << L", ?" << L", ?" << L", ?" << L", ?";
	}
	CQuery query(stmp.str());
	query.AddParam(charId);
	for(UINT n=0;n<ABNORMAL_SLOT_COUNT;n++)
	{
		query.AddParam(abnormalData[n][0]);
		query.AddParam(abnormalData[n][1]);
		query.AddParam(abnormalData[n][2]);
		query.AddParam(abnormalData[n][3]);
	}
	//Connect to sql and execute procedure
	DBConn *pDBConn = new DBConn;

	if(!pDBConn->Execute(query.Get()))
	{
		//prepare insert statement
		wstringstream stmp2;
		stmp2 << L"EXEC lin_CreateActiveSkill ?";
		for(UINT n=0;n<ABNORMAL_SLOT_COUNT;n++)
		{
			stmp2 << L", ?" << L", ?" << L", ?" << L", ?";
		}
		CQuery query2(stmp2.str());
		query2.AddParam(charId);
		for(UINT n=0;n<ABNORMAL_SLOT_COUNT;n++)
		{
			query2.AddParam(abnormalData[n][0]);
			query2.AddParam(abnormalData[n][1]);
			query2.AddParam(abnormalData[n][2]);
			query2.AddParam(abnormalData[n][3]);
		}
		pDBConn->Execute(query2.Get());
	}

	delete pDBConn;

	return false;
};

bool __cdecl CBuffSlot::RequestLoadAbnormalStatus(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	DWORD nUserDBID = 0;
	DWORD nSecondVar = 0;
	
	char Buff[8192];
	int nBuffSize = 0;
	memset(Buff,0, sizeof(Buff));

	packet = Disassemble(packet, "dd", &nUserDBID, &nSecondVar);

	int naBuffTable[ABNORMAL_SLOT_COUNT][4];
	int nBuffAmount = 0;

	memset(naBuffTable, 0, sizeof(naBuffTable));

	DBConn *pDBConn = new DBConn;

	for(unsigned int n=0; n<ABNORMAL_SLOT_COUNT; n++)
	{
		pDBConn->Bind(&naBuffTable[n][0]);
		pDBConn->Bind(&naBuffTable[n][1]);
		pDBConn->Bind(&naBuffTable[n][2]);
	}

	for(unsigned int n=0; n<ABNORMAL_SLOT_COUNT; n++)
		pDBConn->Bind(&naBuffTable[n][3]);

	if(pDBConn->Execute(L"EXEC lin_GetActiveSkills %d", nUserDBID))
	{
		if(pDBConn->Fetch())
		{
			for(DWORD n=0; n<ABNORMAL_SLOT_COUNT; n++)
			{
				if(naBuffTable[n][0]!= 0)
				{
					nBuffSize += Assemble(&Buff[nBuffSize], (sizeof(Buff) - nBuffSize), "dddd", naBuffTable[n][0], naBuffTable[n][1], naBuffTable[n][2], naBuffTable[n][3]);
					nBuffAmount++;
				}
			}
			pQuerySocket->Send("cddb", 0x6F, nSecondVar, nBuffAmount, nBuffSize, Buff);
		}else
			pQuerySocket->Send("cdd", 0x6F, nSecondVar, nBuffAmount);
	}else
		pQuerySocket->Send("cdd", 0x6F, nSecondVar, nBuffAmount);

	delete pDBConn;
	return false;
}
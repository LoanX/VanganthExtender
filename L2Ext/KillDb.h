#pragma once

struct KillData
{
	UINT victimDatabaseId;
	UINT killTime;
};

class CKillDb
{
	CSLock m_Lock;
	map<UINT, vector<KillData>> m_Data;
public:
	CKillDb();
	bool IsInReuse(UINT killerId, UINT victimId, UINT reuse);
};

extern CKillDb g_KillDb;
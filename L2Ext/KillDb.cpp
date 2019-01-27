#include "stdafx.h"
#include "KillDb.h"

CKillDb g_KillDb;

CKillDb::CKillDb()
{
}

bool CKillDb::IsInReuse(UINT killerId, UINT victimId, UINT reuse)
{
	guard;
	bool ret = false;
	unguard;

	m_Lock.Enter();
	map<UINT, vector<KillData>>::iterator it = m_Data.find(killerId);
	if(it!=m_Data.end())
	{
		vector<KillData>& vKillData = it->second;
		bool found = false;
		for(UINT n=0;n<vKillData.size();n++)
		{
			KillData& kd = vKillData[n];
			if(kd.victimDatabaseId == victimId)
			{
				found = true;
				if((kd.killTime + reuse) < time(0))
				{
					kd.killTime = time(0);
				}else
				{
					ret = true;
				}
				break;
			}
		}
		if(!found)
		{
			KillData kd;
			kd.victimDatabaseId = victimId;
			kd.killTime = time(0);
			vKillData.push_back(kd);
		}
	}else
	{
		vector<KillData> vKillData;
		KillData kd;
		kd.victimDatabaseId = victimId;
		kd.killTime = time(0);
		vKillData.push_back(kd);
		m_Data.insert(pair<UINT, vector<KillData>>(killerId, vKillData));
	}
	m_Lock.Leave();
	return ret;
}
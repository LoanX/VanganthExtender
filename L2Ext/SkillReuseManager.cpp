#include "StdAfx.h"
void CUserSkillReuseData::Clear(int nSubclass)
{
	if(nSubclass > -1 && nSubclass < 4)
	{
		Lock();
		mData[nSubclass].clear();
		Unlock();
	}
}

void CUserSkillReuseData::SetReuse(int SkillID, int Level, DWORD ReuseEnd, int nSubclass)
{
	if(nSubclass > -1 && nSubclass < 4)
	{
		CSkillKey key(SkillID, Level);
		Lock();
		mData[nSubclass][key] = (GetTickCount() + ReuseEnd);
		Unlock();
	}
}
DWORD CUserSkillReuseData::GetReuse(int SkillID, int Level, int nSubclass)
{
	if(nSubclass > -1 && nSubclass < 4)
	{
		DWORD dRet = NULL;
		CSkillKey key(SkillID, Level);
		Lock();
		map<CSkillKey, DWORD>::iterator Iter = mData[nSubclass].find(key);
		if(Iter!=mData[nSubclass].end())
		{
			dRet = Iter->second;
		}
		Unlock();
		return dRet;
	}
	return NULL;
}

void CUserSkillReuseData::OnEnterWorld(User *pUser)
{
	int nSubIndex = pUser->pSD->nSubClassIndex;
	if(nSubIndex >= 0 && nSubIndex < 4)
	{
		DWORD Tick = GetTickCount();
		Lock();
		for(map<CSkillKey, DWORD>::iterator Iter = mData[nSubIndex].begin();Iter!=mData[nSubIndex].end();Iter++)
		{
			if(Iter->second > Tick)
			{
				if(!pUser->GetSkillUsedTime(Iter->first.skill_id, Iter->first.level))
				{
					pUser->SetSkillUsableTime(Iter->first.skill_id, Iter->first.level, NULL, (Iter->second - Tick));
				}
			}
		}
		Unlock();
	}
}
CSkillReuseManager::~CSkillReuseManager()
{
	ReleaseData();
}

void CSkillReuseManager::Initialize()
{
	g_Log.Add(CLog::Blue, "[%s] Done.", __FUNCTION__);
}
DWORD CSkillReuseManager::GetReuse(User *pUser, int SkillID, int Level)
{
	DWORD dRet = NULL;
	if(pUser->ValidUser())
	{
		CUserSkillReuseData *pData = NULL;
		map<int, CUserSkillReuseData*>::iterator Iter;
		Lock();
		Iter = mData.find(pUser->nDBID);
		if(Iter != mData.end())
		{
			pData = Iter->second;
		}else
		{
			pData = new CUserSkillReuseData();
			mData[pUser->nDBID] = pData;
		}
		Unlock();
		if(pData)
		{
			try
			{
				dRet = pData->GetReuse(SkillID, Level, pUser->pSD->nSubClassIndex);
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] SkillID[%d] - Exception!", __FUNCTION__, pUser->pSD->wszName, SkillID);
			}
		}
	}
	return dRet;
};
void CSkillReuseManager::OnEnterWorld(User *pUser)
{
	if(pUser->ValidUser())
	{
		CUserSkillReuseData *pData = NULL;
		map<int, CUserSkillReuseData*>::iterator Iter;
		Lock();
		Iter = mData.find(pUser->nDBID);
		if(Iter != mData.end())
		{
			pData = Iter->second;
		}else
		{
			pData = new CUserSkillReuseData();
			mData[pUser->nDBID] = pData;
		}
		Unlock();
		if(pData)
		{
			pData->OnEnterWorld(pUser);
		}
	}
}

void CSkillReuseManager::SetReuse(User *pUser, int SkillID, int Level, DWORD EndReuse)
{
	if(pUser->ValidUser())
	{
		CUserSkillReuseData *pData = NULL;
		map<int, CUserSkillReuseData*>::iterator Iter;
		Lock();
		Iter = mData.find(pUser->nDBID);
		if(Iter != mData.end())
		{
			pData = Iter->second;
		}else
		{
			pData = new CUserSkillReuseData();
			mData[pUser->nDBID] = pData;
		}
		Unlock();
		if(pData)
		{
			pData->SetReuse(SkillID, Level, EndReuse, pUser->pSD->nSubClassIndex);
		}
	}
};
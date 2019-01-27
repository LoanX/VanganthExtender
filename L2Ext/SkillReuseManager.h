#pragma once


class CUserSkillReuseData : private CThreadSafe
{
	map<CSkillKey, DWORD> mData[4]; //4 subclsses
public:
	void Clear(int nSubclass);
	void SetReuse(int SkillID, int Level, DWORD ReuseEnd, int nSubclass);
	DWORD GetReuse(int SkillID, int Level, int nSubclass);
	void OnEnterWorld(User *pUser);
};

class CSkillReuseManager : private CThreadSafe
{
	map<int, CUserSkillReuseData*> mData;
	void ReleaseData()
	{
		Lock();
		for(map<int, CUserSkillReuseData*>::iterator Iter = mData.begin(); Iter!= mData.end();Iter++)
		{
			delete Iter->second;
		}
		Unlock();
	}
public:
	~CSkillReuseManager();
	void Initialize();
	DWORD GetReuse(User *pUser, int SkillID, int Level);
	void OnEnterWorld(User *pUser);
	void SetReuse(User *pUser, int SkillID, int Level, DWORD EndReuse);
};

extern CSkillReuseManager g_SkillReuseManager;
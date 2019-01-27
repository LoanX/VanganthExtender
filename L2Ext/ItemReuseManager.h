#pragma once


class CUserItemReuseData : private CThreadSafe
{
	map<int, DWORD> mData[4]; //4 subclsses
	friend class CItemReuseManager;
public:
	void Clear(int nSubclass);
	void SetReuse(int ItemClassID, DWORD ReuseEnd, int nSubclass);
	DWORD GetReuse(int ItemClassID, int nSubclass);
};

class CItemReuseManager  : private CThreadSafe
{
	map<int, CUserItemReuseData*> mData;
	void ReleaseData()
	{
		Lock();
		for(map<int, CUserItemReuseData*>::iterator Iter = mData.begin(); Iter!= mData.end();Iter++)
		{
			delete Iter->second;
		}
		Unlock();
	}
public:
	~CItemReuseManager();
	void Initialize();
	void OnEnterWorld(User *pUser);
	DWORD GetReuse(User *pUser, int ItemClassID);
	void SetReuse(User *pUser, int ItemClassID, DWORD EndReuse);
};
extern CItemReuseManager g_ItemReuseManager;
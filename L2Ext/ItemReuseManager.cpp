#include "StdAfx.h"
#include "ObjectDB.h"

void CUserItemReuseData::Clear(int nSubclass)
{
	if(nSubclass > -1 && nSubclass < 4)
	{
		Lock();
		mData[nSubclass].clear();
		Unlock();
	}
}

void CUserItemReuseData::SetReuse(int ItemClassID, DWORD ReuseEnd, int nSubclass)
{
	if(nSubclass > -1 && nSubclass < 4)
	{
		Lock();
		mData[nSubclass][ItemClassID] = (GetTickCount() + ReuseEnd);
		Unlock();
	}
}
DWORD CUserItemReuseData::GetReuse(int ItemClassID, int nSubclass)
{
	if(nSubclass > -1 && nSubclass < 4)
	{
		DWORD dRet = NULL;
		Lock();
		map<int, DWORD>::iterator Iter = mData[nSubclass].find(ItemClassID);
		if(Iter!=mData[nSubclass].end())
		{
			dRet = Iter->second;
		}
		Unlock();
		return dRet;
	}
	return NULL;
}
CItemReuseManager::~CItemReuseManager()
{
	ReleaseData();
}

void CItemReuseManager::Initialize()
{
	g_Log.Add(CLog::Blue, "[%s] Done.", __FUNCTION__);
}

DWORD CItemReuseManager::GetReuse(User *pUser, int ItemClassID)
{
	DWORD dRet = NULL;
	if(pUser->ValidUser())
	{
		CUserItemReuseData *pData = NULL;
		map<int, CUserItemReuseData*>::iterator Iter;
		Lock();
		Iter = mData.find(pUser->nDBID);
		if(Iter != mData.end())
		{
			pData = Iter->second;
		}else
		{
			pData = new CUserItemReuseData();
			mData[pUser->nDBID] = pData;
		}
		Unlock();
		if(pData)
		{
			try
			{
				dRet = pData->GetReuse(ItemClassID, pUser->pSD->nSubClassIndex);
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] ItemClassID[%d] - Exception!", __FUNCTION__, pUser->pSD->wszName, ItemClassID);
			}
		}
	}
	return dRet;
};
void CItemReuseManager::SetReuse(User *pUser, int ItemClassID, DWORD EndReuse)
{
	if(pUser->ValidUser())
	{
		CUserItemReuseData *pData = NULL;
		map<int, CUserItemReuseData*>::iterator Iter;
		Lock();
		Iter = mData.find(pUser->nDBID);
		if(Iter != mData.end())
		{
			pData = Iter->second;
		}else
		{
			pData = new CUserItemReuseData();
			mData[pUser->nDBID] = pData;
		}
		Unlock();
		if(pData)
		{
			pData->SetReuse(ItemClassID, EndReuse, pUser->pSD->nSubClassIndex);
		}
	}
};

void CItemReuseManager::OnEnterWorld(User *pUser)
{
	if(pUser->ValidUser())
	{
		CUserItemReuseData *pData = NULL;
		map<int, CUserItemReuseData*>::iterator Iter;
		Lock();
		Iter = mData.find(pUser->nDBID);
		if(Iter != mData.end())
		{
			pData = Iter->second;
		}else
		{
			pData = new CUserItemReuseData();
			mData[pUser->nDBID] = pData;
		}
		Unlock();
		if(pData)
		{
			pData->Lock();
			try
			{
				map<int, DWORD>& mData = pData->mData[pUser->pSD->nSubClassIndex];
				if(mData.size() > 0)
				{
					for(map<int, DWORD>::iterator Iter = mData.begin(); Iter!=mData.end();Iter++)
					{
						if(Iter->second > (GetTickCount() + 1000))
						{
							CItem *pItem = g_ObjectDB.GetTemplateObject(Iter->first)->CastItem();
							CItemDataEx *pData = g_ItemDBEx.GetItemDataEx(Iter->first);
							if(pItem && pData && pData->GetReuseGroup() > 0)
							{
								UINT Reuse = Iter->second - GetTickCount();
								pUser->GetSocket()->Send("chdddd", 0xFE, 0x49, Iter->first, pData->GetReuseGroup(), (Reuse/1000), (pItem->pII->reuseDelay/1000));
							}
						}
					}
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
			}
			pData->Unlock();
		}
	}
}
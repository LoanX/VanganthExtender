#include "StdAfx.h"
#include "SocketDB.h"
#include "OfflineSocketDB.h"

COfflineSocketDB g_OfflineSocketDB;

bool COfflineSocketDB::Add(CUserSocket *pSocket, CIOObject *pIOObject)
{
	Lock();
	try
	{
		OfflineSocketMap::iterator Iter = mData.find(pSocket);
		if(Iter!=mData.end())
		{
			g_Log.Add(CLog::Error, "[%s] Socket[%s] already is in DataBase.", __FUNCTION__);
			Unlock();
			return false;
		}

		COfflineSocketData data;
		data.SetIp(pSocket->addr.S_un.S_addr);
		data.SetHwid(pSocket->pED->longHWID);
		data.pIOObject = pIOObject;

		mData.insert(pair<CUserSocket*, COfflineSocketData>(pSocket, data));
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	Unlock();

	return true;
}

int COfflineSocketDB::GetSize()
{
	int nSize = 0;
	Lock();
	nSize = (int)mData.size();
	Unlock();
	return nSize;
}

int COfflineSocketDB::GetHwidCount(CUserSocket *pSocket)
{
	guard;
	if(pSocket->ValidUserSocket())
	{
		UINT ip = pSocket->addr.S_un.S_addr;
		HWID& hwid = pSocket->pED->longHWID;
		int nCount = 0;
		Lock();
		try
		{
			for(OfflineSocketMap::iterator Iter = mData.begin(); Iter!=mData.end(); Iter++)
			{
				if(hwid.IsEqual(Iter->second.hwid))
				{
					nCount++;
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
		}
		Unlock();
		unguard;
		return nCount;
	}
	unguard;
	return NULL;
}

bool COfflineSocketDB::StartCloseTimer(CUserSocket *pSocket)
{
	bool ret = false;
	Lock();
	try
	{
		OfflineSocketMap::iterator Iter = mData.find(pSocket);
		if(Iter!=mData.end())
		{
			CIOObject *pIOObject = Iter->second.pIOObject;
			if(pIOObject->IsValidIOObject())
			{
				pIOObject->AddTimer(100, 3);
			}else
				g_Log.Add(CLog::Error, "[%s] Invalid CIOObject!", __FUNCTION__);
			mData.erase(Iter);
			ret = true;
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	Unlock();
	if(ret)
		g_SocketDB.SetStatus(pSocket, CSocketData::IN_WORLD);

	return ret;
}
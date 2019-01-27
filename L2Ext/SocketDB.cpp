#include "StdAfx.h"
#include "SocketDB.h"
#include "AccountDB.h"

CUserSocketDB g_SocketDB;

size_t CUserSocketDB::GetSocketCount()
{
	size_t size = 0;
	Lock();
	size = mData.size();
	Unlock();
	return size;
}

int CUserSocketDB::GetIpCount(CIPAddress ip)
{
	int nCount = 0;
	Lock();
	for(UserSocketMap::iterator Iter=mData.begin();Iter!=mData.end();Iter++)
	{
		if(Iter->second.ip == ip && Iter->second.status != CSocketData::OFFLINE_SHOPPING)
		{
			nCount++;
		}
	}
	Unlock();
	return nCount;
};

void CUserSocketDB::AddSocket(CUserSocket *pSocket, CSocketData Data)
{
	Lock();
	mData.insert(pair<CUserSocket*, CSocketData>(pSocket, Data));
	Unlock();
};

void CUserSocketDB::DeleteSocket(CUserSocket *pSocket)
{
	Lock();
	UserSocketMap::iterator Iter = mData.find(pSocket);
	if(Iter!=mData.end())
	{
		mData.erase(Iter);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Cannot find socket[%p]!", __FUNCTION__, pSocket);
	}
	Unlock();
};

void CUserSocketDB::SetStatus(CUserSocket *pSocket, CSocketData::STATUS Status)
{
	Lock();
	UserSocketMap::iterator Iter = mData.find(pSocket);
	if(Iter!=mData.end())
	{
		Iter->second.status = Status;
	}
	Unlock();
};

CSocketData::STATUS CUserSocketDB::GetStatus(CUserSocket *pSocket)
{
	CSocketData::STATUS Status = CSocketData::IDLE;
	Lock();
	UserSocketMap::iterator Iter = mData.find(pSocket);
	if(Iter!=mData.end())
	{
		Status = Iter->second.status;
	}
	Unlock();
	return Status;
}

int CUserSocketDB::GetIPnHWIDCount(UINT ip, LPBYTE lpHwid)
{
	guard;
	int nCount = 0;
	Lock();
	for(UserSocketMap::iterator Iter=mData.begin();Iter!=mData.end();Iter++)
	{
		try
		{
			CUserSocket *pSocket = Iter->first;
			if(pSocket->addr.S_un.S_addr == ip && !memcmp(lpHwid, pSocket->pED->longHWID.hash, 32))
			{
				if(Iter->second.status != CSocketData::OFFLINE_SHOPPING)
				{
					if(pSocket->state != 2)
					{
						if(!g_AccountDB.IsAdditionalBox(pSocket->accountId))
						{
							nCount++;
						}
					}
				}
			}
		}catch(...)
		{

		}
	}
	Unlock();
	unguard;
	return nCount;
}

int CUserSocketDB::GetHWIDCount(LPBYTE lpHwid)
{
	guard;
	int nCount = 0;
	Lock();
	for(UserSocketMap::iterator Iter=mData.begin();Iter!=mData.end();Iter++)
	{
		try
		{
			CUserSocket *pSocket = Iter->first;
			if(pSocket->socketStatus != 2)
			{
				if(!memcmp(lpHwid, pSocket->pED->longHWID.hash, 32))
				{
					if(Iter->second.status != CSocketData::OFFLINE_SHOPPING)
					{
						if(pSocket->state != 2)
						{
							if(!g_AccountDB.IsAdditionalBox(pSocket->accountId))
							{
								nCount++;
							}
						}
					}
				}
			}
		}catch(...)
		{

		}
	}
	Unlock();
	unguard;
	return nCount;
}

void CUserSocketDB::PrintAccountData(CIPAddress Ip, CMacAddress Mac)
{
	Lock();
	for(UserSocketMap::iterator Iter=mData.begin();Iter!=mData.end();Iter++)
	{
		if(Iter->second.ip == Ip && Iter->second.mac == Mac)
		{
			if(Iter->second.status != CSocketData::OFFLINE_SHOPPING && Iter->second.status != CSocketData::LEAVING_WORLD)
			{
				g_Log.Add(CLog::Error, "IP: %s MAC: %s SOCKET: %d AccountId: %d", Ip.ToString().c_str(), Mac.ToString().c_str(), Iter->first->s, Iter->first->accountId);
			}
		}
	}
	Unlock();
}

void CUserSocketDB::CloseSockets(CIPAddress Ip)
{
	Lock();
	for(UserSocketMap::iterator Iter=mData.begin();Iter!=mData.end();Iter++)
	{
		if(Iter->second.ip == Ip)
		{
			CUserSocket *pSocket = Iter->first;
			if(pSocket)
			{
				pSocket->Close();
				Iter->second.status = CSocketData::LEAVING_WORLD;
			}
		}
	}
	Unlock();
};

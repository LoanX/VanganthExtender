#include "StdAfx.h"

CPartyWaitingList::CPartyWaitingList()
{
}

CPartyWaitingList::~CPartyWaitingList()
{
}

bool CPartyWaitingList::IsInList(User *pUser)
{
	bool found = false;
	Lock();
	WaitingList::iterator Iter = lData.begin();
	for(;Iter!=lData.end();Iter++)
	{
		if((*Iter).objectId == pUser->nObjectID)
		{
			found = true;
			break;
		}
	}
	Unlock();
	return found;
}

void CPartyWaitingList::AddMember(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(pUser->pSD->nPartyID == 0)	//if user is in party he wont be added to list
		{
			if(!IsInList(pUser))
			{
				WaitingListMember wlm;
				wlm.objectId = pUser->nObjectID;
				wlm.level = pUser->pSD->nLevel;
				wlm.classId = pUser->pSD->nClass;
				wlm.pUser = pUser;
				wlm.inRoom = 0;
				wcscpy_s(wlm.wName, 25, pUser->pSD->wszName);
				Lock();
				lData.push_back(wlm);
				Unlock();
			}
		}
	}
}

void CPartyWaitingList::DelMember(User *pUser)
{
	Lock();
	WaitingList::iterator Iter = lData.begin();
	for(;Iter!=lData.end();Iter++)
	{
		WaitingListMember& wlm = (*Iter);
		if(wlm.objectId == pUser->nObjectID)
		{
			lData.erase(Iter);
			break;
		}
	}
	Unlock();
}

void CPartyWaitingList::ClearList(User *pUser)
{
	if(pUser->ValidUser())
	{
		pUser->GetSocket()->Send("chdd", 0xFE, 0x35, 0, 0);
	}
}

void CPartyWaitingList::RequestList(User *pUser, int page, int nMinLevel, int nMaxLevel, int nInPartyRoom)
{
	if(pUser->ValidUser())
	{
		CHAR buff[8190];
		memset(buff, 0, sizeof(buff));
		int len = 0;
		int count = 0;
		int max_count = 0;
		page--;
		if(page < 0)
			page = 0;
		int required_count = page*64;
		WaitingList lTmp;
		Lock();
		for(WaitingList::iterator Iter = lData.begin();Iter!=lData.end();Iter++)
		{
			if(Iter->level >= nMinLevel && Iter->level <= nMaxLevel)
			{
	//			if(nInPartyRoom == 1 && Iter->inRoom == 1) //it does noting atm inRoom will never be 1
		//			continue;
				if(Iter->objectId == pUser->nObjectID)
					continue;

				if(Iter->pUser->ValidUser())
				{					
					if(max_count >= required_count)
					{
						lTmp.push_back((*Iter));
					}
					max_count++;
				}
			}
		}
		Unlock();
		
		if( max_count > required_count )
		{
			for(WaitingList::iterator Iter = lTmp.begin();Iter!=lTmp.end();Iter++)
			{
				if(Iter->pUser->ValidUser())
				{
					len += Assemble(&buff[len], (sizeof(buff)-len), "Sdd", Iter->wName, Iter->classId, Iter->level);
					count++;
				}
			}
			pUser->GetSocket()->Send("chddb", 0xFE, 0x35, max_count, count, len, buff);
		}else
		{
			pUser->GetSocket()->Send("chdd", 0xFE, 0x35, max_count, count);
		}
	}
};

void CPartyWaitingList::AskJoinRoom(User *pUser, PWCHAR wTarget)
{
	if(pUser->ValidUser())
	{
		User *pTarget = NULL;
		Lock();
		for(WaitingList::iterator Iter = lData.begin();Iter!=lData.end();Iter++)
		{
			if(!wcscmp(Iter->wName, wTarget))
			{
				pTarget = Iter->pUser;
				break;
			}
		}
		Unlock();

		if(pTarget->ValidUser())
		{
			if(pTarget->pSD->pExData->requestJoinParyRoom.GetRequestor() == NULL)
			{
				CPartyRoom *pRoom = CPartyRoom::FindByMember(pUser);
				try
				{
					if(pRoom)
					{
						int roomId = pRoom->pi.roomId;

						pTarget->pSD->pExData->requestJoinParyRoom.Set(pUser->nObjectID, roomId);
						pTarget->GetSocket()->Send("chS", 0xFE, 0x34, pUser->pSD->wszName);
					}else
						g_Log.Add(CLog::Error, "[%s] Cannot find room!", __FUNCTION__);
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
				}
			}else
			{
				pUser->SendSystemMessage(L"Selected player has been already invited by someone.");
			}
		}else
		{
			pUser->SendSystemMessage(L"Selected player has left the waiting list for party matching and cannot be invited.");
		}
	}
}

void CPartyWaitingList::JoinRoom(User *pUser, int roomId)
{
	if(pUser->ValidUser())
	{
		CHAR buff[8190];
		int len = Assemble(buff, 8190, "dd", roomId, 1);
		_SockFunc(0x876C50L)(pUser->GetSocket(), (const unsigned char*)buff);
	}
}

void CPartyWaitingList::TimerExpired()
{
	guard;
	Lock();
	try
	{
		WaitingList::iterator Iter = lData.begin();
		for(;Iter!=lData.end();)
		{
			if(!Iter->pUser->ValidUser())
			{
				lData.erase(Iter++);
			}else
			{
				Iter++;
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	Unlock();

	unguard;
}
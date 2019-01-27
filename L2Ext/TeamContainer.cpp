#include "StdAfx.h"

void CTeamContainer::Initialize()
{
	//Do Nothing just initialize object
}
int CTeamContainer::GenerateNewTeam()
{
	Lock();
	for(int n=1;n < (mData.size() + 10); n++) //Basicly +2 would be enough :P
	{
		if(mData.find(n) == mData.end())
		{
			UserList lUsers;
			mData.insert(pair<int, UserList>(n, lUsers));
			Unlock();
			return n;
		}
	}
	Unlock();
	return -1;
}
void CTeamContainer::ViewTeamList(CSocket *pSocket)
{
	if(pSocket->ValidUserSocket())
	{
		CUserSocket *pUS = pSocket->CastUserSocket();
		Lock();
		wstring wList;
		pUS->SendSystemMessage(L"Team ID List:");
		for(map<int, UserList>::iterator Iter=mData.begin();Iter!=mData.end();Iter++)
		{
			try
			{
				WCHAR wInt[8];
				int nTeam = Iter->first;
				_itow(nTeam, wInt, 10);
				wstring wMsg = L"ID: ";
				wMsg+= wInt;
				pUS->SendSystemMessage((PWCHAR)wMsg.c_str());
			}
			catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
			}
		}
		Unlock();
	}
}
void CTeamContainer::ViewUserList(CSocket *pSocket, int nTeam)
{
	if(CUserSocket* pUS = pSocket->CastUserSocket())
	{
		Lock();
		map<int, UserList>::iterator Iter = mData.find(nTeam);
		if(Iter!=mData.end())
		{
			wstring wMessage = L"Member List: \n";
			for(UserList::iterator Iter2 = Iter->second.begin(); Iter2!= Iter->second.end(); Iter2++)
			{
				User *pUser = *Iter2;
				if(pUser->ValidUser())
				{
					wMessage += pUser->pSD->wszName;
					wMessage += L" \n";
				}
			}
			wMessage += L"=========END=========\n";
			pUS->SendSystemMessage( (PWCHAR)wMessage.c_str());
		}else
			pUS->SendSystemMessage(L"Selected team cannot be found!");
		Unlock();
	}
}

void CTeamContainer::SendSystemMessage(int nTeam, int nMsgID, int nParam)
{
	CSystemMessage pck(nMsgID);
	pck.AddNumber(nParam);
	Broadcast(nTeam, pck.GetSize(), pck.GetBuff());
}

void CTeamContainer::SendSystemMessage(int nTeam, int nMsgID)
{
	CSystemMessage pck(nMsgID);
	Broadcast(nTeam, pck.GetSize(), pck.GetBuff());
}
void CTeamContainer::Broadcast(int nTeam, size_t nSize, PCHAR packet)
{
	Lock();
	map<int, UserList>::iterator Iter = mData.find(nTeam);
	if(Iter!=mData.end())
	{
		for(UserList::iterator Iter2 = Iter->second.begin(); Iter2!= Iter->second.end(); Iter2++)
		{
			User *pUser = *Iter2;
			if(pUser->ValidUser())
			{
				CUserSocket *pSocket = pUser->GetSocket();
				if(pSocket->ValidUserSocket())
				{
					pSocket->Send("b", nSize, packet);
				}
			}
		}
	}
	Unlock();
}
void CTeamContainer::DelTeam(CSocket *pSocket, int nTeam)
{
	if(CUserSocket *pUS = pSocket->CastUserSocket())
	{
		Lock();
		map<int, UserList>::iterator Iter = mData.find(nTeam);
		if(Iter!=mData.end())
		{
			mData.erase(Iter);
			pUS->SendSystemMessage(L"Team has been deleted.");
		}else
			pUS->SendSystemMessage(L"Team cannot be found.");
		Unlock();
	}
}
void CTeamContainer::ClearTeam(CSocket *pSocket, int nTeam)
{
	if(CUserSocket *pUS = pSocket->CastUserSocket())
	{
		Lock();
		map<int, UserList>::iterator Iter = mData.find(nTeam);
		if(Iter!=mData.end())
		{
			Iter->second.clear();
			pUS->SendSystemMessage(L"Team has been cleared.");
		}else
			pUS->SendSystemMessage(L"Team cannot be found.");
		Unlock();
	}
}
void CTeamContainer::DelUser(CSocket *pSocket, User *pTarget)
{
	if(CUserSocket *pUS = pSocket->CastUserSocket())
	{
		Lock();
		for(map<int, UserList>::iterator Iter = mData.begin();Iter!=mData.end();Iter++)
		{
			for(UserList::iterator Iter2 = Iter->second.begin(); Iter2!= Iter->second.end(); Iter2++)
			{
				User *pUser = *Iter2;
				if(pUser == pTarget)
				{
					Iter->second.erase(Iter2);
					pUS->SendSystemMessage(L"User has been deleted from Team.");
					return;
				}
			}
		}
		Unlock();
	}
}
void CTeamContainer::DelUser(CSocket *pSocket, int nTeam, User *pTarget)
{
	if(CUserSocket *pUS = pSocket->CastUserSocket())
	{
		Lock();
		map<int, UserList>::iterator Iter = mData.find(nTeam);
		if(Iter!=mData.end())
		{
			for(UserList::iterator Iter2 = Iter->second.begin(); Iter2!= Iter->second.end(); Iter2++)
			{
				User *pUser = *Iter2;
				if(pUser == pTarget)
				{
					Iter->second.erase(Iter2);
					pUS->SendSystemMessage(L"User has been deleted from Team.");
					break;
				}
			}
		}
		Unlock();
	}
}
void CTeamContainer::AddUser(CSocket *pSocket, int nTeam, User *pTarget)
{
	if(CUserSocket *pUS = pSocket->CastUserSocket())
	{
		Lock();
		if(pTarget->ValidUser())
		{
			map<int, UserList>::iterator Iter = mData.find(nTeam);
			if(Iter!=mData.end())
			{
				Iter->second.push_back(pTarget);
				wstring wMsg;
				wMsg += pTarget->pSD->wszName;
				wMsg += L" has been added to selected team.";
				pUS->SendSystemMessage((PWCHAR)wMsg.c_str());
			}else
			{
				pUS->SendSystemMessage(L"Selected team_id doesn't exist!");
			}
		}else
		{
			pUS->SendSystemMessage(SystemMessage::INVALID_TARGET_);
		}
		Unlock();
	}
}
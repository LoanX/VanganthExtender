#include "StdAfx.h"
User *CParty::GetMasterCandidate()
{
	typedef User * (__fastcall * __GetMasterCandidate)(CParty*);
	__GetMasterCandidate _GetMasterCandidate = (__GetMasterCandidate) 0x0071CA30;
	return _GetMasterCandidate(this);
}
int CParty::GetMembersAmount()
{
	if(this)
	{
		INT64 dAmount = (INT64)this;
		dAmount += 0x74;
		dAmount = *(DWORD*)dAmount;
		return dAmount;
	}
	return NULL;
}

void CParty::Dismiss()
{
	typedef void (__fastcall * __Dismiss)(CParty*);
	__Dismiss _Dismiss = (__Dismiss) 0x0071E200;
	_Dismiss(this);
}

void CParty::Withdraw(User *pUser)
{
	typedef void (__fastcall * __Withdraw)(CParty* , User*);
	__Withdraw _Withdraw = (__Withdraw) 0x0071FFF0;
	_Withdraw(this, pUser);
}

void CParty::HandOverMaster(User *pNewMaster)
{
	typedef void (__fastcall * __HandOverMaster)(CParty*, User*);
	__HandOverMaster _HandOverMaster = (__HandOverMaster) 0x007206F0;
	_HandOverMaster(this, pNewMaster);
}
User * CParty::GetMaster()
{
	typedef User *(__fastcall * __GetMaster)(CParty*);
	__GetMaster _GetMaster = (__GetMaster) 0x00717620;
	return _GetMaster(this);
}

void CParty::LeaveParty(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(this)
		{
			if(this->GetMembersAmount() > 2)
			{
				User *pMaster = this->GetMaster();
				if(pMaster == pUser)
				{
					User *pNewMaster = this->GetMasterCandidate();
					this->HandOverMaster(pNewMaster);
					this->Withdraw(pUser);
				}else
					this->Withdraw(pUser);
			}else
				this->Dismiss();
		}
	}
}

void CParty::GetAllMembersInRange(UserVector& vUsers, FVector& pos, double range)
{
	guard;
	if(this)
	{
		double x1 = pos.x;
		double y1 = pos.y;
		double z1 = pos.z;
		lpLock->Lock(__FILEW__, __LINE__);
		for(UINT n=0;n<memberCount;n++)
		{
			if(User *pUser = User::GetUserBySID(&memberId[n]))
			{
				if(pUser->pSD->nActiveUser)
				{
					double x2 = pUser->pSD->Pos.x;
					double y2 = pUser->pSD->Pos.y;
					double z2 = pUser->pSD->Pos.z;

					double dist = sqrt(pow((x2-x1), 2) + pow((y2-y1), 2) + pow((z2-z1), 2));
					if(dist < range)
					{
						vUsers.push_back(pUser);
					}
				}
			}
		}
		lpLock->Unlock();
	}
	unguard;
}

void CParty::GetAllMembersEx(UserVector& vUsers, bool lock)
{
	guard;
	if(this)
	{
		if(lock)
		{
			lpLock->Lock(__FILEW__, __LINE__);
		}

		for(UINT n=0;n<memberCount;n++)
		{
			if(User *pUser = User::GetUserBySID(&memberId[n]))
			{
				vUsers.push_back(pUser);
			}			
		}

		if(lock)
		{
			lpLock->Unlock();
		}
	}
	unguard;
}

bool CParty::ValidateMembersForDuel()
{
	if(this)
	{
		UserVector vUsers;
		GetAllMembersEx(vUsers);
		UserVector::iterator Iter;
		for(UINT n=0;n<vUsers.size();n++)
		{
			if(User *pUser = vUsers[n]->SafeCastUser())
			{
				if(!pUser->pSD->nAlive)
					return false;
				if(pUser->pSD->nKarma)
					return false;
				if(pUser->olympiadUser.GetState() != 0)
					return false;
				if(pUser->pSD->nInPeaceZone)
					return false;
				if(pUser->pSD->nInBattleField)
					return false;
				if(pUser->IsNowTrade())
					return false;
				if(pUser->InCombat() && (pUser->CombatTime() + 30) > (DWORD)time(NULL))
					return false;
			}
		}
		return true;
	}
	return false;
}

void CParty::GetMinMaxLevel(INT32& minLevel, INT32& maxLevel)
{
	minLevel = 80;
	maxLevel = 0;

	UserVector vUsers;
	GetAllMembersEx(vUsers);
	UserVector::iterator Iter;
	for(Iter=vUsers.begin(); Iter!=vUsers.end(); Iter++)
	{
		if(User *pUser = (*Iter)->GetUser())
		{
			if(pUser->pSD->nLevel < minLevel)
				minLevel = pUser->pSD->nLevel;
			if(pUser->pSD->nLevel > maxLevel)
				maxLevel = pUser->pSD->nLevel;
		}
	}
}

void CParty::Broadcast(int len, PCHAR buff)
{
	UserVector members;
	GetAllMembersEx(members);
	for(UINT n=0;n<members.size();n++)
	{
		if(User *pMember = members[n]->SafeCastUser())
		{
			pMember->pSocket->Send("b", len, buff);
		}
	}
}
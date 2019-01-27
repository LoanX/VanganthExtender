#include "StdAfx.h"
#include "DB.h"
#include "ClanSys.h"

MembersLimit g_ClanMembersLimit;

CPledge *CPledge::Constructor(INT32 nPledgeID, INT32 nMasterID, PWCHAR wszPledgeName, PWCHAR wszMasterName)
{
	typedef CPledge*(__thiscall *_CPledgeConstructor)(CPledge*, INT32, INT32, PWCHAR, PWCHAR);
	_CPledgeConstructor Constructor = (_CPledgeConstructor) 0x0076B390;
	return Constructor(this, nPledgeID, nMasterID, wszPledgeName, wszMasterName);
}

void* CPledge::Destructor(DWORD dData)
{
	typedef void* (__fastcall *__up)(CPledge *pPledge, DWORD dwData);
	__up _up = (__up) 0x0076B930;
	if(this)
	{
		try
		{
			delete pSD->pPledgeEx;
			pSD->pPledgeEx = NULL;
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception detected.", __FUNCTION__);
		}
	}
	return _up(this, dData);
}

void CPledge::Join(User *pUser)
{
	if(this==NULL) return;
	typedef void (__fastcall *__up)(CPledge*, User*);
	__up _up = (__up) 0x00769B50;
	_up(this, pUser);
}

void CPledge::Withdraw(User *pUser)
{
	if(this==NULL) return;
	typedef void (__fastcall *__up)(CPledge*, User*);
	__up _up = (__up) 0x0076A4E0;
	_up(this, pUser);
}

void CPledge::Oust(int nUserDBID, PWCHAR wszName)
{
	if(this==NULL) return;
	typedef void (__fastcall *__up)(CPledge*, int nUserDBID, PWCHAR);
	__up _up = (__up) 0x00769F20;
	_up(this, nUserDBID, wszName);	
}
CPledgeSharedData * CPledge::GetSharedData()
{
	if(this)
	{
		try
		{
			int nDBID = pSD->nDBID;
			return pSD;
		}
		catch(...)
		{
			return NULL;
		}
	}
	return NULL;
}
PWCHAR CPledge::GetName()
{
	typedef PWCHAR (__fastcall *__gn)(CPledge *pPledge);
	__gn _gn = (__gn) 0x00761A70;
	return _gn(this);
}

void CPledge::UpdatePledgeInfo()
{
	typedef void (__fastcall *__up)(CPledge *pPledge);
	__up _up = (__up) 0x00767520;
	if(this!=NULL)
		_up(this);
	else return;
}
INT64 *CPledge::GetAlliance()
{
	typedef INT64 *(__fastcall *__GA)(CPledge*);
	__GA _GA = (__GA) 0x0049E430;
	return _GA(this);
}

void CPledge::Save(int nField)
{
	typedef void(__fastcall *__GA)(CPledge*, int nField);
	__GA _GA = (__GA) 0x007643F0;
	_GA(this, nField);
}

void CPledge::SetMaster(User *pUser)
{
	typedef void (*f)(CPledge*, User*);
	f(0x7626D0L)(this, pUser);
}

void CPledge::IncreaseLevel678(int nLevel)
{
	if(this && pSD)
	{
		int nOldLevel = pSD->nLevel;
		if(nOldLevel > 4 && nOldLevel < 8)
		{
			nOldLevel++;
			pSD->nLevel = nLevel;
			Save(5);
		}
	}
}
int CPledge::GetAllianceID()
{
	try
	{
		INT64 dAllianceID = (INT64)GetAlliance();
		if(dAllianceID==NULL) return NULL;
		dAllianceID += 0x38;
		dAllianceID = *(INT64*)dAllianceID;
		if(dAllianceID == NULL) return NULL;
		dAllianceID += 0x64;
		return *(DWORD*)dAllianceID;
	}
	catch(...)
	{
		return NULL;
	}
}
int CPledge::GetAllianceCrestID()
{
	try
	{
		INT64 dAllianceID = (INT64)GetAlliance();
		if(dAllianceID==NULL) return NULL;
		dAllianceID += 0x38;
		dAllianceID = *(INT64*)dAllianceID;
		if(dAllianceID == NULL) return NULL;
		dAllianceID += 0xA0;
		return *(DWORD*)dAllianceID;
	}
	catch(...)
	{
		return NULL;
	}
}
PWCHAR CPledge::GetAllianceName()
{
	try
	{
		INT64 dAllianceID = (INT64)GetAlliance();
		if(dAllianceID==NULL) return L"";
		dAllianceID += 0x38;
		dAllianceID = *(INT64*)dAllianceID;
		if(dAllianceID == NULL) return L"";
		dAllianceID += 0x68;
		return (PWCHAR)dAllianceID;
	}
	catch(...)
	{
		return L"";
	}
}
PledgeVector CPledge::GetEnemyList()
{
	typedef void (__fastcall *__up)(CPledge*, void*);
	__up _up = (__up) 0x00887C60;
	BYTE pVectorBuff[64];
	memset(pVectorBuff, 0x00, 64);
	_up(this, pVectorBuff);
	
	return *(PledgeVector*)&pVectorBuff; 
}
CPledge *CPledge::GetEnemyPledgeByName(PWCHAR wName)
{
	CPledge *pPledge= NULL;
	PledgeVector::iterator Iter;
	PledgeVector vEnemy = GetEnemyList();
	for(Iter=vEnemy.begin();Iter!=vEnemy.end();Iter++)
	{
		CPledge *pTmp = *Iter;
		if(pTmp)
		{
			if(!wcscmp(pTmp->pSD->wszClanName, wName))
			{
				pPledge = pTmp;
				break;
			}
		}
	}
	
	return pPledge;
}

void CPledge::GetAttackerList(PledgeVector& attackers)
{
	PledgeLock->Lock(__FILEW__, __LINE__);
	for(set<UINT>::iterator it = m_AttackerList.begin(); it!=m_AttackerList.end();it++)
	{
		if(CPledge *pPledge = GetPledge(*it))
		{
			attackers.push_back(pPledge);
		}
	}
	PledgeLock->Unlock();
}

void CPledge::GetWarList(PledgeVector& pledges)
{
	PledgeLock->Lock(__FILEW__, __LINE__);
	for(map<UINT, UINT>::iterator it = m_WarList.begin(); it!=m_WarList.end();it++)
	{
		if(CPledge *pPledge = GetPledge(it->first))
		{
			pledges.push_back(pPledge);
		}
	}
	PledgeLock->Unlock();
}

void CPledge::GetAllMembers(UserVector& members)
{
	guard;
	PledgeLock->Lock();
	for(set<UINT>::iterator it = m_OnlineMembers.begin(); it!= m_OnlineMembers.end(); it++)
	{
		UINT userId = (*it);
		if(User *pUser = User::GetUserBySID(&userId))
		{
			members.push_back(pUser);
		}
	}
	PledgeLock->Unlock();
	unguard;
}
bool CPledge::IsInWarWith(int nPledgeID)
{
	bool bInWar = false;
	if(this && pSD && nPledgeID)
	{
		PledgeVector::iterator Iter;
		PledgeVector vPledge;
		GetWarList(vPledge);
		for(UINT n=0;n<vPledge.size();n++)
		{
			CPledge *pPledge = vPledge[n];
			if(pPledge && pPledge->pSD)
			{
				if(pPledge->pSD->nDBID == nPledgeID)
				{
					bInWar = true;
					break;
				}
			}
		}
	}
	return bInWar;
}

bool CPledge::IsInWarWithEx(UINT sid)
{
	bool bInWar = false;
	PledgeLock->Lock(__FILEW__, __LINE__);
	for(map<UINT, UINT>::iterator it = m_WarList.begin();it!=m_WarList.end();it++)
	{
		if(it->first == sid)
		{
			if(it->second == 1)
			{
				bInWar = true;
			}
			break;
		}
	}

	PledgeLock->Unlock();
	
	return bInWar;
}

void CPledge::Realase(int nOption)
{
	typedef void (__thiscall *__fnCPledge)(CPledge*, PWCHAR, DWORD dLine, DWORD dOption , DWORD dNull);
	__fnCPledge fnCPledge = (__fnCPledge) 0x007613B0; 
	
	if(this!=NULL)
		fnCPledge(this, __FILEW__, __LINE__, nOption, 0);
}

void CPledge::NotifyMemberKilledDuringWar(const WCHAR* wName)
{
	guard;

	UserVector vUsers;
	GetAllMembers(vUsers);
	//1782	1	a,Your clan member $s1 was killed. $s2 points have been deducted from your clan's reputation score and added to your opponent's clan reputation score.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
	CSystemMessage msg(1782);
	msg.AddText(wName);
	msg.AddNumber(1);
	for(UINT n=0;n<vUsers.size();n++)
	{
		if(User *pUser = vUsers[n]->SafeCastUser())
		{
			pUser->SendSystemMessage(&msg);
		}
	}
	unguard;
}

void CPledge::NotifyMemberKilledEnemyDuringWar()
{	
	guard;

	UserVector vUsers;
	GetAllMembers(vUsers);
	//1783	1	a,For killing an opposing clan member, $s1 points have been deducted from your opponents' clan reputation score.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
	CSystemMessage msg(1783);
	msg.AddNumber(1);
	for(UINT n=0;n<vUsers.size();n++)
	{
		if(User *pUser = vUsers[n]->SafeCastUser())
		{
			pUser->SendSystemMessage(&msg);
		}
	}
	unguard;
}

void CPledge::NotifySubPledgeCreated(int nPledgeType)
{
	guard;

	UserVector vUsers;
	GetAllMembers(vUsers);
	CHAR buff[8190];
	int len = 0;

	if(pSD->pPledgeEx->mSubUnits.size() > 0)
	{
		SubUnitMap::iterator Iter = pSD->pPledgeEx->mSubUnits.find(nPledgeType);
		if(Iter!=pSD->pPledgeEx->mSubUnits.end())
		{
			len = Assemble(buff, sizeof(buff), "chddSS", 0xFE, 0x3F, 1, nPledgeType, Iter->second.Name.c_str(), Iter->second.LeaderName.c_str());
		}
	}

	if(len > 0)
	{
		for(UINT n=0;n<vUsers.size();n++)
		{
			if(User *pUser = vUsers[n]->SafeCastUser())
			{
				pUser->pSocket->Send("b", len, buff);
			}
		}
	}
	unguard;
}

void CPledge::NotifyMemberGraduateFromAcademy(User *pUser, int nReputation)
{
	guard;
	//1749	1	Congratulations! You will now graduate from the Clan Academy and leave your current clan. As a graduate of the academy, you can immediately join a clan as a regular member without being subject to any penalties.
	pUser->pSocket->SendSystemMessage(1749);
	
	UserVector vUsers;
	GetAllMembers(vUsers);
	//1748	1	a,Clan Academy member $s1 has successfully completed the 2nd class transfer and obtained $s2 Clan Reputation points.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
	CSystemMessage msg(1748);
	msg.AddText(pUser->pSD->wszName);
	msg.AddNumber(nReputation);
	for(UINT n=0;n<vUsers.size();n++)
	{
		if(User *pMember = vUsers[n]->SafeCastUser())
		{
			if(pMember->nObjectID != pUser->nObjectID)
			{
				pMember->SendSystemMessage(&msg);
			}
		}
	}
	unguard;
}

MemberData* CPledge::GetMemberDataByDBID(int nDBID)
{	
	MemberData *pData = NULL;
	if(this)
	{
		PledgeLock->Lock(__FILEW__, __LINE__);
		for(MemberDataMap::iterator Iter=m_Members.begin(); Iter!=m_Members.end(); Iter++)
		{
			MemberData *pMember = Iter->second;
			if(pMember)
			{
				try
				{
					if(pMember->nDBID == nDBID)
					{
						pData = pMember;
						break;
					}
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s] Invalid member data[%d] pledge[%d]", __FUNCTION__, pSD->nDBID, nDBID);
					break;
				}
			}
		}
		PledgeLock->Unlock();
	}
	return pData;
}
int CPledge::GetApprenticeDBID(int nSponsorID)
{
	int nDBID = 0;
	if(this && pSD)
	{
		PledgeLock->Lock(__FILEW__, __LINE__);
		try
		{
			for(MemberDataMap::iterator Iter=m_Members.begin(); Iter!=m_Members.end(); Iter++)
			{
				MemberData *pMember = Iter->second;
				if(pMember)
				{
					try
					{
						if(pMember->nSponsorID == nSponsorID)
						{
							nDBID = pMember->nDBID;
							break;
						}
					}catch(...)
					{
						g_Log.Add(CLog::Error, "[%s] Invalid member data[%d] pledge[%d]", __FUNCTION__, pSD->nDBID, nDBID);
						break;
					}
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
		}
		PledgeLock->Unlock();
	}
	return nDBID;
}

MemberData *CPledge::GetApprenticeData(int nSponsorID)
{
	MemberData *pData = NULL;
	if(this)
	{
		PledgeLock->Lock(__FILEW__, __LINE__);
		try
		{
			for(MemberDataMap::iterator Iter=m_Members.begin(); Iter!=m_Members.end(); Iter++)
			{
				MemberData *pMember = Iter->second;
				if(pMember)
				{
					if(pMember->nSponsorID == nSponsorID)
					{
						pData = pMember;
						break;
					}
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
		}
		PledgeLock->Unlock();
	}
	return pData;
}
MemberData *CPledge::GetMemberDataByName(const PWCHAR wsName)
{
	MemberData *pData = NULL;
	if(this)
	{
		PledgeLock->Lock(__FILEW__, __LINE__);
		try
		{
			for(MemberDataMap::iterator Iter=m_Members.begin(); Iter!=m_Members.end(); Iter++)
			{
				MemberData *pMember = Iter->second;
				if(pMember)
				{
					if(!wcscmp(wsName, pMember->wszName))
					{
						pData = pMember;
						break;
					}
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected - user[%S]", __FUNCTION__, wsName);
			pData = NULL;
		}
		PledgeLock->Unlock();
	}
	return pData;
}

int CPledge::GetReputation()
{
	if(this && pSD)
	{
		PledgeLock->Lock(__FILEW__, __LINE__);
		int nReputation = pSD->nReputation;
		PledgeLock->Unlock();
		return nReputation;
	}
	return NULL;
}

void CPledge::UpdatePledgeNameValue(int value)
{
	guard;
	PledgeLock->Lock(__FILEW__, __LINE__);
	INT64 total = pSD->nReputation + value;
	INT32 oldReputation = pSD->nReputation;
	if(total < INT_MAX)
	{
		value = total;
	}else
	{
		value = INT_MAX;
	}
	pSD->nReputation = value;
	PledgeLock->Unlock();
	g_DB.RequestSetPledgeReputation(pSD->nDBID, value);

	if((value < 1) && (oldReputation > 0))
	{
		//1789	1	a,Since the Clan Reputation Score has dropped to 0 or lower, your clan skill(s) will be de-activated.\0	0	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,none\0
		CSystemMessage msg(1789);
		Broadcast(msg.GetSize(), msg.GetBuff());

		UserVector vUsers;
		GetAllMembers(vUsers);
		for(UINT n=0;n<vUsers.size();n++)
		{
			if(User *pMember = vUsers[n]->SafeCastUser())
			{
				DeletePledgeSkills(pMember);
			}
		}
	}

	if((value > 0) && (oldReputation < 1))
	{
		//1862	1	a,Clan skills will now be activated since the clan's reputation score is 0 or higher.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
		CSystemMessage msg(1862);
		Broadcast(msg.GetSize(), msg.GetBuff());

		UserVector vUsers;
		GetAllMembers(vUsers);
		
		for(UINT n=0;n<vUsers.size();n++)
		{
			if(User *pMember = vUsers[n]->SafeCastUser())
			{
				AcquirePledgeSkills(pMember);
			}
		}
	}

	UpdateInfo();
	unguard;
}

int CPledge::IsCaptainOfSubpledge(PWCHAR wszName)
{	
	if(this && pSD)
	{
		if(pSD->pPledgeEx->mSubUnits.size() > 0)
		{
			SubUnitMap::iterator Iter = pSD->pPledgeEx->mSubUnits.begin();
			for(;Iter != pSD->pPledgeEx->mSubUnits.end();Iter++)
			{
				if(!Iter->second.LeaderName.compare(wszName))
					return Iter->second.nID;
			}
		}
	}
	return NULL;
}
int CPledge::GetDefaultRank(int nPledgeType)
{
	int nRank = 6;
	if(nPledgeType == -1)
		nRank = 9;
	if(nPledgeType == 100 || nPledgeType == 200)
		nRank = 7;
	if(nPledgeType > 1000 && nPledgeType < 2003)
		nRank = 8;
	return nRank;
}

bool CPledge::CanJoin(int nPledgeType)
{	
	int nMembers = GetSubPledgeMembers(nPledgeType);
	switch(nPledgeType)
	{
	case MainClan:
		{
			int nLevel = pSD->nLevel;
			if(nLevel < 1)
			{
				if(nMembers < g_ClanMembersLimit.MainClan0)
					return true;
			}else if(nLevel < 2)
			{
				if(nMembers < g_ClanMembersLimit.MainClan1)
					return true;
			}else if(nLevel < 3)
			{
				if(nMembers < g_ClanMembersLimit.MainClan2)
					return true;
			}else if(nLevel < 4)
			{
				if(nMembers < g_ClanMembersLimit.MainClan3)
					return true;
			}else if(nMembers < g_ClanMembersLimit.MainClan4)
				return true;
			break;
		}
	case Academy:
		{
			if(nMembers < g_ClanMembersLimit.Academy)
				return true;
			break;
		}
	case RoyalGuardI:
		{
			if(nMembers < g_ClanMembersLimit.RoyalGuard)
				return true;
			break;
		}
	case RoyalGuardII:
		{
			if(nMembers < g_ClanMembersLimit.RoyalGuard)
				return true;
			break;
		}
	case OrderOfKnightI:
		{
			if(nMembers < g_ClanMembersLimit.OrderOfKnight)
				return true;
			break;
		}
	case OrderOfKnightII:
		{
			if(nMembers < g_ClanMembersLimit.OrderOfKnight)
				return true;
			break;
		}
	case OrderOfKnightIII:
		{
			if(nMembers < g_ClanMembersLimit.OrderOfKnight)
				return true;
			break;
		}
	case OrderOfKnightIV:
		{
			if(nMembers < g_ClanMembersLimit.OrderOfKnight)
				return true;
			break;
		}
	default:
		{
			g_Log.Add(CLog::Error, "[%s][%d] Invalid pledge type[%d]", __FILE__, __LINE__, nPledgeType);
			break;
		}
	}
	return false;
}

void CPledge::NotifyMemberNamedHero(PWCHAR wszName)
{
	guard;

	if(this)
	{
		UpdateInfo();
		//1776	1	a,Clan member $s1 was named a hero. $s2 points have been added to your clan's reputation score.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
		CSystemMessage msg(1776);
		msg.AddText(wszName);
		msg.AddNumber(1000);
		UserVector vUsers;
		GetAllMembers(vUsers);
		for(UINT n=0;n<vUsers.size();n++)
		{
			if(User *pUser = vUsers[n]->SafeCastUser())
			{
				pUser->SendSystemMessage(&msg);
			}
		}
	}
	unguard;
}

void CPledge::NotifyReputationDecreased(int nPoints)
{
	guard;
	UserVector vUsers;
	GetAllMembers(vUsers);
	CSystemMessage msg(SystemMessage::$S1_POINTS_HAVE_BEEN_DEDUCTED_FROM_THE_CLAN_S_REPUTATION_SCORE_);
	msg.AddNumber(-nPoints);
	for(UINT n=0;n<vUsers.size();n++)
	{
		if(User *pUser = vUsers[n]->SafeCastUser())
		{
			pUser->pSocket->Send("b", msg.GetSize(), msg.GetBuff());
		}
	}
	unguard;
}

void CPledge::NotifyReputationIncreased(int nPoints)
{
	guard;
	UserVector vUsers;
	GetAllMembers(vUsers);
	CSystemMessage msg(SystemMessage::YOUR_CLAN_HAS_ADDED_$1S_POINTS_TO_ITS_CLAN_REPUTATION_SCORE_);
	msg.AddNumber(nPoints);
	for(UINT n=0;n<vUsers.size();n++)
	{
		if(User *pMember = vUsers[n]->SafeCastUser())
		{
			pMember->pSocket->Send("b", msg.GetSize(), msg.GetBuff());
		}
	}
	unguard;
}

void CPledge::NotifyMemberGradeChanged(PWCHAR wszName, int nRank)
{
	guard;
	//1761	1	a,Clan member $s1's privilege level has been changed to $s2.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
	UserVector vUsers;
	GetAllMembers(vUsers);
	CSystemMessage msg(1761);
	msg.AddText(wszName);
	msg.AddNumber(nRank);
	for(UINT n=0;n<vUsers.size();n++)
	{
		if(User *pMember = vUsers[n]->SafeCastUser())
		{
			pMember->pSocket->Send("b", msg.GetSize(), msg.GetBuff());
		}
	}
	unguard;
}

void CPledge::UpdateWindowPacket(User *pUser, bool bIsOnline)
{
	if(this && pSD)
	{
		if(pUser->pSD)
		{
			CHAR cBuff[8192];
			memset(cBuff, 0, 8192);
			int nBuffLen = 0;
			int nClass = pUser->pSD->nClass;
			int nRace = pUser->pSD->nRace;
			int nGander = pUser->pSD->nSex;
			int nLevel = pUser->pSD->nLevel;
			int nObjectID = 0;
			if(bIsOnline)
				nObjectID = pUser->nObjectID;
			
			try
			{
				MemberData *pMember = GetMemberDataByDBID(pUser->nDBID);
				if(pMember)
				{
					pMember->nClass = nClass;
					pMember->nRace = nRace;
					pMember->nGender = nGander; 
					pMember->nLevel = nLevel;
					pMember->nObjectID = nObjectID;
					nBuffLen = Assemble(cBuff, 8192, "Sddddddd", pUser->pSD->wszName, nLevel,
						nClass, nGander, nRace, nObjectID, pMember->nPledgeType, pMember->nSponsorID);
				}
			}catch(...)
			{
				EXCEPTION_LOG;
				return;
			}
			UserVector vUsers;
			GetAllMembers(vUsers);
			for(UINT n=0;n<vUsers.size();n++)
			{
				if(User *pMember = vUsers[n]->SafeCastUser())
					pMember->pSocket->Send("cb", 0x54, nBuffLen, cBuff);
			}
		}
	}
}

void CPledge::SubUnitCreatedPacket(User *pUser, int nPledgeType)
{
	if(this && pSD && pUser)
	{
		try
		{
			if(pSD->pPledgeEx->mSubUnits.size() > 0)
			{
				SubUnitMap::iterator Iter = pSD->pPledgeEx->mSubUnits.find(nPledgeType);
				if(Iter!=pSD->pPledgeEx->mSubUnits.end())
				{
					CSocket *pSocket = pUser->GetSocket();
					if(pSocket->ValidUserSocket())
					{
						pSocket->Send("chddSS", 0xFE, 0x3F, 1, nPledgeType, Iter->second.Name.c_str(), Iter->second.LeaderName.c_str());
					}
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}

void CPledge::UpdateMemberList(MemberData *pData)
{	
	if(this && pData)
	{
		CPacket pck;
		pck.WriteC(0x54);
		pck.WriteS(pData->wszName);
		pck.WriteD(pData->nLevel);
		pck.WriteD(pData->nClass);
		pck.WriteD(pData->nGender);
		pck.WriteD(pData->nRace);
		pck.WriteD(pData->nObjectID);
		pck.WriteD(pData->nPledgeType);
		pck.WriteD(pData->nSponsorID);
		Broadcast(pck.GetSize(), pck.GetBuff());
	}
}
bool CPledge::IsValidSubPledgeName(wstring wName)
{
	if(this && pSD)
	{
		if( wName.size() > 2 && wName.size() < 16)
		{
			for(SubUnitMap::iterator Iter = pSD->pPledgeEx->mSubUnits.begin(); Iter!=pSD->pPledgeEx->mSubUnits.end();Iter++)
			{
				if(Iter->second.Name == wName)
					return false;
			}
			return true;
		}
	}
	return false;
}

void CPledge::OpenAllWindowPacket(User *pUser, bool bGM, PWCHAR wszActiveUser)
{
	CSocket *pSocket = pUser->GetSocket();
	if(this && pSD && pUser && pSocket)
	{
		pSocket->Send("c", 0x82);
		//clearing clan skills
		pSocket->Send("chd", 0xFE, 0x39, 0);
		int nPledgeType = 0;
		//Creating subpledges
		try
		{
			for(SubUnitMap::iterator Iter = pSD->pPledgeEx->mSubUnits.begin(); Iter!=pSD->pPledgeEx->mSubUnits.end();Iter++)
			{
				pSocket->Send("chddSS", 0xFE, 0x3F, 1, Iter->second.nID, Iter->second.Name.c_str(), Iter->second.LeaderName.c_str());
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s][%d] Exception Detected.", __FUNCTION__, __LINE__);
		}
		//Send SubPledgeMembers
		//and
		//prepare MainClan Memberlist
		char cBuff[8192];
		int nBuffLen = 0;
		int nMemberAmount = 0;
		
		PledgeLock->Lock(__FILEW__, __LINE__);
		try
		{
			for(MemberDataMap::iterator iter=m_Members.begin(); iter!=m_Members.end(); iter++)
			{
				MemberData *pMember = iter->second;
				if(pMember)
				{
					if(pMember->nPledgeType!=MAIN_CLAN)
					{
						//Send SubPledgeMember
						pSocket->Send("cSdddddd", 0x55, 
							pMember->wszName,
							pMember->nLevel,
							pMember->nClass,
							pMember->nGender,
							pMember->nRace,
							pMember->nObjectID,
							pMember->nPledgeType);
						if(pMember->nSponsorID)
						{
							pSocket->Send("cSddddddd", 0x54, 
							pMember->wszName,
							pMember->nLevel,
							pMember->nClass,
							pMember->nGender,
							pMember->nRace,
							pMember->nObjectID,
							pMember->nPledgeType,
							pMember->nSponsorID);
						}
						//g_Log.Add(CLog::Blue, "[Ext] Sending SubPledgeMemberInfo pledgeID[%d] SubPledge[%d] wszMember[%ls].", pPledgeSharedData->nDBID, pPledgeMember->nPledgeType, pPledgeMember->wszName);
					}else
					{
						//Add Member to buffer for MainClan
						nMemberAmount++;
						nBuffLen += Assemble(&cBuff[nBuffLen], (8192-nBuffLen), "Sdddddd",
							pMember->wszName, pMember->nLevel, pMember->nClass, pMember->nGender, pMember->nRace, pMember->nObjectID, pMember->nSponsorID);
					}
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s][%d] Exception Detected.", __FUNCTION__, __LINE__);
		}
		int nReputation = pSD->nReputation;
		PledgeLock->Unlock();
		int nAllyID = GetAllianceID();
		PWCHAR wszAllianceName = L"";
		if(nAllyID!=NULL)
			wszAllianceName = GetAllianceName();
		int nAllianceCrestID = GetAllianceCrestID();
		int nInWar = 0;
		int nAgitID = pSD->nAgitID;
		if(nAgitID == 20)
			nAgitID = 64;
		nPledgeType = 0;
		if(nIsInWar != 0)
			nInWar = 1;
		//Prepare packet
		if(!bGM)
		{		
			pSocket->Send("cdddSSdddddddddSdddb", 0x53, 0, pSD->nDBID, 0, pSD->wszClanName,
				pSD->wszClanLeaderName, pSD->nCrestID, pSD->nLevel,
				pSD->nCastleID, nAgitID, 10, nReputation,
				pSD->nState, 0, nAllyID, wszAllianceName, nAllianceCrestID, nInWar, nMemberAmount, nBuffLen, cBuff);
		}
		else
		{
			pSocket->Send("cSddSSdddddddddSdddb", 0x90, wszActiveUser, pSD->nDBID, nPledgeType, pSD->wszClanName, pSD->wszClanLeaderName,
				pSD->nCrestID, pSD->nLevel,	pSD->nCastleID, nAgitID, 0, 0,
				0, 0, nAllyID, wszAllianceName, nAllianceCrestID, nInWar, nMemberAmount, nBuffLen, cBuff);
		}
	}
}

void CPledge::AddWindowPacket(User *pUser)
{
	if(this && pSD && pUser && pUser->pSD)
	{
		if(pUser->ValidUser())
		{
			int nDBID = pUser->nDBID;
			int nClass = pUser->pSD->nClass;
			int nLevel = pUser->pSD->nLevel;
			int nGander = pUser->pSD->nSex;
			int nRace = pUser->pSD->nRace;
			int nObjectID = pUser->nObjectID;
			PWCHAR wszName = pUser->pSD->wszName;
			int nPledgeType = 0;
			int nSponsorID = 0;
			
			MemberData *pMember = GetMemberDataByDBID(nDBID);
			if(pMember)
			{
				try
				{
					nPledgeType = pMember->nPledgeType;
					nSponsorID = pMember->nSponsorID;
				}catch(...)
				{
					g_Log.Add(CLog::Error,"[%s] Exception Detected", __FUNCTION__);
				}
			}
			//Sending packet
			UserVector vUsers;
			GetAllMembers(vUsers);
			for(UINT n=0;n<vUsers.size();n++)
			{
				if(User *pMember = vUsers[n]->SafeCastUser())
				{
					if(pMember->nObjectID != pUser->nObjectID)
					{
						pMember->pSocket->Send("cSdddddd", 0x55, wszName, nLevel, nClass, nGander, nRace, nObjectID, nPledgeType );
						if(nSponsorID)
						{
							pMember->pSocket->Send("cSddddddd", 0x54, wszName, nLevel, nClass, nGander, nRace, nObjectID, nPledgeType, nSponsorID );
						}
					}
				}
			}
		}
	}
}

void CPledge::AddDBMemberByUser(User *pUser)
{
	typedef void (__thiscall *_Add)(CPledge*, User*);
	_Add Add = (_Add) 0x00765EB0;
	Add(this, pUser);
}

void CPledge::AddDBMemberByDBID(int nDBID, PWCHAR wszName, int nLevel, int nClass, int nGander, int nRace, int nServerID)
{
	typedef void (__thiscall *_Add)(CPledge*, int nDBID, PWCHAR wszName, int nLevel, int nClass, int nGander, int nRace, int nServerID);
	_Add Add = (_Add) 0x00765B20;
	Add(this, nDBID, wszName, nLevel, nClass, nGander, nRace, nServerID);
}

void CPledge::DeleteDBMeberByDBID(int nDBID)
{
	typedef void (__thiscall *_Del)(CPledge*, int nDBID);
	_Del Del = (_Del) 0x007658A0;
	Del(this, nDBID);
}

void CPledge::UpdateMembersPrivilege(int nRank)
{
	if(pSD && nRank > 0 && nRank < 10)
	{
		UserVector vUsers;
		GetAllMembers(vUsers);
		for(UINT n=0;n<vUsers.size();n++)
		{
			if(User *pUser = vUsers[n]->SafeCastUser())
			{
				MemberData *pData = GetMemberDataByDBID(pUser->nDBID);
				try
				{
					if(pData)
					{
						if(pData->nRank == nRank)
						{
							pUser->pSD->n64_PledgePower1 = ClanSys::C5ToC4Privilege(pSD->PowerData[nRank]);
						//	pUser->pSD->pledgePower = pSD->PowerData[nRank];
							g_DB.RequestSavePledgePower(pUser);
						}
					}
				}catch(...)
				{
					continue;
				}
			}
		}
	}
}

User *CPledge::GetMember(PWCHAR wszName)
{	
	User *pUser = NULL;
	if(this && pSD)
	{
		UINT nObjectID = NULL;
		PledgeLock->Lock(__FILEW__, __LINE__);
		for(MemberDataMap::iterator Iter=m_Members.begin(); Iter!=m_Members.end(); Iter++)
		{
			MemberData *pMember = Iter->second;
			if(pMember)
			{
				if(!wcscmp(wszName, pMember->wszName))
				{
					nObjectID = pMember->nObjectID;
				}
			}
		}
		PledgeLock->Unlock();
		if(nObjectID)
		{
			pUser = User::GetUserBySID(&nObjectID);
		}
	}
	return pUser;
}
int CPledge::GetSubPledgeMembers(int nPledgeType)
{	
	int nMembers = 0;
	if(this && pSD)
	{
		PledgeLock->Lock(__FILEW__, __LINE__);
		for(MemberDataMap::iterator Iter=m_Members.begin(); Iter!=m_Members.end(); Iter++)
		{
			MemberData *pMember = Iter->second;
			if(pMember)
			{
				if(pMember->nPledgeType == nPledgeType)
					nMembers++;
			}
		}
		PledgeLock->Unlock();
	}
	
	return nMembers;
}

void CPledge::Broadcast(size_t nLen, PCHAR packet)
{
	guard;
	UserVector vUsers;
	GetAllMembers(vUsers);
	for(UINT n=0;n<vUsers.size();n++)
	{
		if(User *pMember = vUsers[n]->SafeCastUser())
		{
			pMember->pSocket->Send("b", nLen, packet);
		}
	}
	unguard;
}

void CPledge::SendEnemyList(User *pUser)
{	
		if(this && pUser->ValidUser())
	{
		PledgeVector vPledge = GetEnemyList();
		PledgeVector::iterator Iter;
		//Preparing a packet
		char buff[8190];
		memset(buff, 0, sizeof(buff));
		int nLen = 0;
		int nEnemyAmount = 0;
		for(Iter=vPledge.begin(); Iter!=vPledge.end(); Iter++)
		{
			CPledge *pPledge = (*Iter);
			if(pPledge)
			{
				nLen += Assemble(&buff[nLen], (sizeof(buff)-nLen), "Sdd", pPledge->GetName(), 0, 0);
				nEnemyAmount++;
			}
		}
		if(nEnemyAmount > 0)
			pUser->GetSocket()->Send("chdddb", 0xFE, 0x3E, 0, 0, nEnemyAmount, nLen, buff);
		else
			pUser->GetSocket()->Send("chddd", 0xFE, 0x3E, 0, 0, 0);
	}
}

void CPledge::SendAttackerList(User *pUser)
{	
	if(this && pUser->ValidUser())
	{
		PledgeVector vPledge;
		GetAttackerList(vPledge);
		PledgeVector::iterator Iter;
		//Preparing a packet
		char buff[8190];
		memset(buff, 0, sizeof(buff));
		int nLen = 0;
		int nAttackerAmount = 0;
		for(Iter=vPledge.begin(); Iter!=vPledge.end(); Iter++)
		{
			CPledge *pPledge = *Iter;
			if(pPledge)
			{
				nLen += Assemble(&buff[nLen], (sizeof(buff)-nLen), "Sdd", pPledge->GetName(), 1, 0);
				nAttackerAmount++;
			}
		}
		if(nAttackerAmount > 0)
			pUser->GetSocket()->Send("chdddb", 0xFE, 0x3E, 1, 0, nAttackerAmount, nLen, buff);
		else
			pUser->GetSocket()->Send("chddd", 0xFE, 0x3E, 1, 0, 0);
	}
}
bool CPledge::CanAcceptNewMember()
{
	typedef bool (*canm)(CPledge*);
	canm _canm = (canm) 0x00761E50;
	return _canm(this);
}
bool CPledge::IsWorkingWithAnotherClan(int nTimeout)
{
	typedef bool (*canm)(CPledge*, int);
	canm _canm = (canm) 0x0085DFF0;
	return _canm(this, nTimeout);
}

void CPledge::SendSkillList(User *pUser)
{
	guard;
	if(this && pSD && pUser->ValidUser())
	{
		vector<PledgeSkillInfo> acquirableSkills;
		pSD->pPledgeEx->m_Lock.Enter();
		g_PledgeSkillDb.GetAcquirableSkills(pSD->nLevel, acquirableSkills, pSD->pPledgeEx->m_Skills);
		pSD->pPledgeEx->m_Lock.Leave();
		CPacket packet;
		packet.WriteC(0x8A);
		packet.WriteD(2);
		packet.WriteD(static_cast<UINT>(acquirableSkills.size()));
		for(UINT n=0;n<acquirableSkills.size();n++)
		{
			PledgeSkillInfo& psi = acquirableSkills[n];
			packet.WriteD(psi.skillId);
			packet.WriteD(psi.skillLevel);
			packet.WriteD(g_PledgeSkillDb.GetMaxLevel(psi.skillId));
			packet.WriteD(psi.reputationConsume);
			if(psi.itemConsume.size() > 0)
			{
				packet.WriteD(psi.itemConsume[0].itemId);
			}else
			{
				packet.WriteD(0);
			}
		}
		pUser->pSocket->Send("b", packet.GetSize(), packet.GetBuff());
	}
	unguard;
}

void CPledge::ValidateTotalLevel()
{
	if(this)
	{
		typedef void(*vtl)(CPledge*);
		vtl _vtl = (vtl) 0x00768030;
		_vtl(this);
	}
}

void CPledge::UpdateMembers()
{
	if(this && pSD)
	{
		UserVector vUsers;
		GetAllMembers(vUsers);

		for(UINT n=0;n<vUsers.size();n++)
		{
			if(User *pMember = vUsers[n]->SafeCastUser())
			{
				try
				{
					MemberData *pData = GetMemberDataByDBID(pMember->nDBID);
					if(pData && pData->nRank < 10 && pData->nRank > 0)
					{
						pMember->pSD->n64_PledgePower1 = ClanSys::C5ToC4Privilege(pSD->PowerData[pData->nRank]);
//						pMember->pSD->pledgePower = pSD->PowerData[pData->nRank];
					}
					AcquirePledgeSkills(pMember);
					for(SubUnitMap::iterator Iter = pSD->pPledgeEx->mSubUnits.begin(); Iter!=pSD->pPledgeEx->mSubUnits.end();Iter++)
					{
						pMember->pSocket->Send("chddSS", 0xFE, 0x3F, 1, Iter->second.nID, Iter->second.Name.c_str(), Iter->second.LeaderName.c_str());
					}
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
				}
			}
		}
	}
}
User *CPledge::GetMaster()
{
	typedef User*(*gm)(CPledge*);
	gm _gm = (gm) 0x00763D80;
	return _gm(this);
}
bool CPledge::HasSubUnit(int nPledgeType)
{
	if(nPledgeType == MAIN_CLAN)
			return true;
	try
	{
		if(this && pSD && pSD->pPledgeEx->mSubUnits.size() > 0)
		{
			SubUnitMap::iterator Iter = pSD->pPledgeEx->mSubUnits.begin();
			for(; Iter != pSD->pPledgeEx->mSubUnits.end(); Iter++)
			{
				if(Iter->second.nID == nPledgeType)
					return true;
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
	}
	return false;
}

void CPledge::DismissApprentice(MemberData *pData)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	if(pData && pData->nSponsorID && pSD)
	{
		MemberData *pSponsor = GetMemberDataByDBID(pData->nSponsorID);
		if(pSponsor)
		{
			pData->nSponsorID = 0;
			g_DB.RequestSetSponsor(pData->nDBID, 0, pSD->nDBID);
			if(pSponsor->nObjectID)
			{
				User *pTmp = User::GetUserBySID(&pSponsor->nObjectID);
				if(pTmp->ValidUser())
				{
					pTmp->pSD->nApprenticeSID = 0;
				}
			}
			if(pData->nObjectID)
			{
				User *pTmp = User::GetUserBySID(&pData->nObjectID);
				if(pTmp->ValidUser())
				{
					pTmp->pSD->nSponsorSID = 0;
				}
			}
			UpdateMemberList(pData);
			CSystemMessage msg(SystemMessage::$S2__CLAN_MEMBER_$C1_S_APPRENTICE__HAS_BEEN_REMOVED_);
			msg.AddText(pSponsor->wszName);
			msg.AddText(pData->wszName);
			Broadcast(msg.GetSize(), msg.GetBuff());
		}else
			g_Log.Add(CLog::Error, "[%s] Sponsor MemberData cannot be found - pledge_id[%d] sponsor_id[%d]", __FUNCTION__, pSD->nDBID, pData->nSponsorID);
	}
}
bool CPledge::CreateSubPledge(User *pUser, int nPledgeType, PWCHAR wszName)
{	
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	if(this && pSD)
	{
		CPledgeSharedData *pSD = GetSharedData();
		if(pSD)
		{
			int nPledgeLevel = pSD->nLevel;
			switch(nPledgeType)
			{
			case Academy:
				{
					if(nPledgeLevel > 4)
					{
						if(!HasSubUnit(Academy))
						{
							CSubUnit Unit;
							Unit.nID = Academy;
							Unit.nLeaderID = 0;
							Unit.Name = wszName;
							PledgeLock->Lock(__FILEW__, __LINE__);
							pSD->pPledgeEx->mSubUnits.insert(pair<int, CSubUnit>(nPledgeType, Unit));
							pSD->nHasAcademy = 1;
							PledgeLock->Unlock();
							g_DB.RequestCreateSubPledge(pSD->nDBID, nPledgeType, wszName);
							if(int repCost = g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ACADEMY_SUBPLEDGE))
							{
								UpdatePledgeNameValue(-repCost);
							}
							NotifySubPledgeCreated(nPledgeType);
							return true;
						}else pUser->GetSocket()->SendSystemMessage(L"You have already created the clan academy.");
					}else
						pUser->GetSocket()->SendSystemMessage(L"Your clan level does not meet the requirement to create a the clan academy.");
					break;
				}
			case RoyalGuardI:
				{
					if(nPledgeLevel > 5)
					{
						//Check if exists
						if(!HasSubUnit(RoyalGuardI))
						{
							CSubUnit Unit;
							Unit.nID = RoyalGuardI;
							Unit.nLeaderID = 0;
							Unit.Name = wszName;
							PledgeLock->Lock(__FILEW__, __LINE__);
							pSD->pPledgeEx->mSubUnits.insert(pair<int, CSubUnit>(nPledgeType, Unit));
							pSD->nHasSubUnit100 = 1;
							PledgeLock->Unlock();
							g_DB.RequestCreateSubPledge(pSD->nDBID, nPledgeType, wszName);
							if(int repCost = g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ROYAL_GUARD_SUBPLEDGE))
							{
								UpdatePledgeNameValue(-repCost);
							}
							NotifySubPledgeCreated(nPledgeType);
							return true;
						}else pUser->GetSocket()->SendSystemMessage(L"The 1st Royal Guard has already been created.");
				
					}
					break;
				}
			case RoyalGuardII:
				{
					if(nPledgeLevel > 5)
					{
						//Check if exists
						if(!HasSubUnit(RoyalGuardII))
						{
							CSubUnit Unit;
							Unit.nID = RoyalGuardII;
							Unit.nLeaderID = 0;
							Unit.Name = wszName;
							PledgeLock->Lock(__FILEW__, __LINE__);
							pSD->pPledgeEx->mSubUnits.insert(pair<int, CSubUnit>(nPledgeType, Unit));
							pSD->nHasSubUnit200 = 1;
							PledgeLock->Unlock();
							g_DB.RequestCreateSubPledge(pSD->nDBID, nPledgeType, wszName);
							if(int repCost = g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ROYAL_GUARD_SUBPLEDGE))
							{
								UpdatePledgeNameValue(-repCost);
							}
							NotifySubPledgeCreated(nPledgeType);
							return true;
						}else pUser->GetSocket()->SendSystemMessage(L"The 2nd Royal Guard has already been created.");
					}
					break;
				}
			case OrderOfKnightI:
				{
					if(nPledgeLevel > 6)
					{
						//Check if already exists & royal1 exists
						if(!HasSubUnit(OrderOfKnightI) && HasSubUnit(RoyalGuardI))
						{
							CSubUnit Unit;
							Unit.nID = OrderOfKnightI;
							Unit.nLeaderID = 0;
							Unit.Name = wszName;
							PledgeLock->Lock(__FILEW__, __LINE__);
							pSD->pPledgeEx->mSubUnits.insert(pair<int, CSubUnit>(nPledgeType, Unit));
							pSD->nHasSubUnit1001 = 1;
							PledgeLock->Unlock();
							g_DB.RequestCreateSubPledge(pSD->nDBID, nPledgeType, wszName);
							if(int repCost = g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ORDER_OF_KNIGHT_SUBPLEDGE))
							{
								UpdatePledgeNameValue(-repCost);
							}
							NotifySubPledgeCreated(nPledgeType);
							return true;
						}else pUser->GetSocket()->SendSystemMessage(L"The 1st Order of Knights has already been created.");
					}
					break;
				}
			case OrderOfKnightII:
				{
					if(nPledgeLevel > 6)
					{
						//Check if already exists & royal1 exists
						if(!HasSubUnit(OrderOfKnightII) && HasSubUnit(RoyalGuardI))
						{
							CSubUnit Unit;
							Unit.nID = OrderOfKnightII;
							Unit.nLeaderID = 0;
							Unit.Name = wszName;
							PledgeLock->Lock(__FILEW__, __LINE__);
							pSD->pPledgeEx->mSubUnits.insert(pair<int, CSubUnit>(nPledgeType, Unit));
							pSD->nHasSubUnit1002 = 1;
							PledgeLock->Unlock();
							g_DB.RequestCreateSubPledge(pSD->nDBID, nPledgeType, wszName);
							if(int repCost = g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ORDER_OF_KNIGHT_SUBPLEDGE))
							{
								UpdatePledgeNameValue(-repCost);
							}
							NotifySubPledgeCreated(nPledgeType);
							return true;
						}else pUser->GetSocket()->SendSystemMessage(L"The 2nd Order of Knights has already been created.");
					}
					break;
				}
			case OrderOfKnightIII:
				{
					if(nPledgeLevel > 6)
					{
						//Check if already exists & royal2 exists
						if(!HasSubUnit(OrderOfKnightIII) && HasSubUnit(RoyalGuardII))
						{
							CSubUnit Unit;
							Unit.nID = OrderOfKnightIII;
							Unit.nLeaderID = 0;
							Unit.Name = wszName;
							PledgeLock->Lock(__FILEW__, __LINE__);
							pSD->pPledgeEx->mSubUnits.insert(pair<int, CSubUnit>(nPledgeType, Unit));
							pSD->nHasSubUnit2001 = 1;
							PledgeLock->Unlock();
							g_DB.RequestCreateSubPledge(pSD->nDBID, nPledgeType, wszName);
							if(int repCost = g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ORDER_OF_KNIGHT_SUBPLEDGE))
							{
								UpdatePledgeNameValue(-repCost);
							}
							NotifySubPledgeCreated(nPledgeType);
							return true;
						}else pUser->GetSocket()->SendSystemMessage(L"The 3rd Order of Knights has already been created.");
					}
					break;
				}
			case OrderOfKnightIV:
				{
					if(nPledgeLevel > 6)
					{
						//Check if already exists & royal2 exists
						if(!HasSubUnit(OrderOfKnightIV) && HasSubUnit(RoyalGuardII))
						{
							CSubUnit Unit;
							Unit.nID = OrderOfKnightIV;
							Unit.nLeaderID = 0;
							Unit.Name = wszName;
							PledgeLock->Lock(__FILEW__, __LINE__);
							pSD->pPledgeEx->mSubUnits.insert(pair<int, CSubUnit>(nPledgeType, Unit));
							pSD->nHasSubUnit2002 = 1;
							PledgeLock->Unlock();
							g_DB.RequestCreateSubPledge(pSD->nDBID, nPledgeType, wszName);
							if(int repCost = g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ORDER_OF_KNIGHT_SUBPLEDGE))
							{
								UpdatePledgeNameValue(-repCost);
							}
							NotifySubPledgeCreated(nPledgeType);
							return true;
						}else pUser->GetSocket()->SendSystemMessage(L"The 4th Order of Knights has already been created");
					}
					break;
				}
			default:
				{
					g_Log.Add(CLog::Error, "[%s]Invalid PledgeType[%d] for pledge[%d]", __FUNCTION__, nPledgeType, pSD->nDBID);
					break;
				}
			}
		}
	}
	return false;
}
bool CPledge::SetSubCaptain(User *pUser, int nPledgeType, PWCHAR wszName)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	if(this && pSD && pUser->ValidUser() && nPledgeType > 0)
	{
		CPledgeSharedData *pSD = GetSharedData();
		if(pSD)
		{
			if(HasSubUnit(nPledgeType))
			{
				MemberData *pData = GetMemberDataByName(wszName);
				if(!pData || pData->nPledgeType != 0)
				{
					g_Log.Add(CLog::Error, "[%s] Cannot find user[%S] in pledge", __FUNCTION__, wszName);
				}
				if(pData && pData->nPledgeType == 0)
				{
					if( !IsCaptainOfSubpledge(pData->wszName) )
					{
						SubUnitMap::iterator Iter = pSD->pPledgeEx->mSubUnits.find(nPledgeType);
						if(Iter != pSD->pPledgeEx->mSubUnits.end())
						{
							Iter->second.nLeaderID = pData->nDBID;
							Iter->second.LeaderName = pData->wszName;
							g_DB.RequestSetSubCaptain(pSD->nDBID, nPledgeType, wszName, pData->nDBID);
							NotifySubPledgeCreated(nPledgeType);
							switch(Iter->first)
							{
							case CPledge::ROYAL_GUARD_I:
								{
									wcscpy_s(pSD->wsSubMaster100, 25, Iter->second.LeaderName.c_str());
									break;
								}
							case CPledge::ROYAL_GUARD_II:
								{
									wcscpy_s(pSD->wsSubMaster200, 25, Iter->second.LeaderName.c_str());
									break;
								}
							case CPledge::ORDER_OF_KNIGHTS_I:
								{
									wcscpy_s(pSD->wsSubMaster1001, 25, Iter->second.LeaderName.c_str());
									break;
								}
							case CPledge::ORDER_OF_KNIGHTS_II:
								{
									wcscpy_s(pSD->wsSubMaster1002, 25, Iter->second.LeaderName.c_str());
									break;
								}
							case CPledge::ORDER_OF_KNIGHTS_III:
								{
									wcscpy_s(pSD->wsSubMaster2001, 25, Iter->second.LeaderName.c_str());
									break;
								}
							case CPledge::ORDER_OF_KNIGHTS_IV:
								{
									wcscpy_s(pSD->wsSubMaster2002, 25, Iter->second.LeaderName.c_str());
									break;
								}
							default:
								{
									break;
								}
							}
							return true;
						}
					}else
						pUser->GetSocket()->SendSystemMessage(L"Selected member is already a Captain.");
				}else
					pUser->GetSocket()->SendSystemMessage(L"The Captain could not be found in the main clan.");
			}else 
				pUser->GetSocket()->SendSystemMessage(L"The sub-unit does not exist.");
		}
	}
	return false;
}

void CPledge::UpdateInfo(CUserSocket *pSocket)
{
	if(this && pSD)
	{
		PledgeLock->Lock(__FILEW__, __LINE__);
		CPacket packet;
		packet.WriteC(0x88);
		packet.WriteD(pSD->nDBID);
		packet.WriteD(pSD->nCrestID);
		packet.WriteD(pSD->nLevel);
		packet.WriteD(pSD->nCastleID);
		int nAgitID = pSD->nAgitID;
		if(nAgitID == 20)
			nAgitID = 64;
		packet.WriteD(nAgitID);
		packet.WriteD( 10); //Rank
		packet.WriteD(pSD->nReputation);
		packet.WriteD(pSD->nState);
		packet.WriteD(pSD->nIsGuilty);
		
		LPVOID pAllySD = GetAlliance();
		if(pAllySD)
		{
			pAllySD = (void*)*(UINT64*)((UINT64)pAllySD + 0x38);
			packet.WriteD(*(DWORD*)((UINT64)pAllySD+0x64));
			packet.WriteS((PWCHAR)((UINT64)pAllySD+0x68));
			packet.WriteD(*(DWORD*)((UINT64)pAllySD+0xA8));
			packet.WriteD(0); //allywar ? not used 
			if(pCastle || pAgit)
				packet.WriteD(pSD->_n00EC);
			else
				packet.WriteD(0);
		}else
		{
			packet.WriteD(0);
			packet.WriteS(L"");
			packet.WriteD(0);
			packet.WriteD(0);
			packet.WriteD(0);
		}
		PledgeLock->Unlock();
		if(pSocket)
		{
			pSocket->Send("b", packet.GetSize(), packet.GetBuff());
		}else
		{
			Broadcast(packet.GetSize(), packet.GetBuff());
		}
	}
}

void CPledge::NotifyPledgeSkillAcquired(int nSkillID, int nLevel)
{
	UserVector vUsers;
	GetAllMembers(vUsers);
	CSystemMessage msg(1788);
	msg.AddSkill(nSkillID, nLevel);
	for(UINT n=0;n<vUsers.size();n++)
	{
		if(User *pMember = vUsers[n]->SafeCastUser())
		{
			pMember->pSocket->Send("b", msg.GetSize(), msg.GetBuff());
			AcquirePledgeSkills(pMember);
		}
	}
}

void CPledge::AcquirePledgeSkills(User *pUser)
{
	if(pUser->ValidUser() && this && pSD)
	{
		CPacket packet;
		if(pSD->nReputation > 0)
		{
			UINT nPledgeClass = pUser->GetPledgeClass();
			UINT nSkillCount = 0;
			try
			{
				pSD->pPledgeEx->m_Lock.Enter();
				for(PledgeSkillMap::iterator Iter = pSD->pPledgeEx->m_Skills.begin(); Iter!=pSD->pPledgeEx->m_Skills.end();Iter++)
				{
					PledgeSkillInfo& psi = Iter->second;
					if(psi.skillId)
					{
						if(psi.requiredPledgeClass <= nPledgeClass)
						{
							pUser->AcquireSkill(psi.skillId, psi.skillLevel);
						}
						packet.WriteD(psi.skillId);
						packet.WriteD(psi.skillLevel);
						nSkillCount++;
					}
				}
				pSD->pPledgeEx->m_Lock.Leave();
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
			}
			pUser->pSocket->Send("chdb", 0xFE, 0x39, nSkillCount, packet.GetSize(), packet.GetBuff());
			pUser->SetValidateParametersRequest();
		}
	}
}

void CPledge::DeletePledgeSkills(User *pUser)
{	
	if(pUser)
	{
		try
		{
			for(int n=370;n<392;n++)
				pUser->DeleteSkill(n);
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
			return;
		}
		pUser->ValidateParameters();
		pUser->GetSocket()->Send("chd", 0xFE, 0x39, 0);
	}
}

CPledge* CPledge::GetPledge(UINT sid)
{
	typedef CPledge*(*f)(UINT);
	return f(0x410A60L)(sid);
}
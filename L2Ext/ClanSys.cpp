#include "StdAfx.h"
#include "ClanSys.h"
#include "HTMLCacheManager.h"
#include "DB.h"
#include "PlayerAction.h"
#include "HTMLCache.h"

CPledgeSkillDb g_PledgeSkillDb;

static map<int, int> mNpcReputation;
INT32 g_PledgeLevelUpRequirements[9][5];
BOOL g_ReputationInPvPZone = FALSE;
BOOL g_AttackWarWithoutCTRL = FALSE;
UINT g_ClanLeaderTitleColor = 0;
UINT g_ClanLeaderNicknameColor = 0;

extern MembersLimit g_ClanMembersLimit;

CRITICAL_SECTION g_ExpelBannedUsersLock;

void ClanSys::DeleteExpelLock(LPVOID lpRWLock)
{
	DeleteCriticalSection(&g_ExpelBannedUsersLock);
	typedef void (*f)(LPVOID);
	f(0x781BE0L)(lpRWLock);
}

DWORD ClanSys::EnterExpelLock(LPVOID lpRWLock, const WCHAR* wFunction, int line)
{
	guard;
	EnterCriticalSection(&g_ExpelBannedUsersLock);

	typedef DWORD(*f)(LPVOID, const WCHAR*, int);
	DWORD ret = f(0x7819C0L)(lpRWLock, wFunction, line);

	unguard;
	return ret;
}

void ClanSys::LeaveExpelLock(LPVOID lpRWLock)
{
	guard;

	typedef void(*f)(LPVOID);
	f(0x781DE0L)(lpRWLock);

	LeaveCriticalSection(&g_ExpelBannedUsersLock);

	unguard;
}

void ClanSys::Initialize()
{
	InitializeCriticalSection(&g_ExpelBannedUsersLock);
	g_HookManager.WriteCall(0x9701ED, DeleteExpelLock);
	
	g_HookManager.WriteCall(0x408275, EnterExpelLock);
	g_HookManager.WriteCall(0x4088BE, LeaveExpelLock);

	g_HookManager.WriteCall(0x4A4CDD, EnterExpelLock);
	g_HookManager.WriteCall(0x4A52EB, LeaveExpelLock);

	g_HookManager.WriteCall(0x4A648B, EnterExpelLock);
	g_HookManager.WriteCall(0x4A66B0, LeaveExpelLock);

	g_HookManager.WriteCall(0x7E7FAA, EnterExpelLock);
	g_HookManager.WriteCall(0x7E85F9, LeaveExpelLock);

	g_HookManager.WriteCall(0x7EBE03, EnterExpelLock);
	g_HookManager.WriteCall(0x7EC62A, LeaveExpelLock);


	memset(g_PledgeLevelUpRequirements, 0, sizeof(g_PledgeLevelUpRequirements));

	const TCHAR* section = _T("ClanSystem");
	g_ReputationInPvPZone = GetPrivateProfileInt(section, _T("ReputationInPvPZone"), 0, g_ConfigFile);
	g_AttackWarWithoutCTRL = GetPrivateProfileInt(section, _T("AttackWarWithoutCTRL"), 0, g_ConfigFile);
	g_ClanLeaderTitleColor = GetPrivateProfileInt(section, _T("LeaderTitleColor"), 0, g_ConfigFile);
	g_ClanLeaderNicknameColor = GetPrivateProfileInt(section, _T("LeaderNicknameColor"), 0, g_ConfigFile);

	/*
	MainClan0Limit = 10
	MainClan1Limit = 15
	MainClan2Limit = 20
	MainClan3Limit = 30
	MainClan4Limit = 40
	AcademyLimit = 20
	RoyalGuardLimit = 20
	OrderOfKnightLimit = 10
	*/

	g_ClanMembersLimit.MainClan0 = GetPrivateProfileInt(section, _T("MainClan0Limit"), 10, g_ConfigFile);
	g_ClanMembersLimit.MainClan1 = GetPrivateProfileInt(section, _T("MainClan1Limit"), 15, g_ConfigFile);
	g_ClanMembersLimit.MainClan2 = GetPrivateProfileInt(section, _T("MainClan2Limit"), 20, g_ConfigFile);
	g_ClanMembersLimit.MainClan3 = GetPrivateProfileInt(section, _T("MainClan3Limit"), 30, g_ConfigFile);
	g_ClanMembersLimit.MainClan4 = GetPrivateProfileInt(section, _T("MainClan4Limit"), 40, g_ConfigFile);

	g_ClanMembersLimit.Academy = GetPrivateProfileInt(section, _T("AcademyLimit"), 20, g_ConfigFile);
	g_ClanMembersLimit.RoyalGuard = GetPrivateProfileInt(section, _T("RoyalGuardLimit"), 20, g_ConfigFile);
	g_ClanMembersLimit.OrderOfKnight = GetPrivateProfileInt(section, _T("OrderOfKnightLimit"), 10, g_ConfigFile);

	//;Pledge_x=item_id;count;sp_count;reputation_count;member_count
	for(int n=0;n<8;n++)
	{
		wstringstream key;
		key << L"Pledge_" << (n+1);
		TCHAR temp[8192];
		if(GetPrivateProfileString(section, key.str().c_str(), 0, temp, 8190, g_ConfigFile))
		{
			wstring data(temp);
			data = Utils::ReplaceString(temp, L";", L" ", true);
			wstringstream dataStream;
			dataStream << data;
			dataStream >> g_PledgeLevelUpRequirements[n][0] >> g_PledgeLevelUpRequirements[n][1] >> g_PledgeLevelUpRequirements[n][2] >> g_PledgeLevelUpRequirements[n][3] >> g_PledgeLevelUpRequirements[n][4];
		}

	}

	DWORD nSize = 140; // mov edx, 140
	g_HookManager.WriteMemoryDWORD(0x766350, nSize);
	//.text:000000000076638E cmp     rcx, 28h
	g_HookManager.WriteJump( 0x76638E, AddMemberFix, 1); //asm
	g_HookManager.WriteJump( 0x76859D, DelMemberFix, 1); //asm
	//Decrease Max Pledge in ally to 3
	g_HookManager.WriteMemoryBYTE(0x88CDE9, g_Config.ClanInfo.GetAllySize());
	//Extending MemberData
	g_HookManager.WriteCall( 0x765CF4, CreateMemberData, 0);
	//Hooking destructor - removing pPledge from container
	g_HookManager.WriteMemoryDWORD(0xA53460, (DWORD)RemovePledge);
	//Realloacting new ext info
	g_HookManager.WriteCall( 0x5915F9, HandleExtPledgeInfo, 0);
	//Loading new ext info and adding pPledge to container	
	g_HookManager.WriteCall( 0x591B94, HandleLoadedPledge, 0);
	//Reallocating new ext member info
	g_HookManager.WriteCall( 0x5916D9, HandleExtMemberInfo, 0);
	g_HookManager.WriteCall( 0x591C39, HandleExtMemberInfo, 0);
	//Loading new ext member info
	g_HookManager.WriteCall( 0x5919E7, HandleLoadedMember, 0);
	g_HookManager.WriteCall( 0x591C81, HandleLoadedMember, 0);
	//Adding fresh created pledge to container + creating ext pledge info
	g_HookManager.WriteCall( 0x590A09, HandleCreatedPledge, 0);
	g_HookManager.WriteCall( 0x59117A, HandleCreatedPledge, 0);
	//Updating already logged in members privilege / clan skills
	g_HookManager.WriteCall( 0x59203B, UpdateMembers, 0);
	//Packets
	g_HookManager.WriteCall( 0x590BA6, CPledge_OpenAllWindowPacket, 0);
	g_HookManager.WriteCall( 0x591349, CPledge_OpenAllWindowPacket, 0);
	g_HookManager.WriteCall( 0x8809DE, CPledge_OpenAllWindowPacket, 0);
	g_HookManager.WriteCall( 0x766C8D, CPledge_OpenAllWindowPacket, 0);
	g_HookManager.WriteCall( 0x769DFA, CPledge_OpenAllWindowPacket, 0);
	g_HookManager.WriteCall( 0x84D5D2, CPledge_OpenAllWindowPacket, 0);
	g_HookManager.WriteCall( 0x86F641, CPledge_OpenAllWindowPacket, 0);
	g_HookManager.WriteCall( 0x5158E6, CPledge_UpdateWindowPacket, 0);
	g_HookManager.WriteCall( 0x515FE5, CPledge_UpdateWindowPacket, 0);
	g_HookManager.WriteCall( 0x5196E1, CPledge_UpdateWindowPacket, 0);
	g_HookManager.WriteCall( 0x84D580, CPledge_UpdateWindowPacket, 0);
	g_HookManager.WriteCall( 0x8502FB, CPledge_UpdateWindowPacket, 0);
	g_HookManager.WriteCall( 0x8554D8, CPledge_UpdateWindowPacket, 0);
	g_HookManager.WriteCall( 0x769DE9, CPledge_AddWindowPacket, 0);
	g_HookManager.WriteCall( 0x55694A, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x5591E9, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x58D277, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x593110, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x487BC7, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x487D21, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x4882DE, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x4883EE, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x58C648, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x592AA9, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x433502, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x405B03, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x405A2B, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x4A8118, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x4A7F58, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x768289, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x6DC8C4, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x48201A, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x595B63, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x5A64BC, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x5A6B49, CPledge_UpdateInfoWindowPacket, 0);
	g_HookManager.WriteCall( 0x769251, CPledge_UpdateInfoWindowPacket, 0);
	
	//Increase pledge level
	g_HookManager.WriteCall( 0x898709, PledgeLevelUpByNPC, 0);
	//Create Pledge - Pledge Leader at main clan list
	g_HookManager.WriteCall( 0x5911A1, ReplyCreatePledge, 0);

	g_HookManager.WriteCall( 0x5560F2, CPledge_Join, 0);
	//Leave pledge
	g_HookManager.WriteCall( 0x5560CC, CPledge_Withdraw, 0);
	g_HookManager.WriteCall( 0x554514, CPledge_Oust, 0);
	//Reputation Points
	g_HookManager.WriteCall( 0x4AE54F, CCastle_OnChangeOwner, 0);
	g_HookManager.WriteMemoryDWORD(0x976FE8, (DWORD)CAgit_ChangeOwner);
	g_HookManager.WriteMemoryDWORD(0x987658, (DWORD)CAgit_ChangeOwner);
	g_HookManager.WriteMemoryDWORD(0xA2B1D8, (DWORD)CAgit_ChangeOwner);
	g_HookManager.WriteMemoryDWORD(0xA5E078, (DWORD)CAgit_ChangeOwner);
	g_HookManager.WriteMemoryDWORD(0xA77AA8, (DWORD)CAgit_ChangeOwner);
	
	g_HookManager.WriteMemoryDWORD(0x9A4110, (DWORD)CCastle_StartSiege);
	g_HookManager.WriteCall( 0x765A38, CPledge_DeleteDBMember, 0);
	//Academy member war fix
	g_HookManager.WriteCall( 0x774753, GetRelationTo_GetPledge, 0);
	g_HookManager.WriteCall( 0x77475E, GetRelationTo_GetPledge, 0);
	//Ai reputation add-on
	g_HookManager.WriteMemoryDWORD(0x97FD50, (DWORD)AtomicPledgeLevelUp_Do);

	g_HookManager.WriteCall(0x76A35D, CPledge_TakeOustPenalty);
//	g_HookManager.WriteCall(0x76A25B, User_TakeOustedPenalty);
//	g_HookManager.WriteCall(0x76A330, CDB_RequestSeUserPledgeInfo);

	LoadReputationData();
	g_PledgeSkillDb.Init();
}

void ClanSys::LoadReputationData()
{
	mNpcReputation.clear();
	ifstream file("..\\Script\\NpcReputationData.txt");
	if(file)
	{
		string sLine;
		int nCastle = 0;
		int nAgit = 0;
		while(getline(file, sLine))
		{
			if(sLine.find("//") == 0)
				continue;

			if(sLine.find("npc_begin") == 0)
			{
				int npcId = CParser::GetValueInt(sLine, "npc_class_id");
				int reputation = CParser::GetValueInt(sLine, "reputation_points");
				if(npcId > 0 && reputation > 0)
				{
					mNpcReputation[npcId] = reputation;
				}
			}
		}
	}

	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] npcs.", __FUNCTION__, mNpcReputation.size());
}

void ClanSys::OnNpcDie(CNPC *pNpc, CCreature *pEnemy)
{
	if(pNpc->ValidCreature() && pEnemy->ValidCreature())
	{
		map<int, int>::iterator Iter = mNpcReputation.find(pNpc->pSD->nNpcClassID);
		if(Iter!=mNpcReputation.end())
		{
			User *pUser = NULL;
			if(pEnemy->ValidUser())
			{
				pUser = pEnemy->GetUser();
			}else if(pEnemy->IsSummon())
			{
				CSummon *pSummon = pEnemy->CastSummon();
				pUser = pSummon->GetMaster();
			}

			if(pUser->ValidUser())
			{
				if(CPledge *pPledge = pUser->GetPledge())
				{
					if(pPledge->pSD->nLevel > 4)
					{
						pPledge->UpdatePledgeNameValue(Iter->second);
						pPledge->NotifyReputationIncreased(Iter->second);
					}
				}
			}
		}
	}
}

LPVOID ClanSys::CreateMemberData(int nSize)
{
	return L2Server::New(sizeof(MemberData));
}

const unsigned char* ClanSys::HandleExtPledgeInfo(const unsigned char* packet, const char *format, int nSize1, PWCHAR wszPledgeName, int *pPledgeID, int *pMasterID, int nSize2, ExtPledgeInfo **Info, int *pMemberAmount)
{
	ExtPledgeInfo *pInfo = (ExtPledgeInfo*)L2Server::New(sizeof(ExtPledgeInfo));
	memset(pInfo, 0, sizeof(ExtPledgeInfo));
	*Info = pInfo;
	pInfo->m_Skills = new map<int, int>;
	pInfo->m_SubUnits = new SubUnitMap;
	int nSkillAmount = 0;
	packet = Disassemble(packet, "SddSdddddddddddd", nSize1, wszPledgeName, pPledgeID, pMasterID, nSize2, pInfo->wszMasterName, pMemberAmount,
		&pInfo->nReputation, &pInfo->PowerData[1], &pInfo->PowerData[2], &pInfo->PowerData[3], &pInfo->PowerData[4], &pInfo->PowerData[5], 
		&pInfo->PowerData[6], &pInfo->PowerData[7], &pInfo->PowerData[8], &pInfo->PowerData[9], &nSkillAmount);
	//g_Log.Add(CLog::Blue, "[Loading Pledge] PledgeID[%d] Name[%S]", (*pPledgeID), wszPledgeName);
	if(nSkillAmount > 0)
	{
		for(int n=0; n < nSkillAmount; n++)
		{
			int nSkillID, nLevel;
			packet = Disassemble(packet, "dd", &nSkillID, &nLevel);
			if(nLevel > 0 && nLevel < 4)
				pInfo->m_Skills->insert(pair<int, int>(nSkillID, nLevel));
		}
	}
	int nSubUnitAmount = 0;
	packet = Disassemble(packet, "d", &nSubUnitAmount);
	if(nSubUnitAmount > 0)
	{
		for(int n=0;n<nSubUnitAmount;n++)
		{
			int nPledgeType;
			WCHAR wszName[25];
			WCHAR wszLeaderName[25];
			int nLeaderDBID;
			packet = Disassemble(packet, "dSSd", &nPledgeType, sizeof(wszName), wszName, sizeof(wszLeaderName), wszLeaderName, &nLeaderDBID);
			CSubUnit Unit;
			Unit.nID = nPledgeType;
			Unit.Name = wszName;
			Unit.LeaderName = wszLeaderName;
			Unit.nLeaderID = nLeaderDBID;
			pInfo->m_SubUnits->insert(pair<int, CSubUnit>(nPledgeType, Unit));
		}
	}
	return packet;
}
CPledge *ClanSys::HandleLoadedPledge(CPledge *pPledge, INT32 nPledgeID, INT32 nMasterID, PWCHAR wszPledgeName, ExtPledgeInfo** Info)
{
	ExtPledgeInfo *pInfo = *Info;
	CPledge *pNewPledge = pPledge->Constructor(nPledgeID, nMasterID, wszPledgeName, pInfo->wszMasterName);
	g_PledgeDB.AddPledge(pPledge);
	int nState = 0;
	try
	{
		pPledge->pSD->pPledgeEx = new CPledgeEx(nPledgeID);
		pPledge->pSD->nReputation = pInfo->nReputation;
		nState = 1;
		for(int n=1;n<10;n++)
			pPledge->pSD->PowerData[n] = pInfo->PowerData[n];
		nState = 2;
		//fill data for clan skills
		/*
		for(map<int, int>::iterator it = pInfo->m_Skills->begin(); it!=pInfo->m_Skills->end();it++)
		{
			const PledgeSkillInfo& psi = g_PledgeSkillDb.GetSkillInfo(it->first, it->second);
			if(psi.skillId > 0)
			{
				pPledge->pSD->pPledgeEx->m_Skills[it->first] = psi;
			}
		}
		*/

		pPledge->pSD->pPledgeEx->mSubUnits = (*pInfo->m_SubUnits);
		nState = 3;
		delete pInfo->m_Skills;
		delete pInfo->m_SubUnits;
		nState = 4;
		//Fill Sub Units
		SubUnitMap::iterator Iter = pPledge->pSD->pPledgeEx->mSubUnits.begin();
		nState = 5;
		for(;Iter!=pPledge->pSD->pPledgeEx->mSubUnits.end();Iter++)
		{
			switch(Iter->first)
			{
			case CPledge::ACADEMY:
				{
					pPledge->pSD->nHasAcademy = 1;
					break;
				}
			case CPledge::ROYAL_GUARD_I:
				{
					pPledge->pSD->nHasSubUnit100 = 1;
					wcscpy_s(pPledge->pSD->wsSubMaster100, 25, Iter->second.LeaderName.c_str());
					break;
				}
			case CPledge::ROYAL_GUARD_II:
				{
					pPledge->pSD->nHasSubUnit200 = 1;
					wcscpy_s(pPledge->pSD->wsSubMaster200, 25, Iter->second.LeaderName.c_str());
					break;
				}
			case CPledge::ORDER_OF_KNIGHTS_I:
				{
					pPledge->pSD->nHasSubUnit1001 = 1;
					wcscpy_s(pPledge->pSD->wsSubMaster1001, 25, Iter->second.LeaderName.c_str());
					break;
				}
			case CPledge::ORDER_OF_KNIGHTS_II:
				{
					pPledge->pSD->nHasSubUnit1002 = 1;
					wcscpy_s(pPledge->pSD->wsSubMaster1002, 25, Iter->second.LeaderName.c_str());
					break;
				}
			case CPledge::ORDER_OF_KNIGHTS_III:
				{
					pPledge->pSD->nHasSubUnit2001 = 1;
					wcscpy_s(pPledge->pSD->wsSubMaster2001, 25, Iter->second.LeaderName.c_str());
					break;
				}
			case CPledge::ORDER_OF_KNIGHTS_IV:
				{
					pPledge->pSD->nHasSubUnit2002 = 1;
					wcscpy_s(pPledge->pSD->wsSubMaster2002, 25, Iter->second.LeaderName.c_str());
					break;
				}
			default:
				{
					break;
				}
			}
		}
		nState = 6;

		g_DB.RequestLoadPledgeSkills(nPledgeID);
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected - pledgeID[%d] State[%d]", __FUNCTION__, nPledgeID, nState);
	}
	L2Server::Delete(pInfo);
	return pNewPledge;
}

const unsigned char* ClanSys::HandleExtMemberInfo(const unsigned char* packet, const char *format, int *pDBID, int nSize, ExtMemberInfo **Info, int *pLevel, int *pClass, int *pGender, int *pRace)
{
	ExtMemberInfo *pInfo = (ExtMemberInfo*)L2Server::New(sizeof(ExtMemberInfo));
	memset(pInfo, 0, sizeof(ExtMemberInfo));
	*Info = pInfo;
	return Disassemble(packet, "dSdddddddd", pDBID, sizeof(pInfo->wszMemberName), pInfo->wszMemberName, pLevel, pClass, pGender, pRace, &pInfo->nPledgeType, &pInfo->nRank, &pInfo->nSponsorID, &pInfo->nJoinLevel);
}

void ClanSys::HandleLoadedMember(CPledge *pPledge, int nDBID, ExtMemberInfo** Info, int nLevel , int nClass, int nGander, int nRace, int nObjectID)
{
	ExtMemberInfo *pInfo = *Info;
	if(pPledge && pInfo)
	{
		pPledge->AddDBMemberByDBID(nDBID, pInfo->wszMemberName, nLevel, nClass, nGander, nRace, nObjectID);
		pPledge->PledgeLock->Lock(__FILEW__, __LINE__);
		pPledge->pSD->nTotalCount += 1;
		try
		{
			MemberData *pMember = pPledge->m_Members[nDBID];
			if(pMember)
			{
				pMember->nPledgeType = pInfo->nPledgeType;
				pMember->nRank = pInfo->nRank;
				pMember->nSponsorID = pInfo->nSponsorID;
				pMember->nJoinLevel = pInfo->nJoinLevel;
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception detected - CharID[%d]", __FUNCTION__, nDBID);
		}
		pPledge->PledgeLock->Unlock();
		L2Server::Delete(pInfo);
	}
}
LPVOID ClanSys::HandleCreatedPledge(CPledge *pPledge, INT32 nPledgeID, INT32 nMasterID, PWCHAR wszPledgeName, PWCHAR wszMasterName)
{
	CPledge *pNewPledge = pPledge->Constructor(nPledgeID, nMasterID, wszPledgeName, wszMasterName);
	g_DB.RequestCreateExtPledgeInfo(nPledgeID);
	pPledge->pSD->pPledgeEx = new CPledgeEx(nPledgeID);
	g_PledgeDB.AddPledge(pNewPledge);
	g_DB.RequestLoadPledgeSkills(nPledgeID);
	return pNewPledge;
}

void ClanSys::UpdateMembers(CPledge *pPledge)
{
	pPledge->UpdateMembers();
	pPledge->ValidateTotalLevel();
}
LPVOID ClanSys::RemovePledge(CPledge *pPledge, DWORD dData)
{
	g_PledgeDB.DelPledge(pPledge);
	return pPledge->Destructor(dData);
}
INT64 ClanSys::C5ToC4Privilege(INT64 nPrivilege)
{
	bool bC5Rights[23];
	bool bC4Rights[16];
	UINT nPotega = 1;
	for(UINT i=0; i< 23; i++)
		bC5Rights[i]= false;
	for(UINT i=0; i<16; i++)
		bC4Rights[i]= false;
		
	for(UINT i=22; i>0; i--)
	{
		nPotega = 1 << i;
		if(nPrivilege>=nPotega)
		{
			bC5Rights[i]= true;
			nPrivilege-=nPotega;
		}
	}
	if(bC5Rights[1]) bC4Rights[0]= true;
	if(bC5Rights[2]) bC4Rights[1]= true;
	if(bC5Rights[3]) bC4Rights[2]= true;
	if(bC5Rights[7]) bC4Rights[3]= true;	//edit crest
	if(bC5Rights[10]) bC4Rights[4]= true;
	if(bC5Rights[14]) bC4Rights[5]= true;
	if(bC5Rights[13]) bC4Rights[6]= true;
	if(bC5Rights[15]) bC4Rights[7]= true;
	if(bC5Rights[22]
	|| bC5Rights[16]
	|| bC5Rights[17]
	|| bC5Rights[21]
	|| bC5Rights[20]) bC4Rights[8]= true;
	if(bC5Rights[19]) bC4Rights[9]= true;
	if(bC5Rights[5]) bC4Rights[10]= true;
	if(bC5Rights[4]) bC4Rights[11]= true; //Manage Ranks
	if(bC5Rights[6]) bC4Rights[12]= true; //Dismiss
	if(bC5Rights[8]) bC4Rights[13]= true; //Master Rights
	if(bC5Rights[9]) bC4Rights[14]= true; //Manage Levels
	
	nPotega=1;
	nPrivilege = 0;
	for(UINT i=0; i< 16; i++)
	{
		nPotega = 1 << i;
		if(bC4Rights[i]) nPrivilege+=nPotega;
	}
	return nPrivilege;
}
INT64 ClanSys::C4ToC5Privilege(DWORD dPrivilege)
{
	bool bC4Rights[16];
	bool bC5Rights[23];
	DWORD nPotega = 0;
	for(UINT n=0;n<23;n++)
		bC5Rights[n]=false;
	for(UINT n=0;n<16;n++)
		bC4Rights[n]=false;
	
	//encode c4 rights
	for(int n=16;n>=0;n--)
	{
		nPotega = 1 << n;
		if(dPrivilege >= nPotega)
		{
			bC4Rights[n] = true;
			dPrivilege -= nPotega;
		}
	}
	if(bC4Rights[0]) bC5Rights[1] = true;
	if(bC4Rights[1]) bC5Rights[2] = true;
	if(bC4Rights[2]) bC5Rights[3] = true;
	if(bC4Rights[3]) bC5Rights[7] = true;
	if(bC4Rights[4]) bC5Rights[10] = true;
	if(bC4Rights[5]) bC5Rights[14] = true;
	if(bC4Rights[6]) bC5Rights[13] = true;
	if(bC4Rights[7]) bC5Rights[15] = true;
	if(bC4Rights[8])
	{
		bC5Rights[22] = true;
		bC5Rights[21] = true;
		bC5Rights[20] = true;
		bC5Rights[17] = true;
		bC5Rights[16] = true;
	}
	if(bC4Rights[9]) bC5Rights[19] = true;
	if(bC4Rights[10]) bC5Rights[5] = true;
	if(bC4Rights[11]) bC5Rights[4] = true; //Manage Ranks
	if(bC4Rights[12]) bC5Rights[6] = true; //Dismiss
	if(bC4Rights[13]) bC5Rights[8] = true; //Master Rights
	if(bC4Rights[14]) bC5Rights[9] = true; //Manage Levels

	dPrivilege=0;
	//recode c5 rights
	for(UINT n=0; n<23; n++)
	{
		if(bC5Rights[n])
			dPrivilege += (1 << n);
	}
	return dPrivilege;
}
bool ClanSys::CheckPrivilege(INT64 nPledgePower, int nPrivilege)
{
	nPrivilege = 1 << nPrivilege;
	if( (nPledgePower&nPrivilege) == nPrivilege)
		return true;
	else 
		return false;
}
bool ClanSys::ManagePledgePower(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	if(pSocket->ValidUserSocket())
	{
		int nAction, nRank;
		packet = Disassemble(packet, "dd", &nRank, &nAction);
		User *pUser = pSocket->GetUser();
		if(pUser->ValidUser() && pUser->pSD->nIsPledgeLeader)
		{
			switch(nAction)
			{
			case 1:
				{
					//View
					CPledge *pPledge = pUser->GetPledge();
					if(pPledge && pPledge->pSD)
					{
						if(nRank > 0 && nRank < 10)
							pSocket->Send("cddd", 0x30, nRank, 1, pPledge->pSD->PowerData[nRank]);
					}
					break;
				};
			case 2:
				{
					//Edit
					CPledge *pPledge = pUser->GetPledge();
					if(pPledge && pPledge->pSD && nRank > 0 && nRank < 10)
					{
						UINT nPrivilege;
						Disassemble(packet,"d", &nPrivilege);
						g_DB.RequestSetPledgePrivilege(pPledge->pSD->nDBID, nRank, nPrivilege);
						pPledge->pSD->PowerData[nRank] = nPrivilege;
						pPledge->UpdateMembersPrivilege(nRank);
						pSocket->Send("cddd", 0x30, nRank, nAction, nPrivilege);
					}
					break;
				}
			default:
				{
					g_Log.Add(CLog::Error, "[%s][%d] Invalid nAction[%d] User[%ls]", __FILE__, __LINE__, nAction, pUser->pSD->wszName);
					return true;
				}
			}
		}else
			return false;
	}else
		return true;
	return false;
}
bool ClanSys::RequestMemberPowerInfo(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		WCHAR wszName[25];
		int nUnkn;
		Disassemble(packet, "dS", &nUnkn, sizeof(wszName), wszName);
		CPledge *pPledge = pUser->GetPledge();
		if(pPledge)
		{
			MemberData *pData = pPledge->GetMemberDataByName(wszName);
			if(pData)
			{
				int nRank = pData->nRank;
				if(nRank < 10 && nRank > 0)
				{
					INT64 nPower = pPledge->pSD->PowerData[nRank];
					pSocket->Send("chdSd", 0xFE, 0x3C, nRank, pData->wszName, nPower); 
				}else
					g_Log.Add(CLog::Error, "[%s] Invalid rank[%d] user[%S]", __FUNCTION__, nRank, pData->wszName);
			}
		}else
			return true;
	}else
		return true;
	return false;
}

bool ClanSys::RequestSetMemberRank(CUserSocket *pSocket, const unsigned char* packet)
{
	if(User *pUser = pSocket->pUser->SafeCastUser())
	{
		WCHAR wszName[25];
		wszName[0] = 0;
		int nRank;
		packet = Disassemble(packet, "Sd", sizeof(wszName), wszName, &nRank);
		
		if(!Parser::IsValidSqlString(wszName))
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] used invalid name[%S]!", __FUNCTION__, pUser->pSD->wszName, wszName);
			return false;
		}
		
		CPledge *pPledge = pUser->GetPledge();
		if(pPledge)
		{
			if(nRank > 0 && nRank < 10)
			{
				if(pUser->pSD->nIsPledgeLeader || CheckPrivilege(pUser->pSD->n64_PledgePower1, MANAGE_RANKS))
				{
					User *pMember = pPledge->GetMember(wszName);
					if(pMember->ValidUser())
					{
						pMember->pSD->n64_PledgePower1 = C5ToC4Privilege(pPledge->pSD->PowerData[nRank]);
//						pMember->pSD->pledgePower = pPledge->pSD->PowerData[nRank];
						g_DB.RequestSavePledgePower(pMember);
						pPledge->NotifyMemberGradeChanged(wszName, nRank);
						g_DB.RequestSetPledgeMemberRank(pPledge->pSD->nDBID, wszName, nRank);
					}else
						g_DB.RequestSetPledgeMemberRank(pPledge->pSD->nDBID, wszName, nRank);
					MemberData *pData = pPledge->GetMemberDataByName(wszName);
					if(pData)
					{
						pData->nRank = nRank;
					}
				}
			}
		}
		return false;
	}
	return true;
}

bool ClanSys::RequestMemberInfo(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		UINT nUnkn;
		WCHAR wszName[25];
		Disassemble(packet, "dS", &nUnkn, sizeof(wszName), wszName);
		if(!wcscmp(wszName, L"No Captain exists."))
		{
			return false;
		}else
		{
			CPledge *pPledge = pUser->GetPledge();
			if(pPledge)
			{
				MemberData *pMember = pPledge->GetMemberDataByName(wszName);
				if(pMember)
				{
					CPacket packet;
					packet.WriteC(0xFE);
					packet.WriteH(0x3D);
					packet.WriteD(pMember->nPledgeType);
					packet.WriteS(wszName);
					if(pMember->nObjectID)
					{
						User *pUM = User::GetUserBySID(&pMember->nObjectID);
						if(pUM)
						{
							packet.WriteS(pUM->GetTitle());
						}
						else
						{
							return false;
						}
					}
					else
						packet.WriteS(L"Offline");
					packet.WriteD(pMember->nRank);
					packet.WriteS(pPledge->GetName());
					if(pMember->nPledgeType == CPledge::ACADEMY)
					{
						if(pMember->nSponsorID)
						{
							MemberData *pSponsor = pPledge->GetMemberDataByDBID(pMember->nSponsorID);
							if(pSponsor)
								packet.WriteS(pSponsor->wszName);
							else
								packet.WriteS(L"");
						}
					}else
					{
						MemberData *pApprentice = pPledge->GetApprenticeData(pMember->nDBID);
						if(pApprentice)
							packet.WriteS(pApprentice->wszName);
						else
							packet.WriteS(L"");
					}
					pSocket->Send("b", packet.GetSize(), packet.GetBuff());
				}
			}
		}
	}
	return false;
}
bool ClanSys::RequestReorganizeMember(CUserSocket *pSocket, const unsigned char* packet)
{
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser() && pUser->pSD->nIsPledgeLeader)
	{
		int nSwitch, nNewPledgeType;
		WCHAR wszName[25];
		WCHAR wszName2[25];
		Disassemble(packet, "dSdS", &nSwitch, sizeof(wszName), wszName, &nNewPledgeType, sizeof(wszName2), wszName2);

		if(!Parser::IsValidSqlString(wszName))
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] used invalid name[%S]!", __FUNCTION__, pUser->pSD->wszName, wszName);
			return false;
		}

		if(!Parser::IsValidSqlString(wszName2))
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] used invalid name2[%S]!", __FUNCTION__, pUser->pSD->wszName, wszName2);
			return false;
		}
		CPledge *pPledge = pUser->GetPledge();

		if(nNewPledgeType == CPledge::ACADEMY)
		{
			pUser->pSocket->SendSystemMessage(L"You cannot transfer any member to Academy.");
			return false;
		}

		if(pPledge && pPledge->pSD && pPledge->pSD->nLevel > 4 && pPledge->HasSubUnit(nNewPledgeType))
		{
			switch(nSwitch)
			{
				case 1:	//Switch members
					{
						MemberData *pMember1 = pPledge->GetMemberDataByName(wszName);
						MemberData *pMember2 = pPledge->GetMemberDataByName(wszName2);
						if(pMember1 && pMember2)
						{
							if(pMember1->nPledgeType == CPledge::ACADEMY || pMember2->nPledgeType == CPledge::ACADEMY)
							{
								pUser->pSocket->SendSystemMessage(L"You cannot transfer any member to Academy.");
								return false;
							}

							int nOldPledgeType = pMember1->nPledgeType;
							int nRank = pPledge->GetDefaultRank(nNewPledgeType);
							int nRank2 = pPledge->GetDefaultRank(nOldPledgeType);
							pMember1->nPledgeType = nNewPledgeType;
							pMember2->nPledgeType = nOldPledgeType;
							pMember1->nRank = nRank;
							pMember2->nRank = nRank2;
							
							g_DB.RequestSetPledgeMemberRank(pPledge->pSD->nDBID, wszName, nRank);
							g_DB.RequestReorganizeMember(wszName, nNewPledgeType, pPledge->pSD->nDBID);
							
							g_DB.RequestSetPledgeMemberRank(pPledge->pSD->nDBID, wszName2, nRank2);
							g_DB.RequestReorganizeMember(wszName2, nOldPledgeType, pPledge->pSD->nDBID);
							pPledge->UpdateMemberList(pMember1);
							pPledge->UpdateMemberList(pMember2);
							pUser->SendSystemMessage(L"Transfering member to a sub unit has been succesfull.");
						}else
							pUser->SendSystemMessage(L"Member cannot be found.");
						break;
					}
				default:	//Move one member
					{
						MemberData *pMember = pPledge->GetMemberDataByName(wszName);
						if(pMember)
						{
							if(pPledge->CanJoin(nNewPledgeType))
							{
								int nRank = pPledge->GetDefaultRank(nNewPledgeType);
								pMember->nRank = nRank;
								pMember->nPledgeType = nNewPledgeType;
								g_DB.RequestSetPledgeMemberRank(pPledge->pSD->nDBID, wszName, nRank);
								g_DB.RequestReorganizeMember(wszName, nNewPledgeType, pPledge->pSD->nDBID);
								pPledge->UpdateMemberList(pMember);
								pUser->SendSystemMessage(L"Transfering member to a sub unit has been succesfull.");
							}else
								pUser->SendSystemMessage(L"That subunit is full.");
						}else
							pUser->SendSystemMessage(L"Member cannot be found.");
						break;
					}
			}
		}else
			pUser->SendSystemMessage(L"Your clan doesn't meet requiements.");
	}
	return false;
}
bool ClanSys::RequestCancelDelegateClanMaster(User *pUser)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	if(pUser->ValidUser())
	{
		if(pUser->pSD->nIsPledgeLeader)
		{
			CPledge *pPledge = pUser->GetPledge();
			if(pPledge && pPledge->pSD)
			{
				if(pPledge->pSD->nDelegateCLID)
				{
					pPledge->pSD->nDelegateCLID = 0;
					g_DB.RequestDelegateClanMaster(pPledge->pSD->nDBID, 0);
					const WCHAR* pFile = 0;
					if(g_HtmlCache.IsEnabled())
					{
						pFile = g_HtmlCache.Get(L"pl_cancel_success.htm");
					}else
					{
						pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_cancel_success.htm", pUser->pSD->bLang);
					}
					if(pFile)
					{
						PlayerAction::ShowHTML(pUser, L"pl_cancel_success.htm", pFile, 0);
					}
				}
			}
		}else
			pUser->SendSystemMessage(L"You're not allowed to do that.");
	}
	return false;
}

bool ClanSys::RequestDelegateClanMaster(User *pUser, wstring wMasterName)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	if(pUser->ValidUser())
	{
		if(pUser->pSD->nIsPledgeLeader)
		{
			CPledge *pPledge = pUser->GetPledge();
			if(pPledge && pPledge->pSD)
			{
				if(pPledge->pSD->nDelegateCLID)
				{
					//already delegated
					//Show html
					const WCHAR* pFile = 0;
					if(g_HtmlCache.IsEnabled())
					{
						pFile = g_HtmlCache.Get(L"pl_already_transfer.htm");
					}else
					{
						pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_already_transfer.htm", pUser->pSD->bLang);
					}
					if(pFile)
					{
						PlayerAction::ShowHTML(pUser, L"pl_already_transfer.htm", pFile, 0);
					}
				}else
				{
					MemberData *pMD = pPledge->GetMemberDataByName((PWCHAR)wMasterName.c_str());
					if(pMD)
					{
						try
						{
							if(pMD->nPledgeType == CPledge::MAIN_CLAN)
							{
								//Delegate
								pPledge->pSD->nDelegateCLID = pMD->nDBID;
								g_DB.RequestDelegateClanMaster(pPledge->pSD->nDBID, pMD->nDBID);
								//Show html
								const WCHAR* pFile = 0;
								if(g_HtmlCache.IsEnabled())
								{
									pFile = g_HtmlCache.Get(L"pl_transfer_success.htm");
								}else
								{
									pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_transfer_success.htm", pUser->pSD->bLang);
								}
								if(pFile)
								{
									PlayerAction::ShowHTML(pUser, L"pl_transfer_success.htm", pFile, 0);
								}
							}else
							{
								pUser->SendSystemMessage(L"Selected member cannot be found in main clan.");
							}
						}catch(...)
						{
							g_Log.Add(CLog::Error, "[%s] Exception Detected!",__FUNCTION__);
						}
					}else
					{
						pUser->SendSystemMessage(L"Selected member cannot be found in main clan.");
					}
				}
			}
		}else
		{
			pUser->SendSystemMessage(L"You're not allowed to do that.");
		}
	}
	return false;
}

bool ClanSys::RequestDelegateClanMasterEx(User *pUser, wstring wMasterName)
{
	guard;
	if(pUser->ValidUser())
	{
		if(pUser->pSD->nIsPledgeLeader)
		{
			if(CPledge *pPledge = pUser->GetPledge())
			{

				if(CCastle::IsAnyCastleDuringSiege())
				{
					pUser->SendSystemMessage(L"You cannot delegate clan leader during the castle siege.");
					unguard;
					return false;
				}
				if(pPledge->pSD->nDelegateCLID)
				{
					//already delegated
					//Show html
					const WCHAR* pFile = 0;
					if(g_HtmlCache.IsEnabled())
					{
						pFile = g_HtmlCache.Get(L"pl_already_transfer.htm");
					}else
					{
						pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_already_transfer.htm", pUser->pSD->bLang);
					}
					if(pFile)
					{
						PlayerAction::ShowHTML(pUser, L"pl_already_transfer.htm", pFile, 0);
					}
				}else
				{
					MemberData *pMD = pPledge->GetMemberDataByName((PWCHAR)wMasterName.c_str());
					if(pMD)
					{
						try
						{
							if(pMD->nPledgeType == CPledge::MAIN_CLAN)
							{
								//Delegate
								if(User *pMember = g_UserDB.GetUserByDBID(pMD->nDBID))
								{
									pUser->pSD->nIsPledgeLeader = 0;
									pUser->pSD->n64_PledgePower1 = 0;
									pUser->pSD->n64_PledgePower2 = 0;
									pUser->pSD->n64_PledgePower3 = 0;
									pUser->pSD->n64_PledgePower4 = 0;
									g_DB.RequestSavePledgePower(pUser);
									pUser->SetUserInfoChanged();
									wcscpy(pPledge->pSD->wszClanLeaderName, pMember->pSD->wszName);
									pPledge->pSD->nLeaderDBID = pMember->nDBID;
									pPledge->SetMaster(pMember);
									typedef void (*_setPledge)(User*, CPledge*);
									_setPledge(0x810C50L)(pMember, pPledge);
									
									pMember->SetUserInfoChanged();

									pUser->DeleteSkill(246);
									pUser->SendDeleteSkillToDB(246);
									pUser->DeleteSkill(247);
									pUser->SendDeleteSkillToDB(247);
									pUser->ValidateParameters();

									pPledge->Save(18);
									pPledge->UpdateInfo();

									//Show html
									const WCHAR* pFile = 0;
									if(g_HtmlCache.IsEnabled())
									{
										pFile = g_HtmlCache.Get(L"pl_transfer_success.htm");
									}else
									{
										pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_transfer_success.htm", pUser->pSD->bLang);
									}
									if(pFile)
									{
										PlayerAction::ShowHTML(pUser, L"pl_transfer_success.htm", pFile, 0);
									}
								}else
								{
									pUser->SendSystemMessage(L"Selected member is offline!");
								}
							}else
							{
								pUser->SendSystemMessage(L"Selected member cannot be found in main clan.");
							}
						}catch(...)
						{
							g_Log.Add(CLog::Error, "[%s] Exception Detected!",__FUNCTION__);
						}
					}else
					{
						pUser->SendSystemMessage(L"Selected member cannot be found in main clan.");
					}
				}
			}
		}else
		{
			pUser->SendSystemMessage(L"You're not allowed to do that.");
		}
	}
	unguard;
	return false;
}

bool ClanSys::RequestWarInfo(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		int Type, Type2;
		Disassemble(packet, "dd", &Type, &Type2);
		CPledge *pPledge = pUser->GetPledge();
		if(pPledge)
		{
			if((Type == -1)||(Type2==1)) //attacker list
				pPledge->SendAttackerList(pUser);
			else if((Type == 0)&&(Type2 == 0)) //enemy list
				pPledge->SendEnemyList(pUser);
		}
	}
	return false;
}

bool ClanSys::RequestPledgePowerGradeList(CUserSocket *pSocket, const unsigned char* packet)
{
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		CPledge *pPledge = pUser->GetPledge();
		if(pPledge)
		{
			UINT rankCount[10];
			memset(rankCount, 0, sizeof(rankCount));

			for(MemberDataMap::iterator Iter = pPledge->m_Members.begin();Iter!=pPledge->m_Members.end();Iter++)
			{
				MemberData* pData = Iter->second;
				if(pData)
				{
					if(pData->nRank < 10)
					{
						rankCount[pData->nRank]++;
					}
				}
			}

			pSocket->Send("chddddddddddddddddddd",	0xFE, 0x3B, 9,
						1, rankCount[1],
						2, rankCount[2],
						3, rankCount[3], 
						4, rankCount[4],
						5, rankCount[5],
						6, rankCount[6],
						7, rankCount[7],
						8, rankCount[8],
						9, rankCount[9]);
		}
	}
	return false;
}

bool ClanSys::RequestSetSponsor(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		if(CheckPrivilege(pUser->pSD->n64_PledgePower1, MASTER_RIGHTS) || pUser->pSD->nIsPledgeLeader)
		{
			int nOption;
			WCHAR wsCurrent[25];
			WCHAR wsTarget[25];
			Disassemble(packet, "dSS", &nOption, sizeof(wsCurrent), wsCurrent, sizeof(wsTarget), wsTarget);
			if(!Parser::IsValidSqlString(wsCurrent))
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] used invalid sponsor name[%S]", __FUNCTION__, pUser->pSD->wszName, wsCurrent);
				return false;
			}
			if(!Parser::IsValidSqlString(wsTarget))
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] used invalid apprentice name[%S]", __FUNCTION__, pUser->pSD->wszName, wsTarget);
				return false;
			}
			CPledge *pPledge = pUser->GetPledge();
			if(pPledge && pPledge->pSD)
			{
				try
				{
					if(pPledge->HasSubUnit(CPledge::ACADEMY))
					{
						g_Log.Add(CLog::Blue, "[%s] Current[%S] Target[%S] Option[%d]", __FUNCTION__, wsCurrent, wsTarget, nOption);
						if(nOption)
						{//Set Sponsor
							MemberData *pSponsor = pPledge->GetMemberDataByName(wsCurrent);
							if(pSponsor && pSponsor->nPledgeType != CPledge::ACADEMY)
							{
								//Check if user isnt a sponsor already
								if(pPledge->GetApprenticeDBID(pSponsor->nDBID))
								{
									pUser->SendSystemMessage(L"Selected member already is a sponsor.");
									return false;
								}
								//Check if apprentice hasnt a sponsor
								MemberData *pApprentice = pPledge->GetMemberDataByName(wsTarget);
								if(pApprentice && pApprentice->nPledgeType == CPledge::ACADEMY)
								{
									if(pApprentice->nSponsorID)
									{
										pUser->SendSystemMessage(L"Selected member already got a sponsor.");
										return false;
									}
									//Set a sponsor
									pApprentice->nSponsorID = pSponsor->nDBID;
									g_DB.RequestSetSponsor(pApprentice->nDBID, pSponsor->nDBID, pPledge->pSD->nDBID);
									if(pSponsor->nObjectID)
									{
										User *pTmp = User::GetUserBySID(&pSponsor->nObjectID);
										if(pTmp->ValidUser())
										{
											pTmp->pSD->nApprenticeSID = pApprentice->nObjectID;
										}
									}
									if(pApprentice->nObjectID)
									{
										User *pTmp = User::GetUserBySID(&pApprentice->nObjectID);
										if(pTmp->ValidUser())
										{
											pTmp->pSD->nSponsorSID = pSponsor->nObjectID;
										}
									}
									CSystemMessage msg(SystemMessage::$S2_HAS_BEEN_DESIGNATED_AS_THE_APPRENTICE_OF_CLAN_MEMBER_$S1_);
									msg.AddText(wsCurrent);
									msg.AddText(wsTarget);
									pPledge->Broadcast(msg.GetSize(), msg.GetBuff());
									pPledge->UpdateMemberList(pApprentice);
									return false;
								}
								pUser->SendSystemMessage(L"Selected member cannot be found in academy.");
								return false;
							}
							pUser->SendSystemMessage(L"Selected member cannot be found.");
							return false;
						}else
						{
							//Remove Sponsor
							MemberData *pSponsor = pPledge->GetMemberDataByName(wsCurrent);
							if(pSponsor)
							{
								//Check if sponsor has apprentice
								MemberData *pApprentice = pPledge->GetApprenticeData(pSponsor->nDBID);
								if(pApprentice)
								{
									pPledge->DismissApprentice(pApprentice);
								}else
								{
									pUser->SendSystemMessage(L"Selected member hasn't got an apprentice.");
									return false;
								}
							}else
								pUser->SendSystemMessage(L"Selected member cannot be found.");
							return false;
						}
					}else
						pUser->SendSystemMessage(L"Your clan hasn't got an academy.");
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
				}
			}
		}else
			pUser->GetSocket()->SendSystemMessage(SystemMessage::YOU_DO_NOT_HAVE_THE_RIGHT_TO_DISMISS_AN_APPRENTICE_);
	}
	return false;
}

void ClanSys::EnterWorld(User *pUser)
{
	CPledge *pPledge = pUser->GetPledge();
	if(pPledge && pPledge->pSD)
	{
		pPledge->AcquirePledgeSkills(pUser);
		MemberData *pData = pPledge->GetMemberDataByDBID(pUser->nDBID);
		if(pData)
		{
			INT64 nPower = NULL;			
			if(pData->nRank > 0 && pData->nRank < 10)
			{
				nPower = pPledge->pSD->PowerData[pData->nRank];
			}
			
			pUser->pSD->n64_PledgePower1 = C5ToC4Privilege(nPower);
			pUser->pSD->nPledgeType = pData->nPledgeType;
			//check for sponsor
			if(pData->nSponsorID != 0)
			{
				//inform sponsor that apprentice login
				if(User *pSponsor = g_UserDB.GetUserByDBID(pData->nSponsorID)->GetUser())
				{
					pSponsor->pSD->nApprenticeSID = pUser->nObjectID;
					pUser->pSD->nSponsorSID = pSponsor->nObjectID;
					CSystemMessage msg(SystemMessage::YOUR_APPRENTICE__$S1__HAS_LOGGED_IN_);
					msg.AddText(pUser->pSD->wszName);
					pSponsor->SendSystemMessage(&msg);
				}
			}
			//check for apprentice
			int nApprenticeID = pPledge->GetApprenticeDBID(pUser->nDBID);
			if(nApprenticeID != 0)
			{
				//inform apprentice that sponsor login
				if(User *pApprentice = g_UserDB.GetUserByDBID(nApprenticeID)->GetUser())
				{
					pApprentice->pSD->nSponsorSID = pUser->nObjectID;
					pUser->pSD->nApprenticeSID = pApprentice->nObjectID;
					CSystemMessage msg(SystemMessage::YOUR_SPONSOR__$C1__HAS_LOGGED_IN_);
					msg.AddText(pUser->pSD->wszName);
					pApprentice->SendSystemMessage(&msg);
				}
			}
		}
	}else
	{
		if(pUser->HaveSkill(247, 1))
		{
			pUser->DeleteSkill(247);
			pUser->SendDeleteSkillToDB(247);
			pUser->ValidateSkillList();
		}
	}
}

void ClanSys::LeaveWorld(User *pUser)
{
	CPledge *pPledge = pUser->GetPledge();
	if(pPledge)
	{
		UINT sponsorId = pUser->pSD->nSponsorSID;
		if(sponsorId)
		{
			//inform sponsor that apprentice logout
			User *pSponsor = User::GetUserBySID(&sponsorId);
			if(pSponsor->ValidUser())
			{
				pSponsor->pSD->nApprenticeSID = 0;
				pUser->pSD->nSponsorSID = 0;
				CSystemMessage msg(SystemMessage::YOUR_APPRENTICE__$C1__HAS_LOGGED_OUT_);
				msg.AddText(pUser->pSD->wszName);
				pSponsor->SendSystemMessage(&msg);
			}
		}

		UINT apprenticeId = pUser->pSD->nApprenticeSID;
		if(apprenticeId)
		{
			User *pApprentice = User::GetUserBySID(&apprenticeId);
			if(pApprentice->ValidUser())
			{
				//inform apprentice that sponsor logout
				pApprentice->pSD->nSponsorSID = 0;
				pUser->pSD->nApprenticeSID = 0;
				CSystemMessage msg(SystemMessage::YOUR_SPONSOR__$C1__HAS_LOGGED_OUT_);
				msg.AddText(pUser->pSD->wszName);
				pApprentice->SendSystemMessage(&msg);
			}
		}
	}
}

void ClanSys::CPledge_UpdateWindowPacket(CPledge *pPledge, User *pUser, bool bIsOnline)
{
	pPledge->UpdateWindowPacket(pUser, bIsOnline);
}

void ClanSys::CPledge_OpenAllWindowPacket(CPledge *pPledge, User *pUser, bool bGM, PWCHAR wszActiveChar)
{
	pPledge->OpenAllWindowPacket(pUser, bGM, wszActiveChar);
}

void ClanSys::CPledge_AddWindowPacket(CPledge *pPledge, User *pUser)
{
	pPledge->AddWindowPacket(pUser);
}

void ClanSys::CPledge_UpdateInfoWindowPacket(CPledge *pPledge)
{
	pPledge->UpdateInfo();
}
bool ClanSys::AtomicPledgeLevelUp_Do(AtomicPledgeLevelUp *pJob)
{
	typedef bool (*aplu)(AtomicPledgeLevelUp*);
	aplu _aplu = (aplu) 0x004333A0;
	if(pJob)
	{
		int nReputation = pJob->nLevel;
		if(nReputation < 0 || nReputation > 20)
		{
			User *pUser = User::GetUserBySID(&pJob->nUserSID);
			if(pUser->ValidUser())
			{
				if(pUser->pSD->nPledgeID && pUser->pSD->nPledgeID == pJob->nPledgeID)
				{
					CPledge *pPledge = pUser->GetPledge();
					if(pPledge && pPledge->pSD)
					{
						pPledge->UpdatePledgeNameValue(nReputation);
						if(nReputation > 0)
						{
							CSystemMessage msg(1777);
							msg.AddNumber(nReputation);
							pPledge->Broadcast(msg.GetSize(), msg.GetBuff());
							//pPledge->NotifyReputationIncreased(nReputation);
						}
						else
						{
							pPledge->NotifyReputationDecreased(nReputation);
						}

						g_Log.Add(CLog::Black, "[PledgeUpdateReputation] [%S] current[%d] update[%d]", pPledge->pSD->wszClanName, pPledge->pSD->nReputation, nReputation);
						return true;
					}
				}
			}else
				g_Log.Add(CLog::Error, "[%s] Invalid User[%d]", __FUNCTION__, pJob->nUserSID);
		}
	}
	return _aplu(pJob);
}

void ClanSys::PledgeLevelUpByNPC(CNPC *pNPC, User *pUser, int nLevel)
{
	typedef void(__fastcall *_levelUpByNPC)(CNPC*, User*, int nLevel);
	_levelUpByNPC lubn = (_levelUpByNPC) 0x00763840;
	
	CPledge *pPledge = pUser->GetPledge();
	CPledgeSharedData *pSD = pPledge->GetSharedData();
	if(pSD)
	{
		if(pPledge)
		{
			int nOldLevel = pSD->nLevel;
			if(nOldLevel >4 && nOldLevel < 8)
			{
				int nReputationPoints = pSD->nReputation;
				if(nOldLevel == 5)
				{
					if(nReputationPoints >= 10000)
					{
						//check for members count
						if(pPledge->m_Members.size() >= 30)
						{
							pPledge->IncreaseLevel678(6);
							pPledge->UpdatePledgeNameValue(-10000);
							//274	1	a,Your clan's skill level has increased.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(274);
							return;
						}else
						{
							pUser->GetSocket()->SendSystemMessage(L"Your clan does not have enough members to perform this action.");
						}
					}else
					{
						pUser->GetSocket()->SendSystemMessage(L"Your clan does not have enough reputation points to perform this action.");
					}
				}else if(nOldLevel == 6)
				{
					if(nReputationPoints >= 20000)
					{
						//check for members count
						if(pPledge->m_Members.size() >= 80)
						{
							pPledge->IncreaseLevel678(7);
							pPledge->UpdatePledgeNameValue(-20000);
							//274	1	a,Your clan's skill level has increased.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(274);
							return;
						}else
						{
							pUser->GetSocket()->SendSystemMessage(L"Your clan does not have enough members to perform this action.");
						}
					}else
					{
						pUser->GetSocket()->SendSystemMessage(L"Your clan does not have enough reputation points to perform this action.");
					}
				}else if(nOldLevel == 7)
				{
					if(nReputationPoints >= 40000)
					{
						//check for members count
						if(pPledge->m_Members.size() >= 120)
						{
							pPledge->IncreaseLevel678(8);
							pPledge->UpdatePledgeNameValue(-40000);
							//274	1	a,Your clan's skill level has increased.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(274);
							return;
						}else
						{
							pUser->GetSocket()->SendSystemMessage(L"Your clan does not have enough members to perform this action.");
						}
					}else
					{
						pUser->GetSocket()->SendSystemMessage(L"Your clan does not have enough reputation points to perform this action.");
					}
				}
			}
		}
	}
	lubn(pNPC, pUser, nLevel);
}

void ClanSys::ReplyCreatePledge(CPledge *pPledge, User *pUser)
{
	pPledge->AddDBMemberByUser(pUser);
	pPledge->pSD->nTotalCount += 1;
	int nDBID = pUser->nDBID;
	pPledge->PledgeLock->Lock(__FILEW__, __LINE__);
	MemberDataMap::iterator Iter = pPledge->m_Members.find(nDBID);
	if(Iter!=pPledge->m_Members.end())
	{
		if(Iter->second)
		{
			Iter->second->nRank = 6;
			Iter->second->nPledgeType = 0;
			Iter->second->nSponsorID = 0;
			Iter->second->nJoinLevel = pUser->pSD->nLevel;
		}
	}
	pPledge->PledgeLock->Unlock();
}
bool ClanSys::OnChangeClass(User *pUser, int nClass)
{
	if(pUser->ValidUser())
	{
		CPledge *pPledge = pUser->GetPledge();
		if(pPledge && pPledge->pSD && pPledge->pSD->nLevel > 4)
		{
			int nDBID = pUser->nDBID;

			if(MemberData* pMember = pPledge->GetMemberDataByDBID(nDBID))
			{
				int nPledgeType = 0;
				int nJoinPledgeLevel = 0;
				try
				{
					nPledgeType = pMember->nPledgeType;
					nJoinPledgeLevel = pMember->nJoinLevel;
				}catch(...)
				{
				}

				if(nPledgeType == CPledge::ACADEMY) //Academy
				{
					if(CCategoryDataDB::IsInCategory( nClass, CCategoryDataDB::THIRD_CLASS_GROUP))
					{
						//Second class change
						pUser->pSD->nPledgeType = -1; // academy member
						int nReputation = 170;
						if(nJoinPledgeLevel < 16)
							nJoinPledgeLevel = 16;
						if(nJoinPledgeLevel > 39)
							nJoinPledgeLevel = 39;
						nReputation += (39 - nJoinPledgeLevel) * 10;
						pPledge->UpdatePledgeNameValue(nReputation);
						pPledge->NotifyMemberGraduateFromAcademy(pUser, nReputation);
						//Leave clan
						g_DB.RequestUpdateUserPledge(pUser, pPledge, 2);
						pUser->AddItemToInventory(8181, 1);
					}
				}
			}
		}
	}
	return false;
}
bool ClanSys::AskJoinPledgePacket(CUserSocket *pSocket, const unsigned char* packet)
{
	if(pSocket->ValidUserSocket())
	{
		if(User *pUser = pSocket->GetUser())
		{
			CPledge *pPledge = pUser->GetPledge();
			if(pPledge && pPledge->pSD && !pUser->pSD->bHide)
			{
				if(CheckPrivilege(pUser->pSD->n64_PledgePower1, JOIN_CLAN) || pUser->pSD->nIsPledgeLeader > 0)
				{
					if(!(pPledge->pSD->nState == 3))
					{
						if(pPledge->CanAcceptNewMember())
						{
							if(pPledge->IsWorkingWithAnotherClan(30))
							{
								UINT nTargetID, nPledgeType;
								packet = Disassemble(packet, "dd", &nTargetID, &nPledgeType);
													
								if(pPledge->CanJoin(nPledgeType))
								{
									User *pTarget = User::GetUserBySID(&nTargetID);
									if(pTarget->ValidUser())
									{
										if(pTarget->pSD->nPledgeID == 0)
										{
											if(nPledgeType == -1) //Academy
											{
												int nClass = pTarget->pSD->nClass;
												if(pTarget->pSD->nLevel < 40)
												{
													if(CCategoryDataDB::IsInCategory(nClass, CCategoryDataDB::FIRST_CLASS_GROUP) || CCategoryDataDB::IsInCategory(nClass, CCategoryDataDB::SECOND_CLASS_GROUP))
													{
														//ok
													}else 
													{
														pSocket->SendSystemMessage(1734);//To join a Clan Academy, characters must be Level 40 or below, not belong another clan and not yet completed their 2nd class transfer.
														return false;
													}
												}else 
												{
													pSocket->SendSystemMessage(1734); //To join a Clan Academy, characters must be Level 40 or below, not belong another clan and not yet completed their 2nd class transfer.
													return false;
												}
											}

											if(pTarget->CanJoinPledge())
											{
												if(L2Server::RequestInvite(0, pUser, pTarget, 30))
												{
													SubUnitMap::iterator Iter = pPledge->pSD->pPledgeEx->mSubUnits.find(nPledgeType);
													if(Iter!= pPledge->pSD->pPledgeEx->mSubUnits.end())
													{
														pTarget->GetSocket()->Send("cdSdS", 0x32, pUser->nObjectID, Iter->second.Name.c_str(), nPledgeType, pPledge->pSD->wszClanName);
														pTarget->pSD->nPledgeType = nPledgeType;
													}else if( nPledgeType == 0)
													{
														//invite to main clan
														pTarget->GetSocket()->Send("cdSd", 0x32, pUser->nObjectID, pPledge->pSD->wszClanName, nPledgeType);
														pTarget->pSD->nPledgeType = nPledgeType;
													}
												}else
												{
													pSocket->SendSystemMessage(0xA4); //You are waiting for another reply
													pSocket->Send("cd", 0x33, 0);
												}
											}else
											{
												pSocket->Send("cdddS", 0x64, 760, 1, 0, pUser->pSD->wszName); //$s1 cannot join the clan because one day has not yet passed since he/she left another clan.
												pSocket->Send("cd", 0x33, 0);
											}
										}else
										{
											pSocket->SendSystemMessage(152);//You have invited the wrong target.
											pSocket->Send("cd", 0x33, 0);
										}
									}else
									{
										pSocket->SendSystemMessage(152);//You have invited the wrong target.
										pSocket->Send("cd", 0x33, 0);
									}
								}else
								{
									if(nPledgeType == 0)
										pSocket->Send("cdddS", 0x64, 1835, 1, 0, pPledge->pSD->wszClanName); //Main clan is full
									else
										pSocket->SendSystemMessage(233); //Subpledge is full or doesnt exist
									pSocket->Send("cd", 0x33, 0);
								}
							}else
							{
								//pSocket->SendSystemMessage(8); //You are working with another clan.
								pSocket->SendSystemMessage(0xA4); //You are waiting for another reply.
								pSocket->Send("cd", 0x33, 0);
							}
						}else
						{
							pSocket->SendSystemMessage(231);//After a clan member is dismissed from a clan, the clan must wait at least a day before accepting a new member.
							pSocket->Send("cd", 0x33, 0);
						}
					}else
					{
						pSocket->SendSystemMessage(0x107);//You have already requested the dissolution of your clan.
						pSocket->Send("cd", 0x33, 0);
					}
				}else
				{
					pSocket->SendSystemMessage(794); //You are not authorized to do that.
					pSocket->Send("cd", 0x33, 0);
				}
			}else
			{
				pSocket->SendSystemMessage(794);//You are not authorized to do that.
				pSocket->Send("cd", 0x33, 0);
			}
		}
	}
	return false;
}
bool ClanSys::AnswerJoinPledgePacket(CUserSocket *pSocket, const unsigned char* packet)
{
	if(pSocket->ValidUserSocket())
	{
		if(User *pUser = pSocket->GetUser())
		{
			int nAnswer;
			Disassemble(packet,"d", &nAnswer);
			CObjectSP ObjectSP;
			L2Server::GetInviter(&ObjectSP, pUser);
			User *pInviter = ObjectSP.pObject->GetUser();
			if(pInviter->ValidUser())
			{
				CPledge *pPledge = pInviter->GetPledge();
				if(pPledge)
				{
					if(nAnswer && pPledge->CanJoin(pUser->pSD->nPledgeType))
					{
						if(pUser->pSD->nPledgeID == 0)
						{
							g_DB.RequestUpdateUserPledge(pUser, pPledge, 1);
							L2Server::ResetInvite(pUser);
						}else
						{
							pInviter->GetSocket()->Send("cd", 0x33, 0);
							pPledge->nWaitingForReply = 0;
							L2Server::ResetInvite(pUser);
						}
					}else
					{
						L2Server::ResetInvite(pUser);
						pInviter->GetSocket()->Send("cd", 0x33, 0);
						pPledge->nWaitingForReply = 0;
					}
				}else
				{
					pInviter->GetSocket()->Send("cd", 0x33, 0);
					L2Server::ResetInvite(pUser);
				}
			}else
			{
				pSocket->SendSystemMessage(145);
				L2Server::ResetInvite(pUser);
			}
		}
	}
	return false;
}

void ClanSys::CPledge_Join(CPledge *pPledge, User *pUser)
{
	int nPledgeType = pUser->pSD->nPledgeType;
	int nDBID = pUser->nDBID;
	pPledge->AddDBMemberByUser(pUser);
	pPledge->PledgeLock->Lock(__FILEW__, __LINE__);
	pPledge->pSD->nTotalCount += 1;
	MemberData *pMember = pPledge->m_Members[nDBID];
	pMember->nDBID = nDBID;
	pMember->nPledgeType = nPledgeType;
	pMember->nSponsorID = 0;
	pMember->nJoinLevel = pUser->pSD->nLevel;
	int nRank = pPledge->GetDefaultRank(nPledgeType);
	pMember->nRank = nRank;
	pPledge->PledgeLock->Unlock();
	pPledge->Join(pUser);
	pPledge->AcquirePledgeSkills(pUser);
	pUser->pSD->n64_PledgePower1 = C5ToC4Privilege(pPledge->pSD->PowerData[nRank]);
	g_DB.RequestSaveJoinPledgeLevel(pUser->nDBID, pUser->pSD->nLevel);
	g_DB.RequestSavePledgePower(pUser);
	g_DB.RequestSetUserPledgeTypeAndRank(pUser->nDBID, nRank, nPledgeType);
}



void ClanSys::CPledge_Withdraw(CPledge *pPledge, User *pUser)
{
	//check if it's a captain
	int nPledgeType = pPledge->IsCaptainOfSubpledge(pUser->pSD->wszName);
	if(nPledgeType)
	{
		g_DB.RequestSetSubCaptain(pPledge->pSD->nDBID, nPledgeType, L"", 0);
	}
	pPledge->DeletePledgeSkills(pUser);
	
	MemberData *pData = pPledge->GetMemberDataByDBID(pUser->nDBID);
	if(pData && pData->nSponsorID)
	{
		pPledge->DismissApprentice(pData);
	}

	pPledge->Withdraw(pUser);

	pPledge->pSD->nTotalCount -= 1;
	pUser->pSD->nPledgeType = 0;
	g_DB.RequestSetUserPledgeTypeAndRank(pUser->nDBID, 6, 0);
	g_DB.RequestSaveJoinPledgeLevel(pUser->nDBID, 0);
}

INT32 g_OustedUserPledgeType[16];

void ClanSys::CPledge_Oust(CPledge *pPledge, int nUserDBID, PWCHAR wszName)
{
	//check if it's a captain
	int nPledgeType = pPledge->IsCaptainOfSubpledge(wszName);
	if(nPledgeType)
	{
		g_DB.RequestSetSubCaptain(pPledge->pSD->nDBID, nPledgeType, L"", 0);
	}
	MemberData *pData = pPledge->GetMemberDataByDBID(nUserDBID);
	if(pData)
	{
		UINT threadIdx = GetThreadIndex();
		if(threadIdx < 16)
		{
			g_OustedUserPledgeType[threadIdx] = pData->nPledgeType;
		}
		if(pData->nSponsorID)
		{
			pPledge->DismissApprentice(pData);
		}
	}
	pPledge->Oust(nUserDBID, wszName);
	pPledge->pSD->nTotalCount -= 1;
	if(User *pUser = g_UserDB.GetUserByDBID(nUserDBID))
	{
		pUser->pSD->nPledgeType = 0;
	}
	g_DB.RequestSetUserPledgeTypeAndRank(nUserDBID, 6, 0);
	g_DB.RequestSaveJoinPledgeLevel(nUserDBID, 0);
}

void ClanSys::CPledge_TakeOustPenalty(CPledge *pPledge)
{
	guard;

	UINT threadIdx = GetThreadIndex();
	if(threadIdx < 16)
	{
		if(g_OustedUserPledgeType[threadIdx] != CPledge::ACADEMY)
		{
			typedef void(*f)(CPledge*);
			f(0x764840L)(pPledge);
		}
	}else
	{
		typedef void(*f)(CPledge*);
		f(0x764840L)(pPledge);
		g_Log.Add(CLog::Error, "[%s] thread index overflow[%d]", __FUNCTION__, threadIdx);
	}

	unguard;
}

void ClanSys::User_TakeOustedPenalty(User *pUser)
{
	guard;
	UINT threadIdx = GetThreadIndex();
	if(threadIdx < 16)
	{
		if(g_OustedUserPledgeType[threadIdx] != CPledge::ACADEMY)
		{
			typedef void(*f)(User*);
			f(0x803A90L)(pUser);
		}
	}else
	{
		typedef void(*f)(User*);
		f(0x803A90L)(pUser);
		g_Log.Add(CLog::Error, "[%s] thread index overflow[%d]", __FUNCTION__, threadIdx);
	}
	unguard;
}

void ClanSys::CDB_RequestSeUserPledgeInfo(LPVOID lpInstance, UINT charId, INT32 field, INT32 value)
{
	guard;
	UINT threadIdx = GetThreadIndex();
	if(threadIdx < 16)
	{
		if(g_OustedUserPledgeType[threadIdx] != CPledge::ACADEMY)
		{
			typedef void(*f)(LPVOID, UINT, INT32, INT32);
			f(0x523630L)(lpInstance, charId, field, value);
		}
	}else
	{
		typedef void(*f)(LPVOID, UINT, INT32, INT32);
		f(0x523630L)(lpInstance, charId, field, value);
		g_Log.Add(CLog::Error, "[%s] thread index overflow[%d]", __FUNCTION__, threadIdx);
	}
	unguard;
}

bool ClanSys::RequestShowPledgeSkillList(User *pUser)
{
	//Acquire Pledge Skill
	if(CPledge *pPledge = pUser->GetPledge())
	{
		if(pUser->pSD->nIsPledgeLeader > 0)
		{
			pPledge->SendSkillList(pUser);
		}else
			pUser->pSocket->SendSystemMessage(L"You're not allowed to manage clan skills.");
	}else
		pUser->pSocket->SendSystemMessage(L"Clan skills are available for clan members only.");
	return false;
}
bool ClanSys::GotEnoughReputationForSubpledge(CPledge *pPledge, int nPledgeType)
{
	if(pPledge && pPledge->pSD)
	{
		try
		{
			if(nPledgeType == CPledge::ACADEMY)
			{
				if(pPledge->pSD->nReputation >= g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ACADEMY_SUBPLEDGE))
					return true;
			}else if(nPledgeType == CPledge::ROYAL_GUARD_I || nPledgeType == CPledge::ROYAL_GUARD_II)
			{
				if(pPledge->pSD->nReputation >= g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ROYAL_GUARD_SUBPLEDGE))
					return true;
			}else if(nPledgeType >= CPledge::ORDER_OF_KNIGHTS_I && nPledgeType <= CPledge::ORDER_OF_KNIGHTS_IV)
			{
				if(pPledge->pSD->nReputation >= g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::ORDER_OF_KNIGHT_SUBPLEDGE))
					return true;
			}
		}
		catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
		}
	}
	return false;
}
bool ClanSys::CreateSubUnit(User *pUser, int nPledgeType, wstring wPledgeName, wstring wMasterName)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	try
	{
		CPledge *pPledge = pUser->GetPledge();
		if(pUser->pSD->nIsPledgeLeader > 0)
		{
			if(pPledge && pPledge->pSD)
			{
				if(pPledge->IsValidSubPledgeName(wPledgeName) && Parser::IsValidSqlString(wPledgeName.c_str()))
				{
					if(pPledge->pSD->nLevel > 4)
					{
						if( GotEnoughReputationForSubpledge(pPledge, nPledgeType) )
						{
							if( nPledgeType == CPledge::ACADEMY)
							{
								pPledge->CreateSubPledge(pUser, nPledgeType, (PWCHAR)wPledgeName.c_str());
							}else
							{
								if(wMasterName.size() > 0 && Parser::IsValidSqlString(wMasterName.c_str()))
								{
									if(pPledge->CreateSubPledge(pUser, nPledgeType, (PWCHAR)wPledgeName.c_str()))
									{
										pPledge->SetSubCaptain(pUser, nPledgeType, (PWCHAR)wMasterName.c_str());
										if(nPledgeType == CPledge::ACADEMY)
										{
											const WCHAR* pFile = 0;
											if(g_HtmlCache.IsEnabled())
											{
												pFile = g_HtmlCache.Get(L"pl_create_ok_aca.htm");
											}else
											{
												pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_create_ok_aca.htm", pUser->pSD->bLang);
											}
											if(pFile)
											{
												PlayerAction::ShowHTML(pUser, L"pl_create_ok_aca.htm", pFile, 0);
											}
										}else if( nPledgeType == CPledge::ROYAL_GUARD_I || nPledgeType == CPledge::ROYAL_GUARD_II )
										{
											const WCHAR* pFile = 0;
											if(g_HtmlCache.IsEnabled())
											{
												pFile = g_HtmlCache.Get(L"pl_create_ok_sub1.htm");
											}else
											{
												pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_create_ok_sub1.htm", pUser->pSD->bLang);
											}
											if(pFile)
											{
												PlayerAction::ShowHTML(pUser, L"pl_create_ok_sub1.htm", pFile, 0);
											}
										}else if( nPledgeType >= CPledge::ORDER_OF_KNIGHTS_I && nPledgeType <= CPledge::ORDER_OF_KNIGHTS_IV)
										{
											const WCHAR* pFile = 0;
											if(g_HtmlCache.IsEnabled())
											{
												pFile = g_HtmlCache.Get(L"pl_create_ok_sub2.htm");
											}else
											{
												pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_create_ok_sub2.htm", pUser->pSD->bLang);
											}
											if(pFile)
											{
												PlayerAction::ShowHTML(pUser, L"pl_create_ok_sub2.htm", pFile, 0);
											}
										}
									}
								}else
									pUser->GetSocket()->SendSystemMessage(L"Incorrect Captain's name.");
							}
						}else
						{
							const WCHAR* pFile = 0;
							if(g_HtmlCache.IsEnabled())
							{
								pFile = g_HtmlCache.Get(L"pl_err_fame.htm");
							}else
							{
								pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_err_fame.htm", pUser->pSD->bLang);
							}
							if(pFile)
							{
								PlayerAction::ShowHTML(pUser, L"pl_err_fame.htm", pFile, 0);
							}
						}
					}else 
						pUser->GetSocket()->SendSystemMessage(L"Your clan level does not meet the requirement to create a Sub-Unit. Sub-Units are available for clans that are level 5 and above.");
				}else
				{
					//Incorect name
					const WCHAR* pFile = 0;
					if(g_HtmlCache.IsEnabled())
					{
						pFile = g_HtmlCache.Get(L"pl_already_subname.htm");
					}else
					{
						pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_already_subname.htm", pUser->pSD->bLang);
					}
					if(pFile)
					{
						PlayerAction::ShowHTML(pUser, L"pl_already_subname.htm", pFile, 0);
					}
				}
			}
		}else
			pUser->GetSocket()->SendSystemMessage(L"Sub-Unit can be managed by the clan leader only.");
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
	}
	return false;
}
bool ClanSys::RequestRenameSubPledge(User *pUser, int nPledgeType, wstring wPledgeName)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	try
	{
		if(pUser->ValidUser())
		{
			if(pUser->pSD->nIsPledgeLeader)
			{
				CPledge *pPledge = pUser->GetPledge();
				if(pPledge)
				{
					if( pPledge->IsValidSubPledgeName(wPledgeName))
					{
						pPledge->PledgeLock->Lock(__FILEW__, __LINE__);
						if(pPledge->HasSubUnit(nPledgeType) && nPledgeType != CPledge::MAIN_CLAN)
						{
							SubUnitMap::iterator Iter = pPledge->pSD->pPledgeEx->mSubUnits.find(nPledgeType);
							if(Iter != pPledge->pSD->pPledgeEx->mSubUnits.end())
							{
								Iter->second.Name = wPledgeName;
								pPledge->NotifySubPledgeCreated(nPledgeType);
								if(nPledgeType == CPledge::ACADEMY)
								{
									pUser->SendSystemMessage(L"The Academy has successfully been renamed.");
								}else if(nPledgeType == CPledge::ROYAL_GUARD_I || nPledgeType == CPledge::ROYAL_GUARD_II)
								{
									pUser->SendSystemMessage(L"The Royal Guard has successfully been renamed.");
								}else if(nPledgeType >= CPledge::ORDER_OF_KNIGHTS_I && nPledgeType <= CPledge::ORDER_OF_KNIGHTS_IV)
								{
									pUser->SendSystemMessage(L"The Order of Knights has successfully been renamed.");
								}
								g_DB.RequestRenameSubPledge(pPledge->pSD->nDBID, nPledgeType, (PWCHAR)wPledgeName.c_str());
							}
						}
						pPledge->PledgeLock->Unlock();
					}else
					{
						//Incorect name
						const WCHAR* pFile = 0;
						if(g_HtmlCache.IsEnabled())
						{
							pFile = g_HtmlCache.Get(L"pl_already_subname.htm");
						}else
						{
							pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_already_subname.htm", pUser->pSD->bLang);
						}
						if(pFile)
						{
							PlayerAction::ShowHTML(pUser, L"pl_already_subname.htm", pFile, 0);
						}
					}
				}
			}else
				pUser->GetSocket()->SendSystemMessage(L"You're not allowed to manage Sub-Units.");
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
	}
	return false;
}
bool ClanSys::RequestSetSubUnitCaptain(User *pUser, int nPledgeType, wstring wCaptain)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	try
	{
		CPledge *pPledge = pUser->GetPledge();
		if(pUser->pSD->nIsPledgeLeader > 0)
		{
			if(pPledge && pPledge->pSD)
			{
				if(pPledge->pSD->nLevel > 5)
				{
					if(wCaptain.size() > 0 && wCaptain.size() < 25)
					{
						if(pPledge->SetSubCaptain(pUser, nPledgeType, (PWCHAR)wCaptain.c_str()))
						{
							const WCHAR* pFile = 0;
							if(g_HtmlCache.IsEnabled())
							{
								pFile = g_HtmlCache.Get(L"pl_create_ok_submaster.htm");
							}else
							{
								pFile = g_HTMLCacheManager.GetHTMLFile( L"pl_create_ok_submaster.htm", pUser->pSD->bLang);
							}
							if(pFile)
							{
								PlayerAction::ShowHTML(pUser, L"pl_create_ok_submaster.htm", pFile, 0);
							}
						}
					}else
						pUser->GetSocket()->SendSystemMessage(L"Incorrect Captain's name.");
				}else pUser->GetSocket()->SendSystemMessage(L"Your clan level does not meet the requirements to set a captain.");
			}
		}else
			pUser->GetSocket()->SendSystemMessage(L"Only the clan leader is able to set the captain of a sub-clan.");
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
	}
	return false;
}
bool ClanSys::RequestExchangeReputationForSetItems(User *pUser, int nSetType)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	CPledge *pPledge = pUser->GetPledge();
	if(pUser->pSD->nIsPledgeLeader > 0)
	{
		if(pPledge && pPledge->pSD)
		{
			if(nSetType > 0 && nSetType < 7)
			{
				if(nSetType < 4) 
				{	//Oath
					int nOathPrice = g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::OATH_SET);
					if(pPledge->pSD->nReputation >= nOathPrice)
					{
						if((pUser->pSD->nMaxSlot - pUser->pSD->nCurrSlot) >= 4)
						{
							switch(nSetType)
							{
							case 1:
								{
									//Heavy
									pUser->AddItemToInventory(7850, 1);
									pUser->AddItemToInventory(7851, 1);
									pUser->AddItemToInventory(7852, 1);
									pUser->AddItemToInventory(7853, 1);
									break;
								}
							case 2:
								{
									//Light
									pUser->AddItemToInventory(7850, 1);
									pUser->AddItemToInventory(7854, 1);
									pUser->AddItemToInventory(7855, 1);
									pUser->AddItemToInventory(7856, 1);
									break;
								}
							case 3:
								{
									//Robe
									pUser->AddItemToInventory(7850, 1);
									pUser->AddItemToInventory(7857, 1);
									pUser->AddItemToInventory(7858, 1);
									pUser->AddItemToInventory(7859, 1);
									break;
								}
							default:
								{
									break;
								}
							}
							pPledge->UpdatePledgeNameValue(-nOathPrice);
						}else
							pUser->GetSocket()->SendSystemMessage(L"You do not have enough free space in your inventory.");
					}else
						pUser->GetSocket()->SendSystemMessage(L"Your clan does not have enough reputation points to perform this action.");
				}else
				{	//Apella
					int nAppellaPrice = g_Config.ClanInfo.GetPrice(CConfig::CClanInfo::APPELlA_SET);
					if(pPledge->pSD->nReputation >= nAppellaPrice)
					{
						if((pUser->pSD->nMaxSlot - pUser->pSD->nCurrSlot) >= 4)
						{
							switch(nSetType)
							{
							case 4:
								{
									//Heavy
									pUser->AddItemToInventory(7860, 1);
									pUser->AddItemToInventory(7861, 1);
									pUser->AddItemToInventory(7862, 1);
									pUser->AddItemToInventory(7863, 1);
									break;
								}
							case 5:
								{
									//Light
									pUser->AddItemToInventory(7860, 1);
									pUser->AddItemToInventory(7864, 1);
									pUser->AddItemToInventory(7865, 1);
									pUser->AddItemToInventory(7866, 1);
									break;
								}
							case 6:
								{
									//Robe
									pUser->AddItemToInventory(7860, 1);
									pUser->AddItemToInventory(7867, 1);
									pUser->AddItemToInventory(7868, 1);
									pUser->AddItemToInventory(7869, 1);
									break;
								}
							default:
								{
									break;
								}
							}
							pPledge->UpdatePledgeNameValue(-nAppellaPrice);
						}else
							pUser->GetSocket()->SendSystemMessage(L"You do not have enough free space in your inventory.");
					}else
						pUser->GetSocket()->SendSystemMessage(L"Your clan does not have enough reputation points to perform this action.");
				}
			}else
				g_Log.Add(CLog::Error, "[Ext][%s][%d] Invalid SetType[%d] User[%ls]", __FILE__, __LINE__, nSetType, pUser->pSD->wszName);
		}
	}else pUser->GetSocket()->SendSystemMessage(L"Only the clan leader is able to exchange reputation points.");
	return false;
}

bool ClanSys::AcquireSkillInfo(CUserSocket *pSocket, int nSkillID, int nLevel)
{
	guard;
	if(nSkillID > 369 && nSkillID < 392)
	{
		const PledgeSkillInfo& psi = g_PledgeSkillDb.GetSkillInfo(nSkillID, nLevel);
		if(psi.skillId)
		{
			CHAR itemBuff[8190];
			int itemLen = 0;
			for(UINT n=0;n<psi.itemConsume.size();n++)
			{
				const ItemReq& ir = psi.itemConsume[n];
				itemLen += Assemble(&itemBuff[itemLen], sizeof(itemBuff) - itemLen, "dd", ir.itemId, ir.amount);
			}
			pSocket->Send("cddddddbd", 0x8B, nSkillID, nLevel, psi.reputationConsume, 2, 1, psi.itemConsume.size(), itemLen, itemBuff, 0);
		}
	}
	unguard;
	return false;
}

bool ClanSys::RequestAcquireSkill(CUserSocket *pSocket, int nSkillID, int nLevel)
{
	guard;
	User *pUser = pSocket->GetUser();
	CPledge *pPledge = pUser->GetPledge();
	if(pPledge && pPledge->pSD && nSkillID > 369 && nSkillID < 392)
	{
		const PledgeSkillInfo& psi = g_PledgeSkillDb.GetSkillInfo(nSkillID, nLevel);
		if(pPledge->pSD->nLevel >= psi.requiredPledgeLevel) //Only clan lvl 5+ can acquire clan skills
		{
			if(pUser->pSD->nIsPledgeLeader)
			{
				if(psi.skillId)
				{
					if(psi.requiredPledgeLevel <= pPledge->pSD->nLevel)
					{
						if(pPledge->pSD->nReputation >= psi.reputationConsume)
						{
							bool canAcquire = true;
							//check for required item(s)
							for(UINT n=0;n<psi.itemConsume.size();n++)
							{
								const ItemReq& ir = psi.itemConsume[n];
								if(!pUser->inventory.HaveItemByClassID(ir.itemId, ir.amount))
								{
									canAcquire = false;
									//347	1	a,Incorrect item count.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
									pSocket->SendSystemMessage(347);
									break;
								}
							}
							if(canAcquire)
							{
								//check for previous level of skill
								int acquiredLevel = 0;
								pPledge->pSD->pPledgeEx->m_Lock.Enter();
								PledgeSkillMap& skills = pPledge->pSD->pPledgeEx->m_Skills;
								if(psi.skillLevel > 1)
								{
									for(PledgeSkillMap::iterator it = skills.begin();it!=skills.end();it++)
									{
										if(it->first == nSkillID)
										{
											acquiredLevel = it->second.skillLevel;
											break;
										}
									}
								}
								acquiredLevel++;
								if(acquiredLevel == nLevel)
								{
									skills[nSkillID] = psi;
								}else
								{
									canAcquire = false;
								}
								pPledge->pSD->pPledgeEx->m_Lock.Leave();

								if(canAcquire)
								{
									//Delete required items
									for(UINT n=0;n<psi.itemConsume.size();n++)
									{
										const ItemReq& ir = psi.itemConsume[n];
										pUser->DeleteItemInInventory(ir.itemId, ir.amount);
									}
									//consume reputation points
									if(psi.reputationConsume > 0)
									{
										pPledge->UpdatePledgeNameValue(-psi.reputationConsume);
									}
									//save skill in db
									g_DB.RequestAddPledgeSkill(pPledge->pSD->nDBID, nSkillID, nLevel);
									pPledge->NotifyPledgeSkillAcquired(nSkillID, nLevel);
									pPledge->UpdateInfo();
									pPledge->SendSkillList(pUser);

								}
							}
						}else
						{
							//1852	1	a,The attempt to acquire the skill has failed because of an insufficient Clan Reputation Score.\0	4	79	9B	B0	FF	a,ItemSound3.sys_failed\0	a,	0	0	0	0	0	a,	a,popup\0
							pSocket->SendSystemMessage(1852);
						}
					}
				}
			}else
				pUser->GetSocket()->SendSystemMessage(L"Clan skills can only be managed by the clan leader.");
		}
	}
	unguard;
	return false;
}

bool ClanSys::Die(User *pUser, CCreature *pEnemy)
{
	if(pEnemy->ValidCreature() && pUser->ValidUser())
	{
		User *pUEnemy = NULL;

		if(pUser->IsAcademyMember())
			return false;
		if(pUser->olympiadUser.IsInside())
			return false;

		if((pUser->inPvpZone || pUser->pSD->nInCustomPvpZone ) && g_ReputationInPvPZone == FALSE)
			return false;

		if(pUser->pED->tvtUser.status == TvT::UserFighting)
		{
			return false;
		}

		if(pEnemy->IsSummon())
			pUEnemy = pEnemy->CastSummon()->GetMaster();

		if(pUser->ValidUser())
			pUEnemy = pEnemy->GetUser();

		if(pUEnemy->ValidUser())
		{
			CPledge *pUPledge = pUser->GetPledge();
			CPledge *pEPledge = pUEnemy->GetPledge();
			if(pUPledge && pUPledge->pSD && pEPledge && pEPledge->pSD)
			{
				if(pUPledge->pSD->nLevel > 4 && pUPledge->pSD->nReputation > 0 && pEPledge->pSD->nLevel > 4)
				{
					if(pUPledge->IsInWarWithEx(pEPledge->nObjectID))
					{
						pEPledge->UpdatePledgeNameValue(1);
						pUPledge->UpdatePledgeNameValue(-1);
						pEPledge->NotifyMemberKilledEnemyDuringWar();
						pUPledge->NotifyMemberKilledDuringWar(pUser->pSD->wszName);
					}
				}
			}
			
		}
	}
	return false;
}

void ClanSys::AC_ChangeOwner(CPledge *pPledge, int nLastOwnerID, int nResidenceID)
{
	if(pPledge)
	{
		if(nLastOwnerID)
		{
			CPledge *pOldPledge = GetPledgeByID(nLastOwnerID);
			if(pOldPledge)
			{
				pOldPledge->UpdatePledgeNameValue( -500 );
			}
			pPledge->UpdatePledgeNameValue(500);
		}else
		{
			pPledge->UpdatePledgeNameValue(500);
		}
	}
}

void ClanSys::CAgit_ChangeOwner(CResidence *pAgit, CPledge *pPledge, bool bCheck)
{
	typedef void (*_ChangeOwner)(CResidence*, CPledge *, bool);
	_ChangeOwner ChangeOwner = (_ChangeOwner) 0x00405880;

	if(pAgit)
	{
		if(pAgit->pRD)
		{
			if(g_Config.L2ServerConfig.IsRestoreOldCH())
			{
				g_DB.RequestRestoreCH(pAgit->pRD->nPledgeID, pAgit->pRD->nResidenceID);
			}
		}

		if(pPledge)
		{
			int nLastOwnerID = NULL;
			CResidenceData *pData = pAgit->pRD;
			if(pData)
			{
				try
				{
					AC_ChangeOwner(pPledge, pData->nPledgeID, pData->nResidenceID);
					const ResidenceDataEx& rd = g_ResidenceDBEx.GetData(pAgit->pRD->nResidenceID);
					if(rd.residence_id > 0)
					{
						if(rd.reward_id > 0 && rd.reward_count > 0)
						{
							User *pUser = pPledge->GetMaster();
							if(pUser->ValidUser())
							{
								pUser->AddItemToInventory(rd.reward_id, rd.reward_count);
							}else
							{
								User *pUser = NULL;
								pPledge->PledgeLock->Lock(__FILEW__, __LINE__);
								for(MemberDataMap::iterator Iter=pPledge->m_Members.begin(); Iter!=pPledge->m_Members.end(); Iter++)
								{
									MemberData *pMember = Iter->second;
									if(pMember)
									{
										if(pMember->nPledgeType == CPledge::MAIN_CLAN && pMember->nObjectID)
										{
											pUser = User::GetUserBySID(&pMember->nObjectID);
											if(pUser->ValidUser())
												break;
										}
									}
								}
								pPledge->PledgeLock->Unlock();
								if(pUser->ValidUser())
								{
									pUser->AddItemToInventory(rd.reward_id, rd.reward_count);
									CSystemMessage msg(2414); //2414	1	Becouse clan leader is offline the $s1(s) will be given to $s2.	0	79	9B	B0	FF			0	0	0	0	0		none
									msg.AddItem(rd.reward_id);
									msg.AddText(pUser->pSD->wszName);
									pPledge->Broadcast(msg.GetSize(), msg.GetBuff());									
								}
							}
						}
					}
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
				}
			}
		}
	}
	ChangeOwner(pAgit, pPledge, bCheck);
}

void ClanSys::CCastle_OnChangeOwner(CResidence *pCastle)
{
	typedef void (*_OnChangeOwner)(CResidence*);
	_OnChangeOwner OnChangeOwner = (_OnChangeOwner) 0x004A7AA0;
	if(pCastle)
	{
		CPledge *pPledge = pCastle->GetPledge();
		if(pPledge)
		{
			try
			{
				CResidenceData *pData = pCastle->pRD;
				if(pData)
				{
					AC_ChangeOwner(pPledge, pData->nPledgeID, pData->nResidenceID);
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
			}
		}
	}
	OnChangeOwner(pCastle);
}

void ClanSys::ApplyNewPenalty()
{
	g_HookManager.WriteMemoryDWORD(0xBF6F24, g_Config.ClanInfo.GetPenalty(PLEDGE_DISMISS_PENALTY_TIMEOUT));
	g_HookManager.WriteMemoryDWORD(0xBF6F20, g_Config.ClanInfo.GetPenalty(PLEDGE_DISMISS_TIMEOUT));
	g_HookManager.WriteMemoryDWORD(0xBF6F0C, g_Config.ClanInfo.GetPenalty(PLEDGE_OUST_PENALTY_TIMEOUT));
	g_HookManager.WriteMemoryDWORD(0xBF6F14, g_Config.ClanInfo.GetPenalty(PLEDGE_OUSTED_PENALTY_TIMEOUT));
	g_HookManager.WriteMemoryDWORD(0xBF6F10, g_Config.ClanInfo.GetPenalty(PLEDGE_WITHDRAW_PENALTY_TIMEOUT));
	g_HookManager.WriteMemoryDWORD(0xBF6F38, g_Config.ClanInfo.GetPenalty(ALLIANCE_DISMISS_PENALTY_TIMEOUT));
	g_HookManager.WriteMemoryDWORD(0xBF6F2C, g_Config.ClanInfo.GetPenalty(ALLIANCE_OUST_PENALTY_TIMEOUT));
	g_HookManager.WriteMemoryDWORD(0xBF6F34, g_Config.ClanInfo.GetPenalty(ALLIANCE_OUSTED_PENALTY_TIMEOUT));
	g_HookManager.WriteMemoryDWORD(0xBF6F30, g_Config.ClanInfo.GetPenalty(ALLIANCE_WITHDRAW_PENALTY_TIMEOUT));
}

void ClanSys::CCastle_StartSiege(CCastle *pCastle)
{
	typedef void (*_f)(CCastle*);
	_f f = (_f) 0x004AD270;
	f(pCastle);
}

void ClanSys::EndSiege(CCastle *pCastle)
{
	if(pCastle->pRD)
	{
		if(g_Config.IsSet(CConfig::SIEGE_REPORT))
		{
			CSiegeStat::GetInstance().OnSiegeEnd(pCastle->pRD->nResidenceID);
		}
		int nPledgeID = pCastle->pRD->nPledgeID;
		if(nPledgeID)
		{
			CPledge *pPledge = GetPledgeByID(nPledgeID);
			if(pPledge)
			{
				pPledge->UpdatePledgeNameValue(500);
				CPacket pck;
				pck.WriteC(0x64);
				pck.WriteD(1773);
				pck.WriteD(1);
				pck.WriteD(1);
				pck.WriteD(500);
				pPledge->Broadcast(pck.GetSize(), pck.GetBuff());
				const ResidenceDataEx& rd = g_ResidenceDBEx.GetData(pCastle->pRD->nResidenceID);
				if(rd.residence_id > 0)
				{
					if(rd.reward_id > 0 && rd.reward_count > 0)
					{
						User *pUser = pPledge->GetMaster();
						if(pUser->ValidUser())
						{
							pUser->AddItemToInventory(rd.reward_id, rd.reward_count);
						}else
						{
							User *pUser = NULL;
							pPledge->PledgeLock->Lock(__FILEW__, __LINE__);
							for(MemberDataMap::iterator Iter=pPledge->m_Members.begin(); Iter!=pPledge->m_Members.end(); Iter++)
							{
								MemberData *pMember = Iter->second;
								if(pMember)
								{
									if(pMember->nPledgeType == CPledge::MAIN_CLAN && pMember->nObjectID)
									{
										pUser = User::GetUserBySID(&pMember->nObjectID);
										if(pUser->ValidUser())
											break;
									}
								}
							}
							pPledge->PledgeLock->Unlock();
							if(pUser->ValidUser())
							{
								pUser->AddItemToInventory(rd.reward_id, rd.reward_count);
								CSystemMessage msg(2414); //2414	1	Becouse clan leader is offline the $s1(s) will be given to $s2.	0	79	9B	B0	FF			0	0	0	0	0		none
								msg.AddItem(rd.reward_id);
								msg.AddText(pUser->pSD->wszName);
								pPledge->Broadcast(msg.GetSize(), msg.GetBuff());									
							}
						}
					}
				}
			}
		}
	}else
		g_Log.Add(CLog::Blue, "[%s] pResidenceData null", __FUNCTION__);
}

void ClanSys::CPledge_DeleteDBMember(void *pPointer)
{
	try
	{
		L2Server::Delete(pPointer);
	}
	catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Access violation prevented.", __FUNCTION__);
	}
}
CPledge *ClanSys::GetRelationTo_GetPledge(User *pUser)
{
	if(pUser->ValidUser())
	{
		CPledge *pPledge = pUser->GetPledge();
		if(pPledge)
		{
			if(pUser->IsAcademyMember())
				return NULL;
			return pPledge;
		}else
			return NULL;
	}else
		return NULL;
}

void ClanSys::RequestLevelUp(User *pUser)
{
	guard;
	if(pUser->ValidUser())
	{
		if(pUser->pSD->nIsPledgeLeader)
		{
			if(CPledge *pPledge = pUser->GetPledge())
			{
				INT32 currentLevel = pPledge->pSD->nLevel;
				if(currentLevel < 8)
				{
					////;Pledge_x=item_id;count;sp_count;reputation_count;member_count
					//check for requirements
					
					if(g_PledgeLevelUpRequirements[currentLevel][0] > 0 && g_PledgeLevelUpRequirements[currentLevel][1] > 0)	//ItemId & count
					{
						if(!pUser->inventory.HaveItemByClassID(g_PledgeLevelUpRequirements[currentLevel][0], g_PledgeLevelUpRequirements[currentLevel][1]))
						{
							pUser->SendSystemMessage(L"You don't have enough required item(s) to increase clan level.");
							unguard;
							return;
						}
					}
					if(g_PledgeLevelUpRequirements[currentLevel][2] > pUser->pSD->nSp)
					{
						pUser->SendSystemMessage(L"You don't have enough sp to increase clan level.");
						unguard;
						return;
					}
					if(g_PledgeLevelUpRequirements[currentLevel][3] > pPledge->pSD->nReputation)
					{
						pUser->SendSystemMessage(L"You don't have enough clan reputation point(s) to increase clan level.");
						unguard;
						return;
					}
					if(g_PledgeLevelUpRequirements[currentLevel][4] > pPledge->m_Members.size())
					{
						pUser->SendSystemMessage(L"You don't have enough clan members to increase clan level.");
						unguard;
						return;
					}
					
					//Delete required stuff
					if(g_PledgeLevelUpRequirements[currentLevel][0] > 0 && g_PledgeLevelUpRequirements[currentLevel][1] > 0)	//ItemId & count
					{
						pUser->DeleteItemInInventory(g_PledgeLevelUpRequirements[currentLevel][0], g_PledgeLevelUpRequirements[currentLevel][1]);
					}
					if(g_PledgeLevelUpRequirements[currentLevel][2] > 0)
					{
						pUser->WriteLock(__FILEW__, __LINE__);
						pUser->pSD->nSp = pUser->pSD->nSp - g_PledgeLevelUpRequirements[currentLevel][2];
						pUser->WriteUnlock();
					}
					if(g_PledgeLevelUpRequirements[currentLevel][3] > 0)
					{
						pPledge->UpdatePledgeNameValue(-g_PledgeLevelUpRequirements[currentLevel][3]);
					}

					pPledge->pSD->nLevel = currentLevel + 1;
					pPledge->Save(5);
					pPledge->UpdateInfo();
					pUser->SendSystemMessage(L"Congratulations! You've increased your clan's level!");
					g_Logger.Add(L"Increase pledge[%d][%s] level[%d] by user[%d][%s]", pPledge->pSD->nDBID, pPledge->pSD->wszClanName, pPledge->pSD->nLevel, pUser->nDBID, pUser->pSD->wszName);
				}else
				{
					pUser->SendSystemMessage(L"You cannot increase clan level anymore.");
				}				
			}else
			{
				pUser->SendSystemMessage(L"System cannot find your clan!");
			}
		}else
		{
			pUser->SendSystemMessage(L"Only clan leader can perform this action!");
		}
	}
	unguard;
}

CPledgeSkillDb::CPledgeSkillDb()
{
	m_EmptySkill.skillId = 0;
	m_EmptySkill.skillLevel = 0;
	m_EmptySkill.requiredPledgeLevel = 0;
	m_EmptySkill.reputationConsume = 0;
	m_EmptySkill.requiredPledgeClass = 0;
}

CPledgeSkillDb::~CPledgeSkillDb()
{
}

void CPledgeSkillDb::Init()
{
	m_Skills.clear();
	LoadData();
}

void CPledgeSkillDb::LoadData()
{
	guard;
	wstringstream file(ReadFileW(L"..//Script//PledgeSkillAcquire.txt"));
	wstring line;
	int lineCnt = 0;
	int skillCnt = 0;
	if(file.str().size() == 0)
	{
		g_Log.Add(CLog::Error, "[%s] Missing content for ..//Script//PledgeSkillAcquire.txt !", __FUNCTION__);
	}
	while(getline(file, line))
	{
		lineCnt++;
		if(line.size() > 4)
		{
			if( line[0] == L'/' || line[0] == L';' )
				continue;

			if( line.find(L"skill_begin") == 0 )
			{
				//skill_begin	skill_id=388	skill_level=1	pledge_level=7	reputation_consume=1800	item_consume={{8176;1}}	affect_rank=viscount	skill_end
				PledgeSkillInfo psi;
				psi.skillId = Parser::ParseOptionInt(line, L"skill_id");
				psi.skillLevel = Parser::ParseOptionInt(line, L"skill_level");
				psi.requiredPledgeLevel = Parser::ParseOptionInt(line, L"pledge_level");
				psi.reputationConsume = Parser::ParseOptionInt(line, L"reputation_consume");
				wstring itemConsume = Parser::ParseOptionString(line, L"item_consume");
				itemConsume = Parser::Replace(itemConsume, L'{', L' ');
				itemConsume = Parser::Replace(itemConsume, L'}', L' ');
				itemConsume = Parser::Replace(itemConsume, L';', L' ');
				wstringstream itemStream(itemConsume);
				ItemReq ir;
				while(itemStream >> ir.itemId >> ir.amount)
				{
					psi.itemConsume.push_back(ir);
				}
				wstring affectRank = Parser::ParseOptionString(line, L"affect_rank");
				if(affectRank.size() > 0)
				{
					//vagabond, vassal, heir, knight, elder, baron, viscount, count, marquis
					if(affectRank == L"vagabond")
					{
						psi.requiredPledgeClass = VAGABOND;
					}else if(affectRank == L"vassal")
					{
						psi.requiredPledgeClass = VASSAL;
					}else if(affectRank == L"heir")
					{
						psi.requiredPledgeClass = HEIR;
					}else if(affectRank == L"knight")
					{
						psi.requiredPledgeClass = KNIGHT;
					}else if(affectRank == L"elder")
					{
						psi.requiredPledgeClass = ELDER;
					}else if(affectRank == L"baron")
					{
						psi.requiredPledgeClass = BARON;
					}else if(affectRank == L"viscount")
					{
						psi.requiredPledgeClass = VISCOUNT;
					}else if(affectRank == L"count")
					{
						psi.requiredPledgeClass = COUNT;
					}else if(affectRank == L"marquis")
					{
						psi.requiredPledgeClass = MARQUIS;
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Invalid affect_rank[%S] - line[%d]!", __FUNCTION__, affectRank.c_str(), lineCnt);
					}
				}else
				{
					psi.requiredPledgeClass = VAGABOND;
				}
				map<int, vector<PledgeSkillInfo>>::iterator it = m_Skills.find(psi.requiredPledgeLevel);
				if(it!=m_Skills.end())
				{
					it->second.push_back(psi);
					skillCnt++;
				}else
				{
					vector<PledgeSkillInfo> skills;
					skills.push_back(psi);
					m_Skills.insert(pair<int, vector<PledgeSkillInfo>>(psi.requiredPledgeLevel, skills));
					skillCnt++;
				}
				map<int, int>::iterator maxIt = m_MaxSkillLevel.find(psi.skillId);
				if(maxIt != m_MaxSkillLevel.end())
				{
					if(maxIt->second < psi.skillLevel)
					{
						maxIt->second = psi.skillLevel;
					}
				}else
				{
					m_MaxSkillLevel[psi.skillId] = psi.skillLevel;
				}
			}
		}
	}
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] skill(s).", __FUNCTION__, skillCnt);
	unguard;
}

const PledgeSkillInfo& CPledgeSkillDb::GetSkillInfo(int skillId, int level, bool showLog)
{
	guard;
	for(map<int, vector<PledgeSkillInfo>>::iterator it = m_Skills.begin(); it != m_Skills.end(); it++)
	{
		vector<PledgeSkillInfo>& skills = it->second;
		for(UINT n=0;n<skills.size();n++)
		{
			PledgeSkillInfo& psi = skills[n];
			if(psi.skillId == skillId && psi.skillLevel == level)
			{
				unguard;
				return psi;
			}
		}
	}
	if(showLog)
	{
		g_Log.Add(CLog::Error, "[%s] undefined skill[%d][%d]!", __FUNCTION__, skillId, level);
	}
	unguard;
	return m_EmptySkill;
}

void CPledgeSkillDb::GetAcquirableSkills(int pledgeLevel, std::vector<PledgeSkillInfo> &acquirableSkills)
{
	guard;
	for(map<int, vector<PledgeSkillInfo>>::iterator it = m_Skills.begin(); it!=m_Skills.end();it++)
	{
		if(it->first <= pledgeLevel)
		{
			const vector<PledgeSkillInfo>& skills = it->second;
			for(UINT n=0;n<skills.size();n++)
			{
				const PledgeSkillInfo& skill = skills[n];
				bool updated = false;
				for(UINT m=0;m<acquirableSkills.size();m++)
				{
					if(acquirableSkills[m].skillId == skill.skillId)
					{
						if(acquirableSkills[m].skillLevel < skill.skillLevel)
						{
							acquirableSkills[m] = skill;
							updated = true;
							break;
						}
					}
				}
				if(!updated)
				{
					acquirableSkills.push_back(skill);
				}
			}
		}else
		{
			break;
		}
	}
	unguard;
}

void CPledgeSkillDb::GetAcquirableSkills(int pledgeLevel, std::vector<PledgeSkillInfo> &acquirableSkills, map<int, PledgeSkillInfo>& acquiredSkills)
{
	guard;
	for(map<int, vector<PledgeSkillInfo>>::iterator it = m_Skills.begin(); it!=m_Skills.end();it++)
	{
		if(it->first <= pledgeLevel)
		{
			const vector<PledgeSkillInfo>& skills = it->second;
			for(UINT n=0;n<skills.size();n++)
			{
				const PledgeSkillInfo& skill = skills[n];
				bool canAcquire = true;
				map<int, PledgeSkillInfo>::iterator it = acquiredSkills.find(skill.skillId);
				if(it!=acquiredSkills.end())
				{
					//check for equal or higher level and previous level
					if((it->second.skillLevel + 1) != skill.skillLevel)
					{
						canAcquire = false;
					}
				}else if(skill.skillLevel > 1)
				{
					canAcquire = false;
				}
				if(canAcquire)
				{
					acquirableSkills.push_back(skill);
				}
			}
		}else
		{
			break;
		}
	}
	unguard;
}

int CPledgeSkillDb::GetMaxLevel(int skillId)
{
	guard;
	int level = 0;
	map<int, int>::iterator it = m_MaxSkillLevel.find(skillId);
	if(it!=m_MaxSkillLevel.end())
	{
		level = it->second;
	}
	unguard;
	return level;
}
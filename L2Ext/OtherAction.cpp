#include "StdAfx.h"
#include "OtherAction.h"
#include "ClanSys.h"

extern wstring g_ExtReleaseVersion;
extern INT32 g_MaxPartyLevelRange;
UINT64 g_AmpedAddr = 0;

BOOL g_IsDistributingExpSp[32];
CCreature* g_DistributingCreature[32];

double g_PartyExpRate = 1.0;


void DxSetSevenSignQuestResult_DoServerJob1(LPVOID pJob)
{
	typedef void (*f)(LPVOID pJob);
	f(0x59EF30L)(pJob);
	g_Log.Add(CLog::Blue, "[SevenSignQuest] Saved quest result");
	try
	{
		FOR_ALL_USERS_IN_MAP(SSQSystem::ValidateCpSkill);
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
}

bool SSQSystem_OnSetSealOwner1(LPVOID pInstance, int seal, int owner)
{
	typedef bool (*f)(LPVOID, int, int);
	
	if(f(0x59DEF0L)(pInstance, seal, owner))
	{
		if(seal == SSQSeal::seal3)
		{
			g_Log.Add(CLog::Blue, "[SevenSignQuest] Setting seal of strife owner[%d]", owner);
			FOR_ALL_USERS_IN_MAP(SSQSystem::ValidateCpSkill);
		}
		return true;
	}
	return false;
}

const unsigned char *ReplyContributionWinnerPledge_Disassemble(const unsigned char *packet, const char* format, LPINT lpResidenceId, LPINT lpType, LPINT lpPledgeId)
{
	const unsigned char* ret = Disassemble(packet, format, lpResidenceId, lpType, lpPledgeId);

	int nPledgeId = (*lpPledgeId);
	if(nPledgeId && (*lpType) == 1)
	{
		CPledge *pPledge = g_PledgeDB.GetPledge(nPledgeId);
		if(pPledge)
		{
			if(pPledge->pSD)
			{
				if(pPledge->pSD->nAgitID)
				{
					if(pPledge->pSD->nAgitID != (*lpResidenceId))
					{
						if(!g_Config.L2ServerConfig.IsRestoreOldCH())
						{
							CAgit *pAgit = (CAgit*)g_ResidenceDB.GetResidence(pPledge->pSD->nAgitID);
							if(pAgit)
							{
								pAgit->ResetOwner(true);
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

void CDB_RequestRegisterSiegeAgitPledge(LPVOID pInstance, User* pUser, CSiegeAgit* pAgit, CPledge* pPledge)
{
	typedef void (*f)(LPVOID, User*, CSiegeAgit*, CPledge*);

	if(pPledge && pUser->ValidUser())
	{
		if(pPledge->pSD->nAgitID && pAgit->IsAgit() && pAgit->pRD)
		{
			if(pAgit->pRD->nResidenceID != FORTRESS_OF_DEAD && pAgit->pRD->nResidenceID != DEVASTATED_CASTLE ) // not FoD and DC
			{
				pUser->GetSocket()->SendSystemMessage(847);
				return;
			}
		}
	}

	f(0x52EF60L)(pInstance, pUser, pAgit, pPledge);
}

void CCreatureController_ActionBroadcastSkillUse(CCreatureController* pCC, int nTargetID, int nSkillID, int nSkillLevel, int nEstimatedTime, int nReuseDelay, CSmartID* target, CSkillInfo* pSkillInfo, CSkillAction* pAction)
{
	typedef void (*f)(CCreatureController*, int, int, int, int, int, CSmartID*, CSkillInfo*, CSkillAction*);

	/* // Example usage of custom skill hit time in custom areas
	CCreature *pCaster = CCreature::GetCreatureBySID(&pAction->caster_sid);
	if(pCaster->ValidCreature())
	{
		if(pCaster->pSD->nInCustomPvpZone)
		{
			double Multipler = 2;
			pAction->timer_var *= Multipler;	//skill hit cancel time
			pAction->timer_var2 *= Multipler;	//skill hit time
			nEstimatedTime *= Multipler;		//fixes packet
		//	g_Log.Add(CLog::Blue, "[%s] [%d] [%d] Mult[%f]", __FUNCTION__, pAction->timer_var, pAction->timer_var2, Multipler);
		}
	}

	End of example */

	/* Static reuse feature start */
	CSkillInfoEx *pInfo = g_SkillDBEx.GetSkillInfoEx(nSkillID, nSkillLevel);
	if(pInfo)
	{
		if(pInfo->staticReuseTime > 0)
		{
		//	g_Log.Add(CLog::Blue, "[%s] Reuse[%d] Static[%d]", __FUNCTION__, nReuseDelay, pInfo->staticReuseTime);
			nReuseDelay = pInfo->staticReuseTime;
		}
	}
	/* Static reuse feature end */

	switch(nSkillID)
	{
	case 5130: { /* Volcano */ nSkillID = 1419;	break; }
	case 5131: { /* Tsunami */ nSkillID = 1421;	break; }
	case 5132: { /* Cyclone */ nSkillID = 1420;	break; }
	case 5133: { /* Gehenna */ nSkillID = 1423; break; }
	case 5145: { /* Day of doom */ nSkillID = 1422; break; }
	}

	f(0x69D8C0L)(pCC, nTargetID, nSkillID, nSkillLevel, nEstimatedTime, nReuseDelay, target, pSkillInfo, pAction);
}

INT32 AmpedHookOnReceive1(CUserSocket *pSocket, const unsigned char* packet, INT32 packetId, INT32 packetLen)
{
	guard;

	typedef INT32 (*f)(CUserSocket*, const unsigned char*, INT32, INT32);
	static f _f = (f)(g_AmpedAddr + 0xDD13);

	INT32 ret = 0;

	if(packetId == 0x05)
	{
		unguard;
		return 1;
	}

	if(packetId == 0x0B)
	{
		bool found = false;
		for(UINT n=0;n<34;n++)
		{
			if(packet[n] == 0 && packet[n+1] == 0)
			{
				found = true;
				break;
			}
		}
		if(found)
		{
			ret = _f(pSocket, packet, packetId, packetLen);
		}else
		{
			g_Log.Add(CLog::Error, "Invalid name in CharacterCreatePacket from ip[%d.%d.%d.%d]!", pSocket->addr.S_un.S_un_b.s_b1, pSocket->addr.S_un.S_un_b.s_b2, pSocket->addr.S_un.S_un_b.s_b3, pSocket->addr.S_un.S_un_b.s_b4);
			ret = -1;
		}
	}else
	{
		ret = _f(pSocket, packet, packetId, packetLen);
	}

	unguard;
	return ret;
}

INT32 AmpedHookOnReceive2(CUserSocket *pSocket, const unsigned char* packet, INT32 packetId, INT32 packetLen)
{
	CTL;

	typedef INT32 (*f)(CUserSocket*, const unsigned char*, INT32, INT32);
	static f _f = (f)(g_AmpedAddr + 0xDC6C);
	
	INT32 ret = 0;

	if(packetId == 0x05 || packetId == 0x08)
	{
	
		return 1;
	}
	if(packetId == 0x0B)
	{
		bool found = false;
		for(UINT n=0;n<34;n++)
		{
			if(packet[n] == 0 && packet[n+1] == 0)
			{
				found = true;
				break;
			}
		}
		if(found)
		{
			ret = _f(pSocket, packet, packetId, packetLen);
		}else
		{
			g_Log.Add(CLog::Error, "Invalid name in CharacterCreatePacket from ip[%d.%d.%d.%d]!", pSocket->addr.S_un.S_un_b.s_b1, pSocket->addr.S_un.S_un_b.s_b2, pSocket->addr.S_un.S_un_b.s_b3, pSocket->addr.S_un.S_un_b.s_b4);
			ret = -1;
		}
	}else
	{
		ret = _f(pSocket, packet, packetId, packetLen);
	}

	if(packetId == 0x21 || packetId == 0x20)
	{
		WCHAR wBypass[1024];
		Disassemble(packet, "S", sizeof(wBypass), wBypass);
		if(wcsstr(wBypass, L"pin_") == wBypass)
		{
			ret = 1;
		}

	}

//	g_Log.Add(CLog::Blue, "[%s] PacketNo[%x] ret[%d]", __FUNCTION__, packetId, ret);

	return ret;
}

void AmpedHTMLOverflowDec()
{
	guard;
	UINT idx = GetThreadIndex();
	if(idx < 32)
	{
		g_Log.Add(CLog::Error, "[%s] Overflow HTML decimal pattern!", __FUNCTION__);
	}
	g_Log.StackTrace(CLog::Error);

	unguard;
}

void COtherAction::Initialize()
{
	memset(g_IsDistributingExpSp, 0, sizeof(g_IsDistributingExpSp));
	memset(g_DistributingCreature, 0, sizeof(g_DistributingCreature));

	g_HookManager.WriteMemoryDWORD(0xA25930, (DWORD)COtherAction::CSkillAction2OnEnd);

	g_AmpedAddr = (UINT64)GetModuleHandle(_T("AmpeDx64.dll"));
	if(g_AmpedAddr)
	{
		//sell packet
		g_HookManager.WriteJump((g_AmpedAddr + 0xB843), (g_AmpedAddr + 0xBAB0), 1);
		//buy packet
		g_HookManager.WriteJump((g_AmpedAddr + 0xA1C3), (g_AmpedAddr + 0xA424), 1);

		g_HookManager.WriteCall((g_AmpedAddr + 0xC6C3), AmpedHTMLOverflowDec);
		g_HookManager.WriteCall((g_AmpedAddr + 0xED2C), AmpedDeleteItem, 4);
		g_HookManager.WriteCall( (g_AmpedAddr + 0xDCDF), AmpedHookOnReceive1, 0);
		g_HookManager.WriteCall( (g_AmpedAddr + 0xDC34), AmpedHookOnReceive2, 0);
	}

	g_HookManager.WriteCall(0x6B620E, CCreatureController_ActionBroadcastSkillUse, 0);

	g_HookManager.WriteJump( 0x571CC6, OnLoadItemHook, 22);
	g_HookManager.WriteJump( 0x539BB0, OnSaveItemHook, 0);

	g_HookManager.WriteCall(0x6B6EC4, CSkillAction2AddTimer, 0);
	g_HookManager.WriteCall(0x6B90C2, CSkillAction2AddTimer, 0);
	g_HookManager.WriteCall(0x6B9347, CSkillAction2AddTimer, 0);
	g_HookManager.WriteCall(0x64418E, COtherAction::CreateWindowExWHook, 1);

	g_HookManager.WriteCall(0x4360A6, TimeAttackBoard_GiveReward, 0);

	g_HookManager.WriteCall(0x59FE96, SSQSystem_OnSetSealOwner1, 0);
	g_HookManager.WriteCall(0x5A11F7, DxSetSevenSignQuestResult_DoServerJob1, 0);

	g_HookManager.WriteCall(0x8244B2, COtherAction::CGeo_InSiegeZone, 0);

	g_HookManager.WriteMemoryDWORD(0x988170, (DWORD)COtherAction::CBattleCamp_AttackedBy);

	g_HookManager.WriteCall(0x876E1F, CPartyMatchingRequest_JoinRoom, 0);

	g_HookManager.WriteCall(0x735728, CPartyRoomInfo_SetMaster, 0);
	g_HookManager.WriteCall(0x737A4C, CPartyRoomInfo_SetMaster, 0);
	g_HookManager.WriteCall(0x737F36, CPartyRoomInfo_SetMaster, 0);

	g_HookManager.WriteMemoryDWORD(0x9A4118, (DWORD)CCastle_OnSiegeEnd);
	//g_HookManager.WriteJump(0x873F94, 0x873FEF, 0);	//allow participe in another siege

	//party room level limit
	g_HookManager.WriteMemoryDWORD(0x737A66, 80);	
	g_HookManager.WriteMemoryDWORD(0x737F53, 80);

	const char* cachedVersion = "$Revision: 453 $";
	g_HookManager.WriteMemory(0x9BA080, (LPVOID)cachedVersion, 16);

	if(g_Config.L2ServerConfig.IsExtendClanHallSiege())
	{
		//clear old ch 
		g_HookManager.WriteCall(0x57BC47, ReplyContributionWinnerPledge_Disassemble, 0);
		//allow to register when having ch
		g_HookManager.WriteJump(0x874200, (PVOID)0x874239, 2); 
		//check for FoD and DC when registering to siege with CH
		g_HookManager.WriteCall(0x874249, CDB_RequestRegisterSiegeAgitPledge, 0);
	}

	g_HookManager.WriteCall(0x71D594, COtherAction::CParty_GetAllMemberWhoCanGetExpSp, 0);
	g_HookManager.WriteCall(0x51209D, COtherAction::CContributeData_Distribute, 0);

//	g_HookManager.WriteMemoryDWORD(0xA25908, (DWORD)COtherAction::CSkillAction2OnTick);

//	g_HookManager.WriteCall(0x52ACAB, COtherAction::CDBSocket_SendV, 0);	//hooked by amped
//	g_HookManager.WriteCall(0x52ACBE, COtherAction::CDBSocket_SendV, 0);	//hooked by amped
	g_HookManager.WriteCall(0x54DCFA, COtherAction::CDBSocket_SendV);

	g_HookManager.WriteCall(0x8E3ACA, COtherAction::ObjectDB_OnRegisterObject);
	g_HookManager.WriteCall(0x8E8C4E, COtherAction::ObjectDB_OnRegisterObject);

	g_HookManager.WriteCall(0x51B4FE, COtherAction::OnAuthSocketInitialize);
	g_HookManager.WriteCall(0x6270BE, COtherAction::OnShutdownStart);
}

void COtherAction::OnShutdownStart(PVOID pInstance, UINT timestamp, INT32 id)
{
	typedef void(*f)(PVOID, UINT, INT32);
	f(0x6261D0L)(pInstance, timestamp, id);

	//saving all players abnormals (buffs)
	g_UserDB.Lock();
	map<UINT, User*>& users = g_UserDB.GetUserMap();
	for(map<UINT, User*>::iterator iter = users.begin(); iter!=users.end(); iter++)
	{
		if(User *pUser = iter->second->SafeCastUser())
		{
			pUser->SaveInfectedSkills();
		}
	}
	g_UserDB.Unlock();
}

void COtherAction::OnAuthSocketInitialize(PVOID pSocket)
{
	Sleep(1000);
	typedef void(*f)(PVOID);
	f(0x61ECA0L)(pSocket);
}


void COtherAction::ObjectDB_OnRegisterObject(PVOID pInstance, CObject *pObj)
{
	guard;

	UINT classId = pObj->nObjectType;

	if(classId > 1000000)
	{
		//npc
		if(classId >= 1040000)
		{
			g_Log.Add(CLog::Error, "[CObjectDB::RegisterObject] NpcId overflow [%d] >= 40000!", classId - 1000000);
		}
	}else
	{
		//item or set
		if(classId >= 30000)
		{
			g_Log.Add(CLog::Error, "[CObjectDB::RegisterObject] ItemId overflow [%d] >= 30000!", classId);
		}
	}

	typedef void(*f)(PVOID, CObject*);
	f(0x6FC590L)(pInstance, pObj);

	unguard;
}

void COtherAction::CDBSocket_SendV(CDBSocket *pSocket, const char *format, va_list va)
{
	if(pSocket)
	{
		typedef void(*f)(CDBSocket*, const char*, va_list);
		f(0x54A810L)(pSocket, format, va);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Null CDBSocket!", __FUNCTION__);
		g_Log.StackTrace(CLog::Error);
	}
}

void COtherAction::CSkillAction2OnTick(CSkillAction *pAction)
{
	guard;
	typedef void(*f)(CSkillAction*);
	f(0x6B70F0L)(pAction);
	unguard;
}

void COtherAction::CSkillAction2OnEnd(CSkillAction* pAction, bool finishedWell)
{
	guard;

	//add removing attached skill here
	try
	{
		if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(pAction->GetSkillId(), pAction->GetSkillLevel()))
		{
			INT32 attachSkillId = pSI->nAttachSkillId;
			if(attachSkillId > 0)
			{
				if(CCreature *pTarget = pAction->GetTarget()->SafeCastCreature())
				{
					if(CSkillInfo *pASI = g_SkillDB.GetSkillInfo(attachSkillId, 1))
					{
						INT32 attachedLevel = pTarget->HasAbnormal(pASI->nAbnormalType);
						if(attachedLevel > 0)
						{
							//decrease by 1
							attachedLevel--;
							if(attachedLevel > 0)
							{
								//replace buff
								pTarget->UpdateAbnormalLevel(attachSkillId, attachedLevel, true);
							}else
							{
								pTarget->DispelByAbnormal(pASI->nAbnormalType, true);
							}
						}
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Cannot get attach skill[%d] info!", __FUNCTION__, attachSkillId);
					}
				}
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Handled Exception!", __FUNCTION__);
	}

	typedef void(*f)(CSkillAction*, bool);
	f(0x6B1830L)(pAction, finishedWell);

	unguard;
}

void COtherAction::CContributeData_Distribute(CContributeData *pData, CCreature *pCreature)
{
	guard;

	UINT idx = GetThreadIndex();
	if(idx < 32)
	{
		g_IsDistributingExpSp[idx] = TRUE;
		g_DistributingCreature[idx] = pCreature;
	}

	typedef void(*f)(CContributeData*, CCreature*);
	f(0x6F6490L)(pData, pCreature);

	if(idx < 32)
	{
		g_IsDistributingExpSp[idx] = FALSE;
		g_DistributingCreature[idx] = 0;
	}

	unguard;
}

void COtherAction::CParty_GetAllMemberWhoCanGetExpSp(LPVOID lpContainer, User** lpUser)
{
	if(User *pUser = (*lpUser)->GetUser())
	{
		//party level check
		if(g_MaxPartyLevelRange)
		{
			if(CParty *pParty = pUser->GetParty())
			{
				int baseLevel = pUser->pSD->nLevel;

				UserVector vMembers;
				pParty->GetAllMembersEx(vMembers, false);
				for(INT32 n = 0;n<vMembers.size();n++)
				{
					if(User *pMember = vMembers[n]->GetUser())
					{
						if( (baseLevel + g_MaxPartyLevelRange) < pMember->pSD->nLevel)
						{
							if(pUser->IsInRange(pMember, 1500))
							{
								return;
							}
						}
						if(CSummon *pSummon = pMember->GetSummonOrPet())
						{
							if( (baseLevel + g_MaxPartyLevelRange) < pSummon->pSD->nLevel)
							{
								if(pUser->IsInRange(pSummon, 1500))
								{
									return;
								}
							}
						}
					}
				}
			}
		}
	}

	typedef void(*f)(LPVOID, User**);
	f(0x7A1D60L)(lpContainer, lpUser);
}

double COtherAction::CBattleCamp_AttackedBy(CBattleCamp* pBattleCamp, CCreature* pEnemy, double Damage)
{
	typedef double (*f)(CBattleCamp*, CCreature*, double);

	if(g_Config.L2ServerConfig.IsProtectHeadquarter())
	{
		if(pEnemy->ValidCreature())
		{
			User *pEUser = NULL;
			if(pEnemy->ValidUser())
				pEUser = pEnemy->GetUser();
			else if(pEnemy->IsSummon())
				pEUser = pEnemy->CastSummon()->GetMaster();

			if(pEUser->ValidUser())
			{
				CPledge *pPledge = pEUser->GetPledge();
				if(pPledge && pPledge->pSD)
				{
					CResidence *pResidence = pBattleCamp->pResidence;
					if(pResidence)
					{
						if(pResidence->IsDefendingPledge(pPledge->pSD->nDBID) || pResidence->IsOffendingPledge(pPledge->pSD->nDBID))
						{
							return f(0x43E570L)(pBattleCamp, pEnemy, Damage);
						}
						//send system message about no dealing dmg
					}
				}
				return 0.0;
			}
		}
	}
	return f(0x43E570L)(pBattleCamp, pEnemy, Damage);
}

bool COtherAction::CGeo_InSiegeZone(LPVOID pGeo, int x, int y, int z, int zero)
{
	typedef bool (*f)(LPVOID, int, int, int, int);
	
	if(f(0x5CBCD0L)(pGeo, x, y, z, zero))
	{
		if(SSQPart::twilight == SSQSystem::GetInstance()->GetSealOwner(SSQSeal::seal3))
		{
			return false;
		}

		return true;
	}
	return false;
}

bool COtherAction::TimeAttackBoard_GiveReward(LPVOID pInstance, User* pUser, int nRoomNo, LPVOID pArray)
{
	//1775	1	Clan member $c1 was an active member of the highest-ranked party in the Festival of Darkness. $s2 points have been added to your clan's reputation score.	0	79	9B	B0	FF			0	0	0	0	0		none
	typedef bool (*f)(LPVOID, User*, int, LPVOID);

	if(f(0x64FF40L)(pInstance, pUser, nRoomNo, pArray))
	{
		if(CPledge *pPledge = pUser->GetPledge())
		{
			if(pPledge->pSD && pPledge->pSD->nLevel > 4)
			{
				int nPoints = 0;
				//as good i know it gives always static amount of points
				switch(nRoomNo)
				{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				default:
					{
						nPoints = 300;
						break;
					}
				}

				CSystemMessage msg(1775);
				msg.AddText(pUser->pSD->wszName);
				msg.AddNumber(nPoints);
				pPledge->UpdatePledgeNameValue(nPoints);
				pPledge->Broadcast(msg.GetSize(), msg.GetBuff());
			}
		}
		return true;
	}
	return false;
}

HWND COtherAction::CreateWindowExWHook(DWORD dwExStyle,LPCWSTR lpClassName,LPCWSTR lpWindowName,DWORD dwStyle,int X,int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam)
{
	DWORD dwSize;
	TCHAR File[256];
	GetModuleFileName( GetModuleHandle(_T("IlExt.dll")), File, 256 );
	dwSize = GetFileVersionInfoSize(File, NULL);
	LPVOID pData = new char[dwSize];
	memset(pData, 0, dwSize);
	GetFileVersionInfo(File, NULL, dwSize, pData);
	LPVOID pVersion = NULL;
	UINT dwVersionLen;
	VerQueryValue(pData, _T("\\StringFileInfo\\041504b0\\ProductVersion"), &pVersion, &dwVersionLen);
	WCHAR wVersion[128];
	//_AnsiToUnicode((char*)pVersion, wVersion);
	wcscpy_s(wVersion, 128, (const WCHAR*)pVersion);
	delete [] pData;
	wstring wTitle = L"L2Server - IlExt ";
	wTitle += wVersion;
	wTitle = CParser::Replace(wTitle, L',', L'.');
	g_ExtReleaseVersion = wTitle;
	wTitle += L" - Vanganth@hotmail.com";
	//Custom title
	//wTitle = "My Custom Title";

	g_SplashScreen.Hide();
	return CreateWindowExW(dwExStyle, lpClassName, wTitle.c_str(), dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void COtherAction::CSkillAction2AddTimer(CSkillAction *pAction, int Tick)
{
	CTL;

	typedef void (*f)(CSkillAction*, int);
	try
	{
		if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(pAction->GetSkillId(), pAction->GetSkillLevel()))
		{
			if(pAction->GetTimerVal1() == Tick && pAction->GetState() == 0)
			{
				/* Song/Dance mp consumption - start */
				if(pSI->IsSongOrDance())
				{
					if(CCreature *pCaster = pAction->GetCaster())
					{
						pAction->SetMpConsumeMultipler(pAction->GetMpConsumeMultipler() * pCaster->pSD->pExData->SkillMod.fSongDanceMpMultipler);
					}
				}
				/* Song/Dance mp consumption - end */

				/* Skill reuse system message - start */
				CCreature *pCaster = pAction->GetCaster();
				if(User *pUser = pCaster->SafeCastUser())
				{
					if(pUser->olympiadUser.IsInside())
					{
						DWORD secLeft = pUser->GetSkillReuse(pSI->nSkillID) - GetTickCount();
						secLeft /= 1000;
						if(secLeft > 0)
						{	//There are $s1 second(s) remaining in $s2's re-use time.
							CSystemMessage msg(2415);
							msg.AddNumber(secLeft);
							msg.AddSkill(pSI->nSkillID, pSI->nLevel);
							pUser->SendSystemMessage(&msg);
						}
					}
				}
				//instant cast time
				if(pSI->nInstantCastTime)
				{
					CCreature* pTarget = pAction->GetTarget()->SafeCastCreature();
					if(pCaster->ValidCreature() && pTarget)
					{
						if(pSI->nAttachSkillId != 0)
						{
							int attachSkillId = pSI->nAttachSkillId;
							INT32 maxLevel = g_SkillDBEx.GetMaxSkillLevel(attachSkillId);
							CSkillInfo *pASI = g_SkillDB.GetSkillInfo(attachSkillId, maxLevel);
							if(pASI)
							{
								INT32 attachLevel = pTarget->HasAbnormal(pASI->nAbnormalType);
								if(attachLevel < maxLevel)
								{
									attachLevel++;
								}
								CSkillInfo *pASI = g_SkillDB.GetSkillInfo(attachSkillId, attachLevel);
								if(pASI)
								{
									pASI->ActivateSkill(pCaster, pTarget, 0, 0);
								}else
								{
									g_Log.Add(CLog::Error, "[%s] Invalid attach skill[%d][%d] - cannot find skill info!", __FUNCTION__, attachSkillId, attachLevel);
								}
							}else
							{
								g_Log.Add(CLog::Error, "[%s] Invalid attach skill[%d] - cannot find skill info!", __FUNCTION__, attachSkillId);
							}
						}else
						{
							pSI->ActivateSkill(pCaster, pTarget, 0, 0);
						}
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Invalid Creature - pCaster[%p] pTarget[%p]", __FUNCTION__, pCaster, pTarget);
					}
					if(Tick < 80)
					{
						g_Log.Add(CLog::Error, "[%s] Invalid tick time[%d] skill_id[%d] level[%d] instant_activate_time[%d]", __FUNCTION__, Tick, pAction->GetSkillId(), pAction->GetSkillLevel(), pAction->GetSkillInfo()->nInstantCastTime);
						Tick = 600;
					}
				}
				/* Skill reuse system message - end */
/*
				if(pAction->pSI)
				{
					if(pAction->pSI->nInstantCastTime > 0)
					{
						if(pAction->timer_var > pAction->pSI->nInstantCastTime)
						{
							Tick = pAction->pSI->nInstantCastTime;
						}
					}
				}
*/
			}/*else if(pAction->state == 1)
			{
				if(pAction->pSI)
				{
					bool Execute = false;
					if(pAction->timer_var2 == Tick)
					{
						if(pAction->pSI->nInstantCastTime > 0)
						{
							Tick = pAction->timer_var - pAction->pSI->nInstantCastTime + pAction->timer_var2;
							Execute = true;
						}
					}else if(pAction->timer_var3 == Tick)
					{
						if(pAction->pSI->nInstantCastTime > 0)
						{
							Tick = pAction->timer_var - pAction->pSI->nInstantCastTime + pAction->timer_var3;
							Execute = true;
						}
					}
					if(Execute)
					{
						if(pAction->pSI)
						{
							CCreature* pCaster = CCreature::GetCreatureBySID(&pAction->caster_sid);
							CCreature* pTarget = CCreature::GetCreatureBySID(&pAction->target_sid);
							if(pCaster->ValidCreature() && pTarget->ValidCreature())
							{
								if(pAction->pSI->nAttachSkillId != 0)
								{
									int attachSkillId = pAction->pSI->nAttachSkillId;
									INT32 maxLevel = g_SkillDBEx.GetMaxSkillLevel(attachSkillId);
									CSkillInfo *pASI = g_SkillDB.GetSkillInfo(attachSkillId, maxLevel);
									if(pASI)
									{
										INT32 attachLevel = pTarget->HasAbnormal(pASI->nAbnormalType);
										if(attachLevel < maxLevel)
										{
											attachLevel++;
										}
										CSkillInfo *pASI = g_SkillDB.GetSkillInfo(attachSkillId, attachLevel);
										if(pASI)
										{
											pASI->ActivateSkill(pCaster, pTarget, 0, 0);
										}else
										{
											g_Log.Add(CLog::Error, "[%s] Invalid attach skill[%d][%d] - cannot find skill info!", __FUNCTION__, attachSkillId, attachLevel);
										}
									}else
									{
										g_Log.Add(CLog::Error, "[%s] Invalid attach skill[%d] - cannot find skill info!", __FUNCTION__, attachSkillId);
									}
								}else
								{
									pAction->pSI->ActivateSkill(pCaster, pTarget, 0, 0);
								}
							}else
							{
								g_Log.Add(CLog::Error, "[%s] Invalid Creature - pCaster[%p] pTarget[%p]", __FUNCTION__, pCaster, pTarget);
							}
							if(Tick < 80)
							{
								g_Log.Add(CLog::Error, "[%s] Invalid tick time[%d] skill_id[%d] level[%d] instant_activate_time[%d]", __FUNCTION__, Tick, pAction->pSI->nSkillID, pAction->pSI->nLevel, pAction->pSI->nInstantCastTime);
								Tick = 600;
							}
						}
					}
				}
			}*/
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}

	f(0x696D40L)(pAction, Tick);
}

bool COtherAction::CPartyMatchingRequest_JoinRoom(LPVOID pInstance, CUserSP* pUserSP, int roomId, int locationFilter)
{
	typedef bool (*f)(LPVOID, CUserSP*, int, int);
	User *pUser = pUserSP->get();
	if(f(0x7284A0L)(pInstance, pUserSP, roomId, locationFilter))
	{
		g_PartyWaitingList.DelMember(pUser);
		return true;
	}
	return false;
}

void COtherAction::CPartyRoomInfo_SetMaster(CPartyRoomInfo *pInfo, CUserSP* pUserSP)
{
	typedef void (*f)(CPartyRoomInfo*, CUserSP*);

	g_PartyWaitingList.DelMember(pUserSP->get());

	f(0x737330L)(pInfo, pUserSP);
}

void COtherAction::CCastle_OnSiegeEnd(CCastle *pCastle)
{
	typedef void (*f)(CCastle*);

	f(0x4A6900L)(pCastle);

	ClanSys::EndSiege(pCastle);
}

void COtherAction::AmpedDeleteItem(LPVOID pInstance, CItem *pItem)
{
	if(pItem->IsValidItem())
	{
		if(pItem->pII->etcType == EtcItemCastleGuard)
		{
			return;
		}
	}

	g_World.AddToDelete(pItem);
	g_World.DeleteItemFromWorld(pItem, false);
}

CItem* OnLoadItem(const unsigned char* packet, const char *format, int *pItemDBID, int *pItemType, int *pAmount, int *pEnchant, int *pEroded, int *pBless, int *pIdent, int *pWished)
{
	typedef CItem*(*f)(INT64, int);
	int nAugmentationID = 0;
	int nManaLeft = 0;
	int nLifeTime = 0;
	UINT nProtectionTimeout = 0;

	packet = Disassemble(packet, "dddddddddddd", pItemDBID, pItemType, pAmount, pEnchant, pEroded, pBless, pIdent, pWished, &nAugmentationID, &nManaLeft, &nLifeTime, &nProtectionTimeout);

	CItem *pItem = f(0x6FAB60L)(0x0BC5D810, *pItemType);
	if(pItem->IsValidItem())
	{
		if(CShadowItem::IsShadowItem(*pItemType))
		{
			pItem->nManaLeft = nManaLeft;
		}else
		{
			pItem->nManaLeft = 0;
		}
		
		if(pItem->GetWeapon())
		{
			pItem->nAugmentationID = nAugmentationID;
		}else
		{
			pItem->nAugmentationID = 0;
		}

		if(nLifeTime == 0)
		{
			if(UINT lifeTime = g_ItemDBEx.GetLifeTime(*pItemType))
			{
				if(lifeTime > 0)
				{
					nLifeTime = time(0) + lifeTime;
				}
			}
		}
		pItem->nLifeTime = nLifeTime;
		pItem->nProtectionTimeout = nProtectionTimeout;
	//	g_Log.Add(CLog::Blue, "[%s] Loading and setting Augmentation[%d] ManaLeft[%d]", __FUNCTION__, nAugmentationID, nManaLeft);
	}
	return pItem;
}

int OnSaveItem(PCHAR packet, int nSize, CItem *pItem, int nItemDBID, int nCharDBID, int nItemType, int nAmount, int nEnchant, int nEroded, int nBless, int nIdent, int nWished, int nWarehouse)
{
	int nAugmentationID = 0;
	int nManaLeft = 0;
	int nLifeTime = 0;
	UINT nProtectionTimeout = 0;
	if(pItem->IsValidItem())
	{
		nAugmentationID = pItem->nAugmentationID;
		nManaLeft = pItem->nManaLeft;
		nLifeTime = pItem->nLifeTime;
		nProtectionTimeout = pItem->nProtectionTimeout;
	}
	return Assemble(packet, nSize, "dddddddddddddd", nItemDBID, nCharDBID, nItemType, nAmount, nEnchant, nEroded, nBless, nIdent, nWished, nWarehouse, nAugmentationID, nManaLeft, nLifeTime, nProtectionTimeout);
}


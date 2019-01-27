#include "StdAfx.h"
#include "L2Server.h"
#include "Random.h"
#include "DB.h"
#include "ObjectDB.h"

int SpoilAmountWrapper(double min, double max, double baseChance, double dice)
{
	int amount = 0;
//	g_Log.Add(CLog::Blue, "[%s] min[%f] max[%f] baseChance[%f] dice[%f]", __FUNCTION__, min, max, baseChance, dice);

	if(dice < baseChance)
	{
		double multipler = baseChance / 100.0;
		if(multipler > 1.0)
		{
			min *= multipler;
			max *= multipler;
		}
		max = max - min;
		if(max > 0.0)
		{
			min += g_Random.RandDouble(max);
		}
		if(min > 0.0)
		{
			min *= 10;
			amount = static_cast<int>(min);
			if((amount % 10) >= 5)
			{
				amount += 10;
				amount /= 10;
			}else
			{
				amount /= 10;
			}
		}
	}
	return amount;
}

void BroadcastToNeighborExceptSelf(int objectID, int range, FVector* pPos, int buffLen, PCHAR buff)
{
	typedef void(*f)(INT64, int, int, FVector*, const char*, int, PCHAR);
	f(0x4E8CB0L)(0x0E427AA8, objectID, range, pPos, "b", buffLen, buff);
}

BOOL AppendToFile(HANDLE hFile, const wchar_t* format, ...)
{
	typedef BOOL (*f)(HANDLE, const wchar_t* format, ...);
	return f(0x5B7BA0L)(hFile, format);
}

void L2Server::BroadcastToAllUser(size_t len, PCHAR buff)
{
	typedef void (*f)(const char *format, int, PCHAR);
	f(0x00822F40L)("b", static_cast<int>(len), buff);
}

void L2Server::BroadcastToNeighborDist(INT32 ObjectID, FVector *vPos, int Distance, int nSize, PCHAR buff)
{
	typedef void (*f) (UINT64, INT32, INT32, FVector *Pos, int, const char *fmt, int, PCHAR);
	f(0x00403DE0L)(0x0E427AA8L /* not user */, ObjectID, 524288, vPos, Distance, "b", nSize, buff);
};

CObject *L2Server::GetObjectByID(UINT nObjectID)
{
	typedef CObject*(__fastcall *__gobid)(UINT nObjectID);
	__gobid _gobid = (__gobid) 0x006F4920;
	return _gobid(nObjectID);
}
int L2Server::Rnd(int n)
{
	n=rand()%n;
	return n;
}
int L2Server::GetNoblessIndexByDBID(int nDBID)
{
	typedef int( *__GNI)(INT64, int);
	__GNI _GNI = (__GNI) 0x00475E60;
	return _GNI(0x7D55770, nDBID);
}
LPVOID L2Server::New(size_t nSize)
{
	typedef LPVOID( *f)(size_t nSize);
	LPVOID p = f(0x912D80L)(nSize);
	if(p)
	{
		memset(p, 0, nSize);
	}
	return p;
};
void L2Server::Delete(LPVOID pPointer)
{
	typedef LPVOID(*f)(LPVOID);
	f(0x912BC0L)(pPointer);
};

bool L2Server::RequestInvite(int nType, User *pInviter, User *pInvited, int nTimeout)
{
	//Type - 0 - pledge, 2- party, 5 - friend, 6- alliance, 9 - MPCC, 
	typedef bool (*f)(int, User*, User*, int);
	return f(0x0085C9F0L)(nType, pInviter, pInvited, nTimeout);
}

User *L2Server::GetInviter(CObjectSP *pObjectSP, User *pUser)
{
	typedef User* (*f)(CObjectSP*, User*);
	return f(0x0085B740L)(pObjectSP, pUser);
}

void L2Server::ResetInvite(User *pUser)
{
	typedef void (*f)(User*);
	f(0x0085CD30L)(pUser);
};

long int L2Server::GetL2Time()
{
	typedef long int(*f)(INT64);
	return f(0x00646E90L)(0x6F8E3A0L);
}

void L2Server::CallStack(int nType)
{
	typedef void (*f)(int);
	f(0x005B8E30L)(nType);
}

BYTE L2Server::IsValidPointer(LPVOID pPointer)
{
	CBytePointer* pByte = (CBytePointer*)pPointer;
	try
	{
		BYTE mem = pByte->bVar;
		if(mem > 0)
			return mem;
		else 
			return 1;
	}catch(...)
	{
	}
	return 0;
}

void L2Server::SetResidenceOwner(CPledge *pPledge, UINT residenceId)
{
	guard;

	if(CResidence *pResidence = g_ResidenceDB.GetResidence(residenceId))
	{
		if(pResidence->pRD->nPledgeID > 0)
		{
			if(CPledge* pOldOwner = g_PledgeDB.GetPledge(pResidence->pRD->nPledgeID))
			{
				pOldOwner->pSD->nAgitID = 0;
				pOldOwner->pAgit = 0;
				pOldOwner->Save(7);
				pOldOwner->UpdateInfo();
			}else
			{
				g_Log.Add(CLog::Error, "[%s] old owning pledge[%d] is not loaded yet!", __FUNCTION__, pResidence->pRD->nPledgeID);
			}
		}
		pPledge->pSD->nAgitID = pResidence->pRD->nResidenceID;
		pPledge->pAgit = (CAgit*)pResidence;
		pPledge->Save(7);
		pPledge->UpdateInfo();
		pResidence->SetPledge(pPledge);
		g_DB.RequestOwnAgit(pResidence->pRD->nResidenceID, pPledge->pSD->nDBID);
		g_Log.Add(CLog::Blue, "[%s] Pledge[%d] owned residence[%d].", __FUNCTION__, pPledge->pSD->nDBID, pResidence->pRD->nResidenceID);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] cannot find residence[%d]!", __FUNCTION__, residenceId);
	}

	unguard;
}

void L2Server::SetResidenceBattleTime(UINT residenceId, UINT startTime)
{
	guard;
	startTime += time(0);
	if(CResidence *pResidence = g_ResidenceDB.GetResidence(residenceId))
	{
		pResidence->pRD->SiegeDate = startTime;
		pResidence->ChangeResidenceState(1);
		g_DB.RequestSetNextAgitBattle(pResidence);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] cannot find residence[%d]!", __FUNCTION__, residenceId);
	}
	unguard;
}

void L2Server::TimerDispatchFix(PINT64 pPointer, int nVar)
{
	typedef void (*f)(PINT64, int);

	if(L2Server::IsValidPointer(pPointer))
	{
		UINT64 vTable = (*(PINT64)pPointer);
		UINT64 nTimerExpired = (*(PINT64)(vTable+0x98));
		if(vTable != 0x97E3B8)
		{
			if(L2Server::IsValidPointer((LPVOID)nTimerExpired))
			{
				f(0x7F0230)(pPointer, nVar);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] vTable[%p] timerFunc[%p] - REPORT IT TO DEVELOPER PLEASE!", __FUNCTION__, vTable,  nTimerExpired);
			}
		}
	}
	else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid Pointer!", __FUNCTION__); 
	}
};

void EnableNpcPosEvent(const WCHAR* wName)
{
	typedef void (*_f)(const WCHAR*);
	_f(0x790830L)(wName);
}

void DisableNpcPosEvent(const WCHAR* wName)
{
	typedef void (*_f)(const WCHAR*);
	_f(0x790950L)(wName);
}

/* example usage
npc_begin	warrior	18008	[karik_a]	level=70	acquire_exp_rate=1.3	acquire_sp=632	unsowing=0	clan={-1}	ignore_clan_list={}	clan_help_range=300	slot_chest=[]	slot_rhand=[]	slot_lhand=[]	shield_defense_rate=0	shield_defense=0	skill_list={@s_race_demons;@s_increase_p_def14;@s_evil_attack;@s_npc_resist_unholy5;@s_npc_critical_power;@s_npc_critical_chance;@s_npc_resist_bow5}	npc_ai={[karik_a];{[HoldMagic]=@s_npc_hold7}}	category={}	race=demonic	sex=male	undying=0	can_be_attacked=1	corpse_time=7	no_sleep_mode=0	agro_range=1000	ground_high={200;0;0}	ground_low={37;0;0}	exp=429634524	org_hp=2444.5	org_hp_regen=7.5	org_mp=1345.8	org_mp_regen=2.7	collision_radius={25;30}	collision_height={90;109}	str=40	int=21	dex=30	wit=20	con=43	men=20	base_attack_type=fist	base_attack_range=40	base_damage_range={0;0;80;120}	base_rand_dam=30	base_physical_attack=757.75	base_critical=4	physical_hit_modify=4.75	base_attack_speed=253	base_reuse_delay=0	base_magic_attack=517.45	base_defend=295.92	base_magic_defend=179.260985460277	physical_avoid_modify=0	soulshot_count=0	spiritshot_count=0	hit_time_factor=0.45	item_make_list={}	corpse_make_list={}	additional_make_list={}	additional_make_multi_list={{{{[adena];17272;35136;70}};100};{{{[silver_nugget];1;1;37.5}};100};{{{[thread];3;9;30}};100};{{{[metallic_fiber];1;1;24.9999}};100};{{{[mithril_ore];1;1;18.75}};100};{{{[rp_bellion_cestus_i];1;1;0.0642}};100};{{{[sealed_dark_crystal_shield_fragment];1;1;3.1251}};100};{{{[enria];1;1;1.695}};100};{{{[life_stone_level_671];1;1;0.0768}};100};{{{[mid_grade_life_stone_level_671];1;1;0.0129}};100};{{{[sealed_dark_crystal_helmet_design];1;1;2.586}};100};{{{[sealed_tallum_bonnet_design];1;1;2.5641}};100};{{{[sealed_dark_crystal_helmet];1;1;0.0129}};100};{{{[sealed_dark_crystal_shield];1;1;0.0384}};100}}	hp_increase=0	mp_increase=0	safe_height=100	npc_end

vector<pair<wstring, wstring>> aiAparams;	//container for ai params
aiParams.push_back(pair<wstring, wstring>(L"HoldMagic", L"1036039"));	//adding HoldMagic skill param (skill pair have to be from skill_pch.txt it wont recognize the name)
SpawnNpc(1018008, aiParams, 12440, 10000, 4332, 1200);	//spawning the npc

*/
void SpawnNpc(UINT npcClassId, vector<pair<wstring, wstring>> aiParams, int x, int y, int z, UINT heading)
{
	const WCHAR* wszName = g_ObjectDB.GetName(npcClassId);
	CNPC *pNPC = g_ObjectDB.CreateNPC(npcClassId);
	if(pNPC)
	{
		const WCHAR* wszAI = g_ObjectDB.GetAI(npcClassId);
		if(wszAI)
		{
			//ai params
			CHAR aiParamsBuff[4096];
			int aiParamsLen = 0;
			int aiParamsCount = 0;
			memset(aiParamsBuff, 0, sizeof(aiParamsBuff));
			for(UINT n=0;n<aiParams.size();n++)
			{
				aiParamsLen += Assemble(&aiParamsBuff[aiParamsLen], 4096 - aiParamsLen, "SS", aiParams[n].first.c_str(), aiParams[n].second.c_str());
				aiParamsCount++;
			}

			//Fill data with correct info
			CHAR buff[8190];
			memset(buff, 0, 8190);

			int buffSize = Assemble(buff, 8190,"cddpddddddddSdb", 0x01, pNPC->pSD->nServerIndex, pNPC->nObjectID, 
				0, 0, x, y, z,
				heading, 0, 0, 0, wszAI, aiParamsCount, aiParamsLen, aiParamsBuff);
			NpcServer::Send(buffSize, buff);
		}else
			g_Log.Add(CLog::Error, "[%s] There is no AI specified for npc with class id[%d]", __FUNCTION__, npcClassId);
	}else
		g_Log.Add(CLog::Error, "[%s] There is no npc with class id[%d]", __FUNCTION__, npcClassId);

}
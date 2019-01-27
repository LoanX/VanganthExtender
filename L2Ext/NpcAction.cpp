#include "StdAfx.h"
#include "NpcAction.h"
#include "HerbSystem.h"
#include "CursedWeaponSystem.h"
#include "ClanSys.h"
#include "TvT.h"
#include "AutoLoot.h"
#include "DropList.h"
#include "FractionSystem.h"
#include "SubStack.h"
#include "DB.h"
#include "AutoAnnounce.h"
#include "ChampionNpc.h"
#include "LureProtection.h"
#include "VIPSystem.h"

CCreature* g_NpcKiller[26];
CNPC* g_KilledNpc[26];

vector<wstring> g_EnabledEventNpc;
vector<wstring> g_DisabledEventNpc;
vector<INT32> g_EpicNpcs;

void NpcAction::Initialize()
{
	for(UINT n=0;n<26;n++)
	{
		g_NpcKiller[n] = 0;
		g_KilledNpc[n] = 0;
	}

	const TCHAR *npcMod = _T("NpcMod");
	TCHAR temp[0x4000] = { 0 };
	if(GetPrivateProfileString(npcMod, _T("EpicNpcList"), 0, temp, 0x4000, g_ConfigFile))
	{
		wstringstream npcStream(temp);
		UINT id = 0;
		while(npcStream >> id)
		{
			g_EpicNpcs.push_back(id);
		}
	}else
	{
		//default list
/*		g_EpicNpcs.push_back(1029001);	//queen ant
		g_EpicNpcs.push_back(1029014);	//orfen
		g_EpicNpcs.push_back(1029022);	//zaken
		g_EpicNpcs.push_back(1029020);	//baium
		g_EpicNpcs.push_back(1029019);	//antaras
		g_EpicNpcs.push_back(1029066);	//antaras
		g_EpicNpcs.push_back(1029067);	//antaras
		g_EpicNpcs.push_back(1029068);	//antaras
		g_EpicNpcs.push_back(1029028);	//valakas
		g_EpicNpcs.push_back(1029046);	//follower of frintezza
		g_EpicNpcs.push_back(1029047);	//follower of frintezza 2
*/
	}


	g_HookManager.WriteCall( 0x74D30D, NpcAction::OnDie, 0); //CSummon::Die
	g_HookManager.WriteMemoryDWORD( 0x989520, (DWORD)NpcAction::OnDie);
	g_HookManager.WriteMemoryDWORD( 0xA1B160, (DWORD)NpcAction::OnDie);
	g_HookManager.WriteMemoryDWORD( 0xA28F80, (DWORD)NpcAction::OnDie);
	g_HookManager.WriteMemoryDWORD( 0xAA0F70, (DWORD)NpcAction::OnDie);

	g_HookManager.WriteCall( 0x6F1E1A, NpcAction::EnterWorld, 0);

	g_HookManager.WriteCall( 0x6C438C, NpcAction::OnNpcDieAddAdenaToInventory, 0);
	g_HookManager.WriteCall( 0x6C4792, NpcAction::OnNpcDieAddAdenaToInventory, 0);
	g_HookManager.WriteCall( 0x6C4BD4, NpcAction::OnNpcDieAddAdenaToInventory, 0);

	g_HookManager.WriteCall( 0x6C3CFA, NpcAction::OnNpcDieAddEventToInventory, 0);
	g_HookManager.WriteCall( 0x6C3DC9, NpcAction::OnNpcDieAddEventToInventory, 0);
	g_HookManager.WriteCall( 0x6C3E8D, NpcAction::OnNpcDieAddEventToInventory, 0);
	g_HookManager.WriteCall( 0x6C3F44, NpcAction::OnNpcDieAddEventToInventory, 0);
	g_HookManager.WriteCall( 0x6C3FF5, NpcAction::OnNpcDieAddEventToInventory, 0);
	g_HookManager.WriteCall( 0x6C4096, NpcAction::OnNpcDieAddEventToInventory, 0);
	g_HookManager.WriteCall( 0x6C4123, NpcAction::OnNpcDieAddEventToInventory, 0);
	g_HookManager.WriteCall( 0x6C41DF, NpcAction::OnNpcDieAddEventToInventory, 0);
	g_HookManager.WriteCall( 0x6C4464, NpcAction::OnNpcDieAddItemToInventory, 0);

	g_HookManager.WriteMemoryDWORD(0x989500, (DWORD)NpcAction::LeaveWorld);
	g_HookManager.WriteMemoryDWORD(0xA1B140, (DWORD)NpcAction::LeaveWorld);
	g_HookManager.WriteMemoryDWORD(0xA28F60, (DWORD)NpcAction::LeaveWorld);
	g_HookManager.WriteMemoryDWORD(0xA4AC90, (DWORD)NpcAction::LeaveWorld);
	g_HookManager.WriteMemoryDWORD(0xA4B600, (DWORD)NpcAction::LeaveWorld);
	g_HookManager.WriteMemoryDWORD(0xAA0F50, (DWORD)NpcAction::LeaveWorld);

	g_HookManager.WriteCall( 0x6D4F2D, NpcAction::OnNpcClassChange, 4);

	g_HookManager.WriteCall( 0x6D7C9D, NpcAction::OnGetClassIdFromNameInNpcCreatePacket);

	g_HookManager.WriteCall( 0x74CD17, NpcAction::OnTimerExpired);

	g_HookManager.WriteMemoryDWORD(0x988FF0, (UINT)NpcAction::OnTimerExpired);
	g_HookManager.WriteMemoryDWORD(0xA1AC30, (UINT)NpcAction::OnTimerExpired);
	g_HookManager.WriteMemoryDWORD(0xA28A50, (UINT)NpcAction::OnTimerExpired);
	g_HookManager.WriteMemoryDWORD(0xAA0A40, (UINT)NpcAction::OnTimerExpired);

}

void NpcAction::OnTimerExpired(CNPC *pNpc, INT32 id)
{
	CTL;

	if(pNpc->IsBoss() || pNpc->IsZZoldagu())
	{
		g_LureProtection.Handle(pNpc);
	}

	typedef void(*f)(CNPC*, INT32);
	f(0x6C5E60L)(pNpc, id);
}

INT32 NpcAction::OnGetClassIdFromNameInNpcCreatePacket(UINT64 instance, const WCHAR* wNpcName)
{
	typedef INT32 (*f)(UINT64, const WCHAR*);
	INT32 classId = f(0x6FB7F0L)(instance, wNpcName);
	if(classId > 0)
	{
		if(g_ChampionNpc.IsEnabled())
		{
			classId = g_ChampionNpc.GetNpcClassId(classId);
		}
	}

	return classId;
}

bool NpcAction::OnNpcClassChange(CCreature *pCreature, UINT classId)
{
	guard;

	UINT64 addr = (*(PUINT64)pCreature);
	addr += 0x310;
	addr = (*(PUINT64)addr);

	if(g_SubStack.IsEnabled())
	{
		if(User *pUser = pCreature->SafeCastUser())
		{
			SubStackInfo& ssi = pUser->pED->subStackInfo;
			if(ssi.classId[pUser->pSD->nSubClassIndex][0] != UINT_MAX)
			{
				if(ssi.classId[pUser->pSD->nSubClassIndex][0] == pUser->pSD->nClass)
				{
					ssi.classId[pUser->pSD->nSubClassIndex][0] = classId;
				}else
				{
					ssi.classId[pUser->pSD->nSubClassIndex][1] = classId;
				}
				g_DB.RequestSaveSubStack(pUser->nDBID, pUser->pSD->nSubClassIndex, ssi.classId[pUser->pSD->nSubClassIndex][0], ssi.classId[pUser->pSD->nSubClassIndex][1]);
			}
		}
	}

	typedef bool (*f)(CCreature*, UINT);
	bool ret = f((PVOID)addr)(pCreature, classId);

	unguard;
	return ret;
}

void NpcAction::LeaveWorld(CNPC *pNPC)
{
	guard;

	if(pNPC->ValidCreature())
	{
		//do your stuff here ;)
	}

	typedef void(*f)(CNPC*);
	f(0x6C0AD0L)(pNPC);	//CNPC::LeaveWorld

	unguard;
}

bool NpcAction::OnNpcDieAddEventToInventory(CNPC *pNpc, int itemId, int count, bool isPick, int param2, int param3, int param4)
{
	typedef bool (*f)(CNPC*, int, int, bool, int, int, int);

	UINT threadIndex = GetThreadIndex();
	if(threadIndex < 26)
	{
		CCreature *pKiller = g_NpcKiller[threadIndex];
		User *pUser = pKiller->GetUser();
		if(pKiller->IsSummon())
		{
			pUser = pKiller->CastSummon()->GetMaster();
		}
		if(pUser->ValidUser())
		{
			if(g_AutoLoot.IsEnabled())
			{
				if(g_AutoLoot.Handle(pNpc, pKiller, itemId, count))
				{
					return true;
				}
			}
		}
	}

	return f(0x4EF130L)(pNpc, itemId, count, isPick, param2, param3, param4);
}

bool NpcAction::OnNpcDieAddItemToInventory(CNPC *pNpc, int itemId, int count, bool isPick, int param2, int param3, int param4)
{
	typedef bool (*f)(CNPC*, int, int, bool, int, int, int);

	if(g_DropList.IsEnabled())
	{
		return true;
	}

	UINT threadIndex = GetThreadIndex();
	if(threadIndex < 26)
	{
		CCreature *pKiller = g_NpcKiller[threadIndex];
		if(g_AutoLoot.IsEnabled())
		{
			if(g_AutoLoot.Handle(pNpc, pKiller, itemId, count))
			{
				return true;
			}
		}		
	}

	return f(0x4EF130L)(pNpc, itemId, count, isPick, param2, param3, param4);
}

bool NpcAction::OnNpcDieAddAdenaToInventory(CNPC *pNpc, int itemId, int count, bool isPick, int param2, int param3, int param4)
{
	typedef bool (*f)(CNPC*, int, int, bool, int, int, int);

	if(g_DropList.IsEnabled())
	{
		return true;
	}

	UINT threadIndex = GetThreadIndex();
	if(threadIndex < 26)
	{
		CCreature *pKiller = g_NpcKiller[threadIndex];
		User *pUser = pKiller->GetUser();
		if(pKiller->IsSummon())
		{
			pUser = pKiller->CastSummon()->GetMaster();
		}
		if(pUser->ValidUser())
		{
			if(itemId == 57 || itemId == 6360 || itemId == 6361 || itemId == 6362)
			{
				double adenaCount = static_cast<double>(count);
				adenaCount *= pUser->GetAdenaBonus();
				adenaCount *= g_VIPSystem.GetAdenaRate(pUser);
			//	g_Log.Add(CLog::Blue, "[%s] adena [%d] vs adena with bonus[%f]", __FUNCTION__, count, adenaCount);
				count = static_cast<int>(adenaCount);
			}

			if(g_AutoLoot.IsEnabled())
			{
				if(g_AutoLoot.Handle(pNpc, pKiller, itemId, count))
				{
					return true;
				}
			}
		}
	}

	return f(0x4EF130L)(pNpc, itemId, count, isPick, param2, param3, param4);
}

void NpcAction::EnterWorld(CNPC* pNPC, bool bSetDefaultParam, int nHP, int nMP)
{
	if(pNPC->ValidCreature())
	{
		//Do your stuff here
		int npcId = pNPC->pSD->nNpcClassID;
		for(UINT n=0;n<g_EpicNpcs.size();n++)
		{
			if(g_EpicNpcs[n] == npcId)
			{
				pNPC->pSD->nIsEpicNpc = TRUE;
			//	g_Log.Add(CLog::Blue, "[%s] Setting epic npc flag for npc[%S][%d]", __FUNCTION__, pNPC->pSD->wszName, npcId);
				break;
			}
		}

		pNPC->pSD->spawnPos.x = static_cast<INT32>(pNPC->pSD->Pos.x);
		pNPC->pSD->spawnPos.y = static_cast<INT32>(pNPC->pSD->Pos.y);
		pNPC->pSD->spawnPos.z = static_cast<INT32>(pNPC->pSD->Pos.z);

		if(pNPC->IsBoss())
		{
			g_AutoAnnounce.OnBossEnterWorld(npcId);
		}
	}

	typedef void (*f)(CNPC*, bool, int, int);
	f(0x6C67C0L)(pNPC, bSetDefaultParam, nHP, nMP);
}

bool NpcAction::OnDie(CNPC *pNPC, CCreature *pEnemy)
{
	typedef bool(*f)(CCreature*, CCreature*);
	
	UINT threadIdx = GetThreadIndex();
	if(threadIdx < 26)
	{
		g_NpcKiller[threadIdx] = pEnemy;
		g_KilledNpc[threadIdx] = pNPC;
	}

	if(pNPC->ValidCreature())
	{
		if(!pNPC->npcKilledByGM)
		{
			g_CursedWeaponSystem.OnNpcDie(pNPC, pEnemy);
			g_HerbSystem.DropHerb(pNPC, pEnemy);
			g_NpcMaker.OnNpcDie(pNPC);
			ClanSys::OnNpcDie(pNPC, pEnemy);
			g_DropList.OnDie(pNPC, pEnemy);
			g_FractionSystem.OnDie(pNPC, pEnemy);
		}else
		{
			g_NpcMaker.OnNpcDie(pNPC, true);
		}
		
	}

	bool ret = f(0x6C38B0L)(pNPC, pEnemy);

	if(threadIdx < 26)
	{
		g_NpcKiller[threadIdx] = 0;
		g_KilledNpc[threadIdx] = 0;
	}

	return ret;
}
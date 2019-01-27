#include "StdAfx.h"
#include "Exploits.h"
#include "DB.h"
#include "TerritoryData.h"
#include "DoorData.h"

void CAntiExploit::Init()
{
	g_HookManager.WriteCall( 0x83829B, CAntiExploit::User_Ride, 0);
	g_HookManager.WriteCall( 0x88FCDE, CAntiExploit::User_Ride, 0);
	g_HookManager.WriteCall( 0x83837B, CAntiExploit::User_DespawnSummon, 0);
//	g_HookManager.WriteCall( 0x854F18, CAntiExploit::User_OnChangeSubjobEnd, 0);
	g_HookManager.WriteCall( 0x5F7F1A, CAntiExploit::GetHTMLFile, 0);
	g_HookManager.WriteCall( 0x5F7F61, CAntiExploit::GetHTMLFile, 0);
	g_HookManager.WriteCall( 0x5F7FB3, CAntiExploit::GetHTMLFile, 0);
}


const WCHAR* CAntiExploit::GetHTMLFile(LPVOID pInstance, const WCHAR* wFileName)
{
	typedef const WCHAR*(*f)(LPVOID, const WCHAR*);
	try
	{
		if(wFileName)
		{
			//check file name length
			size_t htmlLen = wcslen(wFileName);

			UINT nameLen = 0;
			WCHAR lastChar = 0;
			bool validName = false;
			const WCHAR* invalidChars = L"{}`'\",/\\!@#%^&*()";
			bool foundInvalidChar = false;

			for(UINT n=0;n<htmlLen;n++)
			{
				if(wFileName[n] == 0)
				{
					break;
				}

				if(lastChar == L'.' && wFileName[n] == L'.')
				{
					nameLen = 260;
					break;
				}
				for(UINT c=0;c<17;c++)
				{
					if(wFileName[n] == invalidChars[c])
					{
						foundInvalidChar = true;
						break;
					}
				}

				lastChar = wFileName[n];
				nameLen++;
			}

			if(foundInvalidChar == false && nameLen > 4 && nameLen < 255)
			{
				//check for .htm or .html at the end of name
				if(nameLen > 5)
				{
					if((wFileName[nameLen-1] == L'l' || wFileName[nameLen-1] == L'L') && (wFileName[nameLen-2] == L'm' || wFileName[nameLen-2] == L'M') && (wFileName[nameLen-3] == L't' || wFileName[nameLen-3] == L'T') && (wFileName[nameLen-4] == L'h' || wFileName[nameLen-4] == L'H') && wFileName[nameLen-5] == L'.')
					{
						validName = true;
					}
				}
				if(!validName)
				{
					if((wFileName[nameLen-1] == L'm' || wFileName[nameLen-1] == L'M') && (wFileName[nameLen-2] == L't' || wFileName[nameLen-2] == L'T') && (wFileName[nameLen-3] == L'h' || wFileName[nameLen-3] == L'H') && wFileName[nameLen-4] == L'.')
					{
						validName = true;
					}
				}
			}

			if(validName)
			{
				return f(0x5F7190L)(pInstance, wFileName);
			}else
			{
				if(wFileName)
				{
					g_Log.Add(CLog::Error, "[%s] Invalid file name [%S]!", __FUNCTION__, wFileName);
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Invalid file name!", __FUNCTION__);
				}
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	return f(0x5F7190L)(pInstance, 0);
}

void CAntiExploit::User_Ride(User *pUser, CPet *pPet)
{
	if(pUser->ValidUser())
	{
		if(pUser->IsNowTrade())
			pUser->TradeCancel();
		
		pUser->pSocket->pED->itemActionTimestamp = GetTickCount() + 1000;

		if(!pUser->pSD->nCursedOwner)
		{
			if(pUser->pSD->pExData->territoryData.territoryId[FORBIDDEN_RIDE_ZONE] == 0)
			{
				//door data check
				bool canRide = true;

				if(g_DoorData.IsEnabled() && g_DoorData.GetBlockRideRange() > 0)
				{
					vector<DoorInfo*> doors;
					g_DoorData.GatherNearestDoors(pUser->pSD->Pos, doors, g_DoorData.GetBlockRideRange());
					if(doors.size() > 0)
					{
						canRide = false;
					}
				}
				if(canRide)
				{
					pUser->Ride(pPet);
				}else
				{
					pUser->pSocket->SendSystemMessage(L"You're too close to the doors!");
				}
			}else
			{
				pUser->pSocket->SendSystemMessage(L"You're not allowed to ride strider here!");
			}
		}else
			pUser->pSocket->SendSystemMessage(L"You cannot mount a pet while using cursed weapon.");
	}
}

void CAntiExploit::User_DespawnSummon(User *pUser)
{
	bool bCombat = pUser->InCombat();
	if(bCombat)
	{
		DWORD dCombatTime = pUser->CombatTime();
		DWORD dCurrentTime = (int) time(NULL);
		if(dCombatTime != 0 && (dCombatTime+30) >  dCurrentTime)
		{
			pUser->GetSocket()->SendSystemMessage(L"You cannot despawn your summon during combat.");
		}else
			pUser->DespawnSummon();
	}else
		pUser->DespawnSummon();
}

void CAntiExploit::User_OnChangeSubjobEnd(INT64 nCached, User *pUser)
{
	g_DB.RequestSaveCharacterForSubJob(pUser);
	
	typedef void (*f)(UINT64, User*, bool, bool);
	f(0x0053A250L)(0x10488E0L, pUser, true, false);	//CDB::SendSaveCharacterInfo

	pUser->SaveInfectedSkills();
}

bool CAntiExploit::ValidateStats(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		if(pCreature->pSD->nSTR < 1 || pCreature->pSD->nSTR > 99)
		{
			g_Log.Add(CLog::Error, "[%s] Invalid nSTR[%d][%S]", __FUNCTION__, pCreature->pSD->nSTR, pCreature->pSD->wszName);
			return true;
		}else if(pCreature->pSD->nINT < 1 || pCreature->pSD->nINT > 99)
		{
			g_Log.Add(CLog::Error, "[%s] Invalid nINT[%d][%S]", __FUNCTION__, pCreature->pSD->nINT, pCreature->pSD->wszName);
			return true;
		}else if(pCreature->pSD->nDEX < 1 || pCreature->pSD->nDEX > 99)
		{
			g_Log.Add(CLog::Error, "[%s] Invalid nDEX[%d][%S]", __FUNCTION__, pCreature->pSD->nDEX, pCreature->pSD->wszName);
			return true;
		}else if(pCreature->pSD->nCON < 1 || pCreature->pSD->nCON > 99)
		{
			g_Log.Add(CLog::Error, "[%s] Invalid nCON[%d][%S]", __FUNCTION__, pCreature->pSD->nCON, pCreature->pSD->wszName);
			return true;
		}else if(pCreature->pSD->nMEN < 1 || pCreature->pSD->nMEN > 99)
		{
			g_Log.Add(CLog::Error, "[%s] Invalid nMEN[%d][%S]", __FUNCTION__, pCreature->pSD->nMEN, pCreature->pSD->wszName);
			return true;
		}else if(pCreature->pSD->nWIT < 1 || pCreature->pSD->nWIT > 99)
		{
			g_Log.Add(CLog::Error, "[%s] Invalid nWIT[%d][%S]", __FUNCTION__, pCreature->pSD->nWIT, pCreature->pSD->wszName);
			return true;
		}

		if(pCreature->pSD->fHP < 0)
		{
			if(pCreature->pSD->nIsPC)
				pCreature->pSD->fHP = 0;
		}
		if(pCreature->pSD->fCP < 0)
		{
			if(pCreature->pSD->nIsPC)
				pCreature->pSD->fCP = 0;
		}
		if(pCreature->pSD->fMP < 0)
		{
			if(pCreature->pSD->nIsPC)
				pCreature->pSD->fMP = 0;
		}

		if(pCreature->pSD->fHP > pCreature->pSD->MaxHP)
			pCreature->pSD->fHP = pCreature->pSD->MaxHP;
		if(pCreature->pSD->fMP > pCreature->pSD->MaxMP)
			pCreature->pSD->fMP = pCreature->pSD->MaxMP;
		if(pCreature->pSD->fCP > pCreature->pSD->MaxCP)
			pCreature->pSD->fCP = pCreature->pSD->MaxCP;
	}
	return false;
}
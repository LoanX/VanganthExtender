#include "StdAfx.h"
#include "TerritoryData.h"
#include "SpawnProtection.h"

void CCreatureTimerHandler::AddTimer(int ID, int Time)
{
	Lock();
	bool bNew = true;
	for(list<CTimerData>::iterator Iter = lData.begin();Iter!=lData.end();Iter++)
	{
		if(Iter->GetID() == ID)
		{
			Iter->Set(ID, Time);
			bNew = false;
			break;
		}
	}
	if(bNew)
	{
		CTimerData Data(ID, Time);
		lData.push_back(Data);
	}
	Unlock();
}

void CCreatureTimerHandler::DelTimer(int ID)
{
	Lock();
	try
	{
		for(list<CTimerData>::iterator Iter = lData.begin();Iter!=lData.end();Iter++)
		{
			if(Iter->GetID() == ID)
			{
				lData.erase(Iter);
				break;
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	Unlock();
}

void CCreatureTimerHandler::TimerExpired(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		try
		{
			vector<int> vExpired;

			Lock();
			for(list<CTimerData>::iterator Iter = lData.begin();Iter!=lData.end();Iter++)
			{
				if(Iter->IsExpired())
				{
					vExpired.push_back(Iter->GetID());
				}
			}
			Unlock();
			
			for(INT32 n=0;n<vExpired.size();n++)
			{
				DelTimer(vExpired[n]);
				Handle(vExpired[n], pCreature);
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
		}
	}
}

void CCreatureTimerHandler::Handle(int ID, CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		if(User *pUser = pCreature->SafeCastUser())
		{
			if(pUser->pED->leftWorld)
			{
				return;
			}

		}
		try
		{
			switch(ID)
			{
			case VALIDATE_STATS:
				{
					HandleValidateStat(pCreature);
					break;
				}
			case USER_SPAWN_PROTECTION:
				{
					HandleUserSpawnProtection(pCreature);
					break;
				}
			case SUMMON_BETRAY:
				{
					HandleSummonBetray(pCreature);
					break;
				}
			case USER_SEND_ITEM_LIST:
				{
					HandleUserSendItemList(pCreature);
					break;
				}
			case USER_DISMOUNT_WYVERN:
				{
					HandleUserDismountWyvern(pCreature);
					break;
				}
			case CRASH:
				{
					HandleCrash(pCreature);
					break;
				}
			case USER_SEND_SPIRIT_STATUS:
				{
					HandleUserSendSpiritStatus(pCreature);
					break;
				}
			case USER_SEND_ERA_MOON:
				{
					HandleUserSendEraMoon(pCreature);
					break;
				}
			case DISPEL_AUGMENTATION:
				{
					HandleUserDispelAugmentation(pCreature);
					break;
				}
			case USER_VALIDATE_ACQUIRED_SKILLS:
				{
					HandleUserValidateAcquiredSkills(pCreature);
					break;
				}
			case VALIDATE_ITEM_REUSE:
				{
					HandleValidateItemReuse(pCreature);
					break;
				}
			case USER_RESET_PVP_FLAG:
				{
					HandleUserResetPvPFlag(pCreature);
					break;
				}
			default:
				{
					if(ID < USER_SHADOW_ITEM_START)
					{
						HandleUserShadowItem(pCreature, ID);
						break;
					}
					g_Log.Add(CLog::Error, "[%s] Not Handled Timer ID[%d]", __FUNCTION__, ID);
					break;
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception [%d] switch.", __FUNCTION__, ID);
		}
	}
}

void CCreatureTimerHandler::HandleValidateStat(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		pCreature->ValidateParameters();
	}
}

void CCreatureTimerHandler::HandleUserValidateAcquiredSkills(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		pUser->ValidateAcquiredSkills();
	}
}

void CCreatureTimerHandler::HandleUserShadowItem(CCreature *pCreature, int ID)
{
	if(User *pUser = pCreature->GetUser())
	{
		CShadowItem::TimerExpired(pUser, ID);
	}
}

void CCreatureTimerHandler::HandleUserDispelAugmentation(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		pUser->DispelByAbnormal(177, true);
	}
}

void CCreatureTimerHandler::HandleUserSendEraMoon(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		g_EraSystem.OnEnterWorld(pUser);
	}
}

void CCreatureTimerHandler::HandleCrash(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		LPINT p = NULL;
		(*p) = 123409876;
	}
}

void CCreatureTimerHandler::HandleUserSendSpiritStatus(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		g_SpiritSystem.UpdateClientStatus(pUser);
	}
}

void CCreatureTimerHandler::HandleUserDismountWyvern(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		int nYongmaType = pUser->pSD->nYongmaType;
		if( nYongmaType == 2) //Check for Fly Zone
		{
			if(pUser->pSD->pExData->territoryData.territoryId[FORBIDDEN_FLY_ZONE] != 0)
			{
				if(pUser->pSD->nGetOffTimer)
				{
					pUser->GetOff(true);
				}
			}
			pUser->pSD->nGetOffTimer = 0;
		}
	}
}

void CCreatureTimerHandler::HandleUserSendItemList(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		pUser->SendItemList();
	}
}

void CCreatureTimerHandler::HandleSummonBetray(CCreature *pCreature)
{
	if(pCreature->IsSummon() && pCreature->pSD->pExData)
	{
		if(pCreature->pSD->pExData->SummonBetrayData.Betraying)
		{
			CSummon *pSummon = pCreature->CastSummon();
			User *pMaster = pSummon->GetMaster();
			if(pMaster->ValidUser())
			{
				pSummon->WriteLock(__FILEW__, __LINE__);
				pSummon->Action(pMaster, 1, 0);
				pSummon->Action(pMaster, 1, 0);
				pSummon->WriteUnlock();
				AddTimer(SUMMON_BETRAY, 1);
				pSummon->pSD->pExData->SummonBetrayData.ActiveTimer = true;
			}
		}else
		{
			pCreature->pSD->pExData->SummonBetrayData.ActiveTimer = false;
			pCreature->pCreatureController->AsyncDoNothing();
		}
	}	
}

void CCreatureTimerHandler::HandleUserSpawnProtection(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pED && !pUser->pED->spawnProtection)
		{
			g_SpawnProtection.ActivateProtection(pUser);
		}
	}
}

void CCreatureTimerHandler::HandleValidateItemReuse(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		g_ItemReuseManager.OnEnterWorld(pUser);
	}
}

void CCreatureTimerHandler::HandleUserResetPvPFlag(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		pUser->SetGuilty(false);
		pUser->pSD->nPvPAttackTimeStamp = 0;
		pUser->SetCharInfoChanged();
		pUser->SetUserInfoChanged();
		pUser->SendRelationChanged();
	}
}
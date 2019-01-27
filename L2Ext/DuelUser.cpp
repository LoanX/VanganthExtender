#include "StdAfx.h"
#include "DuelUser.h"
#include "CreatureAction.h"
#include "DuelSystem.h"

CDuelUser::CDuelUser(User *pUser, int nTeam)
{
	if(pUser->ValidUser())
	{
		Blocked = false;
		Team = nTeam;
		pUser->lpSharedDataLock->Lock(__FILEW__, __LINE__);
		pUserO = pUser;
		userServerId = pUser->nObjectID;
		Stat[HP] = pUser->pSD->fHP;
		Stat[MP] = pUser->pSD->fMP;
		Stat[CP] = pUser->pSD->fCP;
		Position = pUser->pSD->Pos;
		pUser->lpSharedDataLock->Unlock();
		pUser->abnormalStatusLock.Lock(__FILEW__, __LINE__);
		memcpy(AbnormalTable, pUser->pSD->AbnormalTable, sizeof(AbnormalTable));
		AbnormalDataVector::iterator Iter = pUser->vAbnormalStatus.begin();
		for(;Iter!=pUser->vAbnormalStatus.end();Iter++)
		{
			vBuff.push_back(*Iter);
		}
		pUser->abnormalStatusLock.Unlock();
		
		if(g_DuelSystem.IsDispelAbnormals())
			pUser->DispelAllByGM();

		pUser->pCreatureController->AsyncDoNothing();
	}
}

void CDuelUser::StartFight()
{
	User *pUser = pUserO;
	if(pUser->ValidUser())
	{
		pUser->pSD->DuelInfo.SetFighting(1);
		pUser->SendCharInfo(false);
		pUser->SendUserInfo(false);
	}else
		g_Log.Add(CLog::Error, "[%s] User cannot be found [%d]", __FUNCTION__, userServerId);
}

void CDuelUser::RestoreData()
{
	guard;

	User *pUser = pUserO;
	if(pUser->ValidUser())
	{
		//CP, HP, MP
		pUser->lpSharedDataLock->Lock(__FILEW__, __LINE__);
		pUser->pSD->fCP = Stat[CP];
		pUser->pSD->fHP = Stat[HP];
		pUser->pSD->fMP = Stat[MP];
		pUser->lpSharedDataLock->Unlock();
		//Buff List
		pUser->DispelAllByGM();
		pUser->abnormalStatusLock.Lock(__FILEW__, __LINE__);
		AbnormalDataVector::iterator Iter = vBuff.begin();
		for(;Iter!=vBuff.end();Iter++)
		{
			pUser->vAbnormalStatus.push_back(*Iter);
			if(Iter->pSI->nAbnormalType > 63 &&  Iter->pSI->nAbnormalType < 78)
			{
				//song
				pUser->songSlotsUsed++;

			}else if(Iter->pSI->nAbnormalType > 77 && Iter->pSI->nAbnormalType < 91)
			{
				//dance
				pUser->danceSlotsUsed++;
			}
		}
		//Abnormal Table
		memcpy(pUser->pSD->AbnormalTable, AbnormalTable , sizeof(AbnormalTable));
		pUser->abnormalStatusLock.Unlock();
		pUser->ValidateParameters();
		pUser->CheckAbnormalVisualEffect();
	}

	unguard;
}

void CDuelUser::Block()
{
	guard;
	User *pUser = pUserO;
	if(pUser->ValidUser())
	{
		//4515 lvl 1
		Blocked = true;
		CSkillInfo *pSI = g_SkillDB.GetSkillInfo(4515, 1);
		if(pSI)
		{
			CreatureAction::OnInfectedSkillBy(pUser, pUser, pSI);
		}
	}
	unguard;
}

void CDuelUser::Send(PCHAR Buff, int Size)
{
	User *pUser = pUserO;
	if(pUser->ValidUser())
	{
		CSocket *pSocket = pUser->GetSocket();
		if(pSocket->ValidUserSocket())
		{
			pSocket->Send("b", Size, Buff);
		}
	}
}

void CDuelUser::DoSocialAction(SocialActionType Action)
{
	User *pUser = pUserO;
	if(pUser->ValidUser())
	{
		pUser->SocialAction(Action);
	}
}

void CDuelUser::DoEndJob()
{
	guard;

	User *pUser = pUserO;
	if(pUser->ValidUser())
	{
		pUser->pSD->DuelInfo.Clear();
		pUser->pSD->nPvPAttackTimeStamp = 0;
		RestoreData();
		CHAR Buff[128];
		int nSize = Assemble(Buff, 128, "chd", EX_PACKET, DUEL_END, 0);
		pUser->GetSocket()->Send("b", nSize, Buff);
		pUser->pCreatureController->AsyncDoNothing();
		pUser->StopSummon();
		pUser->SetGuilty(false);
		pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::USER_RESET_PVP_FLAG, 0);
	}else
		g_Log.Add(CLog::Error, "[%s] User cannot be found [%d]", __FUNCTION__, userServerId);

	unguard;
}
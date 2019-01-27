#include "StdAfx.h"
#include "DB.h"

void CKillDeathStat::OnUserDie(User *pUser, CCreature *pEnemy)
{
	User *pKiller = NULL;
	if(pEnemy->ValidUser())
		pKiller = pEnemy->GetUser();
	if(pEnemy->IsSummon() || pEnemy->IsPet())
	{
		pKiller = pEnemy->CastSummon()->GetMaster();
	}
	if(pKiller->ValidUser())
	{
		int DeadCharID = pUser->nDBID;
		int DeadClassID = pUser->pSD->nClass;
		int DeadLevel = pUser->pSD->nLevel;
		int KillerCharID = pKiller->nDBID;
		int KillerClassID = pKiller->pSD->nClass;
		int KillerLevel = pKiller->pSD->nLevel;
		int KillerHP = (int)pKiller->pSD->fHP;
		int KillerMaxHP = (int)pKiller->pSD->MaxHP;
		int KillerCP = (int)pKiller->pSD->fCP;
		int KillerMaxCP = (int)pKiller->pSD->MaxCP;
		PVP_STATE PvP = PK;
		int SkillID = pKiller->pSD->pExData->CastedSkill.skill_id;
		int SkillLevel = (int)pKiller->pSD->pExData->CastedSkill.level;
		if(SkillID == 0)
			SkillLevel = 0;
		if(pUser->pSD->bGuilty)
			PvP = PVP;
		CPledge *pPledge1 = pUser->GetPledge();
		CPledge *pPledge2 = pKiller->GetPledge();
		if(pPledge1 && pPledge1->pSD && pPledge2 && pPledge2->pSD)
		{
			if(pPledge1->IsInWarWithEx(pPledge2->nObjectID))
				PvP = CLAN_WAR;
		}
		if(PvP == 0)
		{
			if(pUser->pSD->nInBattleField)
				PvP = PVP;
			else if(pUser->pSD->nInCustomPvpZone)
				PvP = PVP;
			else if(pUser->pSD->nKarma)
				PvP = KARMA;
			if(PvP == KARMA && pKiller->pSD->nKarma)
				PvP = BOTH_PK;
		}

		IVector Location(pUser->pSD->Pos);
		g_DB.RequestSaveKillDeathStat(DeadCharID, DeadClassID, DeadLevel, KillerCharID, KillerClassID, KillerLevel, KillerHP, KillerMaxHP, KillerCP, KillerMaxCP, Location.x, Location.y, Location.z, PvP, SkillID, SkillLevel);
	}
};
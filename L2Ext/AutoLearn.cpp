#include "StdAfx.h"
#include "AutoLearn.h"
#include "DB.h"
#include "SkillAcquireDB.h"

map<int, int> mSkillMastery;
BOOL stackArmorMastery;
BOOL stackWeaponMastery;
BOOL disableAutoLearnOnEnterWorld;
list<int> lIgnoredSkills;
INT32 g_AllowedSkillLevelDifference = 9;
BOOL bAutoLearn = FALSE;

int GetMasteryType(int skillId)
{
	map<int, int>::iterator Iter = mSkillMastery.find(skillId);
	if(Iter!=mSkillMastery.end())
	{
		if(Iter->second == MASTERY_WEAPON && stackWeaponMastery == FALSE)
			return Iter->second;
		else if(Iter->second != MASTERY_WEAPON && stackArmorMastery == FALSE)
			return Iter->second;
	}

	return MASTERY_NONE;
}

void CAutoLearn::Initialize()
{
	const TCHAR* section = _T("AutoLearn");
	bAutoLearn = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
	if(bAutoLearn)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled.", __FUNCTION__);
		disableAutoLearnOnEnterWorld = GetPrivateProfileInt(section, _T("DisableAutoLearnOnEnterWorld"), 0, g_ConfigFile);
		g_AllowedSkillLevelDifference = GetPrivateProfileInt(section, _T("AllowedSkillLevelDifference"), 9, g_ConfigFile);
		if(disableAutoLearnOnEnterWorld)
		{
			//EnterWorld
			g_HookManager.WriteCall( 0x518F10, AcquireSkillOnEnterWorld, 0);
		}else
		{
			//EnterWorld
			g_HookManager.WriteCall( 0x518F10, CCreature_AcquireSkill, 0);
		}

		//CCreature::LevelUp
		g_HookManager.WriteCall( 0x515D6B, CCreature_AcquireSkill, 0);
		//CCreature::ChangeClass
		g_HookManager.WriteCall( 0x519539, AcquireSkillOnChangeClass, 0);
		//AtomicPledgeLevelUp::Do
		g_HookManager.WriteCall( 0x43354F, CCreature_AcquireSkill, 0);
		//NpcPledgeLevelUp
		g_HookManager.WriteCall( 0x6DC912, CCreature_AcquireSkill, 0);
		//User::SetPledge
		g_HookManager.WriteCall( 0x810DC0, CCreature_AcquireSkill, 0);
		//User::ValidateSubJobParameters
		g_HookManager.WriteCall( 0x852694, CCreature_AcquireSkill, 0);
		//User::EnterWorld
		g_HookManager.WriteCall( 0x84DDD5, CCreature_DeleteSkill, 0);
		g_HookManager.WriteCall( 0x84DDDF, CAutoLearn::OnSendDeletedSkillToDB, 0);

		//CPledge::Dismiss
		g_HookManager.WriteCall( 0x76BF2F, CCreature_DeleteSkill, 0);
		//CCreature::LevelDown
		g_HookManager.WriteCall( 0x515506, CCreature_DeleteSkill, 0);
		//CCreature::LevelDown
		g_HookManager.WriteCall( 0x515563, CCreature_DeleteSkill, 0);
		//CCreature::LevelDown
		g_HookManager.WriteCall( 0x515579, CAutoLearn::OnSendDeletedSkillToDB, 0);

		//User::OnChangeSubjobEnd
		g_HookManager.WriteCall( 0x854DCB, CCreature_DeleteSkill, 0);
		g_HookManager.WriteCall( 0x6B9BBF, CNoblessSystem_SetNoblessSkill, 0);

		stackArmorMastery = GetPrivateProfileInt(section, _T("StackArmorMastery"), 1, g_ConfigFile);
		stackWeaponMastery = GetPrivateProfileInt(section, _T("StackWeaponMastery"), 1, g_ConfigFile);

		TCHAR tmp[8192]; memset(tmp, 0, sizeof(tmp));
		if(GetPrivateProfileString(section, _T("RobeMasteryList"), NULL, tmp, 8192, g_ConfigFile))
		{
			
			tstringstream sstr;
			sstr << tmp ;
			int skillId = 0;
			while(sstr >> skillId)
			{
				mSkillMastery.insert(pair<int, int>(skillId, MASTERY_ROBE));
			}
		}
		
		if(GetPrivateProfileString(section, _T("LightMasteryList"), NULL, tmp, 8192, g_ConfigFile))
		{
			tstringstream sstr;
			sstr << tmp ;
			int skillId = 0;
			while(sstr >> skillId)
			{
				mSkillMastery.insert(pair<int, int>(skillId, MASTERY_LIGHT));
			}
		}
		
		if(GetPrivateProfileString(section, _T("HeavyMasteryList"), NULL, tmp, 8192, g_ConfigFile))
		{
			tstringstream sstr;
			sstr << tmp ;
			int skillId = 0;
			while(sstr >> skillId)
			{
				mSkillMastery.insert(pair<int, int>(skillId, MASTERY_HEAVY));
			}
		}

		if(GetPrivateProfileString(section, _T("WeaponMasteryList"), NULL, tmp, 8192, g_ConfigFile))
		{
			tstringstream sstr;
			sstr << tmp ;
			int skillId = 0;
			while(sstr >> skillId)
			{
				mSkillMastery.insert(pair<int, int>(skillId, MASTERY_WEAPON));
			}
		}
	}
}

void CAutoLearn::CCreature_AcquireSkill(CCreature *pCreature, int nSkillID, int nSkillLevel, int Replace, int UseLock)
{
	if(pCreature->ValidUser())
	{
		if(stackArmorMastery == FALSE || stackWeaponMastery == FALSE)
		{
			if(int Type = GetMasteryType(nSkillID))
			{
				for(map<int, int>::iterator Iter = mSkillMastery.begin();Iter!=mSkillMastery.end();Iter++)
				{
					if(Iter->second == Type && Iter->first != nSkillID)
					{
						if(pCreature->GetAcquiredSkillLevel(Iter->first) > 0)
						{
							return;
						}
					}
				}
			}
		}

		if(pCreature->HaveSkill(nSkillID, nSkillLevel))
		{
			return;
		}
		else
		{
			pCreature->AcquireSkill(nSkillID, nSkillLevel, Replace, UseLock);
			g_DB.SendAcquiredSkill(pCreature->nDBID, nSkillID, nSkillLevel, false);
			return;
		}
	}
	pCreature->AcquireSkill(nSkillID, nSkillLevel, Replace, UseLock);
}

void CAutoLearn::AcquireSkillOnEnterWorld(CCreature *pCreature, int nSkillID, int nSkillLevel, int Replace, int UseLock)
{
	if(pCreature->ValidUser())
	{
		if(User *pUser = pCreature->GetUser())
		{
			if(stackArmorMastery == FALSE || stackWeaponMastery == FALSE)
			{
				if(int Type = GetMasteryType(nSkillID))
				{
					for(map<int, int>::iterator Iter = mSkillMastery.begin();Iter!=mSkillMastery.end();Iter++)
					{
						if(Iter->second == Type && Iter->first != nSkillID)
						{
							if(pCreature->GetAcquiredSkillLevel(Iter->first) > 0)
							{
								return;
							}
						}
					}
				}
			}
		}

		if(pCreature->HaveSkill(nSkillID, nSkillLevel))
		{
			return;
		}
		else
		{
			pCreature->AcquireSkill(nSkillID, nSkillLevel, Replace, UseLock);
			g_DB.SendAcquiredSkill(pCreature->nDBID, nSkillID, nSkillLevel, false);
			return;
		}
	}
	pCreature->AcquireSkill(nSkillID, nSkillLevel, Replace, UseLock);
}

void CAutoLearn::AcquireSkillOnChangeClass(CCreature *pCreature, int nSkillID, int nSkillLevel, int Replace, int UseLock)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		if(stackArmorMastery == FALSE || stackWeaponMastery == FALSE)
		{
			if(int Type = GetMasteryType(nSkillID))
			{
				for(map<int, int>::iterator Iter = mSkillMastery.begin();Iter!=mSkillMastery.end();Iter++)
				{
					if(Iter->second == Type && Iter->first != nSkillID)
					{
						if(pCreature->GetAcquiredSkillLevel(Iter->first) > 0)
						{
							return;
						}
					}
				}
			}
		}

		if(pCreature->HaveSkill(nSkillID, nSkillLevel))
		{
			return;
		}
		else
		{
			pCreature->AcquireSkill(nSkillID, nSkillLevel, Replace, UseLock);
			return;
		}
	}
	pCreature->AcquireSkill(nSkillID, nSkillLevel, Replace, UseLock);
}

void CAutoLearn::OnSendDeletedSkillToDB(User *pUser, INT32 skillId)
{
	CTL;

	int acquiredLevel = pUser->GetAcquiredSkillLevel(skillId);
	if(acquiredLevel > 0)
	{
		if(CSkillInfo *pInfo = g_SkillDB.GetSkillInfo(skillId, acquiredLevel))
		{
			if(pInfo->nSkillID == 239) //Expertise
			{
				if(pUser->pSD->nLevel < pInfo->nMagicLevel)
				{
					pUser->SendDeleteSkillToDB(skillId);
				}
			}else if(pInfo->nMagicLevel > (pUser->pSD->nLevel + g_AllowedSkillLevelDifference))
			{
				pUser->SendDeleteSkillToDB(skillId);
			}
		}
	}
}

void CAutoLearn::CCreature_DeleteSkill(CCreature *pCreature, int nSkillID)
{
	if(pCreature->ValidUser())
	{
		int nLevel = pCreature->GetAcquiredSkillLevel(nSkillID);
		CSkillInfo *pInfo = g_SkillDB.GetSkillInfo(nSkillID, nLevel);
		if(pInfo)
		{
			if(pInfo->nSkillID == 239) //Expertise
			{
				if(pCreature->pSD->nLevel < pInfo->nMagicLevel)
				{
					pCreature->DeleteSkill(nSkillID);
					pCreature->GetUser()->SendDeleteSkillToDB(nSkillID);
				}
			}else if(pInfo->nMagicLevel > (pCreature->pSD->nLevel + g_AllowedSkillLevelDifference))
			{
				pCreature->DeleteSkill(nSkillID);
				pCreature->GetUser()->SendDeleteSkillToDB(nSkillID);
			}
		}
	}else
		pCreature->DeleteSkill(nSkillID);
}

void CAutoLearn::CNoblessSystem_SetNoblessSkill(User *pUser, int nSkillID, int nLevel, int Replace, int UseLock)
{
	pUser->AcquireSkill(nSkillID, nLevel, Replace, UseLock);
	g_DB.SendAcquiredSkill(pUser->nDBID, nSkillID, nLevel, false);
}

#pragma once

enum MASTERY_TYPE
{
	MASTERY_NONE = 0,
	MASTERY_ROBE = 1,
	MASTERY_LIGHT = 2,
	MASTERY_HEAVY = 3,
	MASTERY_WEAPON = 4
};

class CAutoLearn
{
public:
	static void Initialize();
	static void CCreature_AcquireSkill(CCreature *pCreature, int nSkillID, int nSkillLevel, int Replace, int UseLock);
	static void AcquireSkillOnEnterWorld(CCreature *pCreature, int nSkillID, int nSkillLevel, int Replace, int UseLock);
	static void AcquireSkillOnChangeClass(CCreature *pCreature, int nSkillID, int nSkillLevel, int Replace, int UseLock);
	static void CCreature_DeleteSkill(CCreature *pCreature, int nSkillID);
	static void OnSendDeletedSkillToDB(User *pUser, INT32 skillId);
	static void CNoblessSystem_SetNoblessSkill(User *pUser, int nSkillID, int nLevel, int Replace, int UseLock);
};
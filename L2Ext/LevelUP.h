#pragma once


class CL2LevelUP
{
	public:
		static void StartSystem();
		static void ExpInc(User *, INT32, bool);
		static bool ValidateLevel(CCreature *pCreature);
		static INT64 GetExperience(INT32 nExperience);
		static double CPCParamDB_GetKarmaIncreaseAmount(LPVOID pInstance, int Level);
};
extern "C"
{
	// Hooks
	void LevelUpSystemFixHook2();
	void UserInfoExpFix1();
	void UserInfoExpFix2();
	// Wrappers
	INT64 L2LevelUP_GetExperienceWrapper(INT32 nExperience);
}
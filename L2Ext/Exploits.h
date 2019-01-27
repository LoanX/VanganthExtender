#pragma once

//Exploit Fixes
class CAntiExploit
{
public:
	static void Init();
	static void User_Ride(User *pUser, CPet *pPet);
	static void User_DespawnSummon(User *pUser);
	static void User_OnChangeSubjobEnd(INT64 nCached, User *pUser);
	static bool ValidateStats(CCreature *pCreature);
	static const WCHAR* GetHTMLFile(LPVOID pInstance, const WCHAR* wFileName);
};
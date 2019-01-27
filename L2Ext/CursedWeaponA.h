#pragma once
#include "class.h"

class CCursedWeaponData
{
public:
	int nWeaponID;
	int nSkillID;
	int nDropRate;
	int nKillCount;
	DWORD nExpireTime;
	DWORD nDropTime;
	int nOwnerDBID;
	UINT nOwnerObjectID;
	int nX;
	int nY;
	int nZ;
private:
	CRITICAL_SECTION cs_lock;
public:
	CCursedWeaponData()
	{
		InitializeCriticalSection(&cs_lock);
		nWeaponID = 0;
		nSkillID = 0;
		nDropRate = 0;
		nKillCount=0;
		nExpireTime=0;
		nDropTime = 0;
		nOwnerDBID=0;
		nOwnerObjectID=0;
		nX=0;
		nY=0;
		nZ=0;

	};
	void Lock(){ EnterCriticalSection(&cs_lock);};
	void Unlock(){ LeaveCriticalSection(&cs_lock);};
	void Save();
	void Clear();
	~CCursedWeaponData(){DeleteCriticalSection(&cs_lock);};
};

class CursedWeapon
{
public:
	static void Initialize();
	static bool CursedWeaponList(CSocket *pSocket);
	static bool CursedWeaponLocation(CSocket *pSocket);
	static bool NpcDropZariche(CNPC *pNPC, CCreature *pKiller);
	static bool NpcDropAkamanah(CNPC *pNPC, CCreature *pKiller);
	static void PickUp(User *pUser, CItem *pItem);
	static void PickUp(CPet *pPet, CItem *pItem);
	static void OwnerDie(User *pUser, CCreature *pKiller);
	static void Victim(User *pUser, CCreature *pKiller);
	static void TimerExpired();
	static void ValidateLevel(User *pUser, int nWeaponID);
	static void LogIn(User *pUser);
	static void LogOut(User *pUser);
	static void SendTimeLeft(CSocket *pSocket, int nWeaponID);
	static int GetCursedWeaponLevel(User *pUser);
	static void GetZaricheInfo(CSocket *pSocket);
	static void GetAkamanahInfo(CSocket *pSocket);
	static void ResetAkamanah(CSocket *pSocket);
	static void ResetZariche(CSocket *pSocket);
};
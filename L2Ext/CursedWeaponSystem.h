#pragma once

#include "CursedWeapon.h"

class CCursedWeaponSystem
{
private:
	map<int, CCursedWeapon*> m_Weapons;
public:
	CCursedWeaponSystem();
	~CCursedWeaponSystem();
	void Initialize();
	bool IsCursedWeapon(CItem *pItem);
	bool IsCursedWeapon(int nWeaponID);
	void OnNpcDie(CNPC *pNPC, CCreature *pEnemy);
	void OnPickUp(User *pUser, CItem *pItem);
	void OnPickUp(CPet *pPet, CItem *pItem);
	void OnDie(User *pUser, CCreature *pKiller);
	void OnEnterWorld(User *pUser);
	void OnLeaveWorld(User *pUser);
	void OnTeleport(User *pUser);
	void OnChangeSubjob(User *pUser);
	void TimerExpired();
	int GetLevel(User *pUser);
	bool WeaponList(CSocket *pSocket);
	bool WeaponLocation(CSocket *pSocket);
	bool RequestReset(User *pUser, int nWeaponID);
	bool RequestInfo(User *pUser, int nWeaponID);
};

extern CCursedWeaponSystem g_CursedWeaponSystem;
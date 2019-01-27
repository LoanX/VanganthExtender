#pragma once


class CShadowItem
{
public:
	static bool IsShadowItem(int nItemID);
	static void EnterWorld(User *pUser);
	//Actions
	static void EquipItem(User *pUser, CItem *pItem);
	static void TimerExpired(User *pUser, int nTimerID);
};
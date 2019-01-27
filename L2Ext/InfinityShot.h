#pragma once

class CInfinityShot
{
	BOOL m_Enabled;
	BOOL m_AllowSpiritShotInOlympiad;
	BOOL m_AllowBlessedSpiritShotInOlympiad;
	BOOL m_AllowSoulShotInOlympiad;
	void LoadINI();
public:
	void Init();
	bool AutoOn(User *pUser, int itemId);
	bool AutoOff(User *pUser, int itemId);
	void AutoOff(User *pUser);
	bool Activate(User *pUser, CItem *pItem);
	bool TurnOn(User *pUser, CItem *pItem);
	inline BOOL IsEnabled() { return m_Enabled; };
	inline BOOL IsAllowSpiritShotInOlympiad() { return m_AllowSpiritShotInOlympiad; };
	inline BOOL IsAllowBlessedSpiritShotInOlympiad() { return m_AllowBlessedSpiritShotInOlympiad; };
	inline BOOL IsAllowSoulShotInOlympiad() { return m_AllowSoulShotInOlympiad; };
};

extern CInfinityShot g_InfinityShot;
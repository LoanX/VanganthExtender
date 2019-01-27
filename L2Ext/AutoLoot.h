#pragma once


class CAutoLoot
{
	BOOL m_enabled;
	BOOL m_activeByDefault;
	BOOL m_affectParty;
	BOOL m_enableUserExCommand;
	BOOL m_affectZZoldagu;
	BOOL m_affectBoss;
	vector<int> m_ExceptionItems;
	vector<int> m_AffectedItems;
public:
	CAutoLoot();
	~CAutoLoot();
	void Init();
	inline BOOL IsEnabled() { return m_enabled; };
	inline BOOL IsActiveByDefault() { return m_activeByDefault; };
	bool Handle(CNPC *pNpc, CCreature *pKiller, INT32 itemId, INT32 amount);
	inline BOOL IsEnabledUserExCommand() { return m_enableUserExCommand; };
};

extern CAutoLoot g_AutoLoot;
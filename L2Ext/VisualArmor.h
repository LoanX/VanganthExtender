#pragma once

struct VisualArmorInfo
{
	UINT armorType;
	int chestSlotId;
	int legsSlotId;
	int glovesSlotId;
	int feetSlotId;
	int minLevel;
	int maxLevel;
	ItemReq price;

	VisualArmorInfo() : armorType(0), chestSlotId(0), legsSlotId(0), glovesSlotId(0), feetSlotId(0), minLevel(0), maxLevel(0) {};
};

class CVisualArmor
{
	CSLock m_Lock;
	bool m_Enabled;
	bool m_CheckBodyParts;
	map<UINT, VisualArmorInfo*> m_Armors;
	time_t m_ReloadTick;
	void LoadData();
public:
	CVisualArmor();
	~CVisualArmor();
	void Init();
	void ShowMenu(User *pUser);
	void OnRequestEquip(User *pUser);
	VisualArmorInfo* Get(UINT armorId);
	void OnEquip(User *pUser, UINT armorId);
	void OnUnequip(User *pUser);
	void TimerExpired();
	inline void RequestReload() { m_ReloadTick = time(0) + 1; m_Enabled = FALSE; };
	inline bool IsEnabled() { return m_Enabled; };
	inline bool CheckBodyParts() { return m_CheckBodyParts; };
};

extern CVisualArmor g_VisualArmor;
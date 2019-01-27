#pragma once

/*
4_MagicWeaponEnchantBonus=25
4_NormalWeaponEnchantBonus=25
4_ArmorEnchantBonus=25
4_DropRateBonus=25
4_SpoilRateBonus=25
4_ExpRateBonus=25
4_SpRateBonus=25
4_CraftRateBonus=25
4_SpecialChat=1
*/
struct VIPInfo
{
	int level;
	ItemReq requiredItem;
	int requiredLevel;
	DWORD timestamp;
	int magicWeaponEnchantBonus;
	int normalWeaponEnchantBonus;
	int armorEnchantBonus;
	int adenaRateBonus;
	int dropRateBonus;
	int spoilRateBonus;
	int expRateBonus;
	int spRateBonus;
	int craftRateBonus;
	bool specialChat;
	UINT chatDelay;
	bool canLoginWhenFull;
	ItemReq reward;
	int skillId;
	int skillLevel;
	VIPInfo() : skillId(0), skillLevel(0), level(0), requiredLevel(0), timestamp(0), magicWeaponEnchantBonus(0), normalWeaponEnchantBonus(0), armorEnchantBonus(0),
		dropRateBonus(0), adenaRateBonus(0), spoilRateBonus(0), expRateBonus(0), spRateBonus(0), craftRateBonus(0), specialChat(false), chatDelay(0), canLoginWhenFull(false) {};
};

extern "C"
{
	void DropRateHook();
	double GetDropRateWrapper();
	void SpoilRateHook();
	double GetSpoilRateWrapper(CCreature* pCreature);
	void CraftRateHook1();
	void CraftRateHook2();
	double GetCraftRateWrapper(CCreature* pCreature, CRecipeInfo* pRecipeInfo);
	void UserLimitHook();
	int GetUserLimitWrapper(int accountId);
};

class CVIPSystem
{
	vector<VIPInfo> m_Info;
	VIPInfo m_BaseInfo;
	BOOL m_Enabled;
	UINT m_NotVIPLevelRestriction;
public:
	CVIPSystem();
	~CVIPSystem();
	inline BOOL IsEnabled() { return m_Enabled; };
	inline UINT GetNotVIPLevelRestriction() { return m_NotVIPLevelRestriction; };
	void Init();
	void LoadINI();
	void OnLoad(User *pUser);
	bool RequestChangeLevel(User *pUser, int newLevel);
	void SendBonusInfo(User *pUser, int level);
	void SendExpireInfo(User *pUser);
	void TimerExpired(User *pUser);
	void HandleCommand(User *pUser, wstring wCommand);
	VIPInfo& GetInfo(int vipLevel);
	double GetAdenaRate(CCreature *pCreature);
	double GetDropRate(CCreature *pCreature);
	double GetSpoilRate(CCreature *pCreature);
	int GetExp(int baseExp, int vipLevel);
	int GetSP(int baseSP, int vipLevel);
	double GetCraftRate(CCreature *pCreature, CRecipeInfo* pRecipeInfo);
	void OnHelp(User *pUser);
	void OnStatus(User *pUser);
};

extern CVIPSystem g_VIPSystem;

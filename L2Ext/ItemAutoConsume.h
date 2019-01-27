#pragma once

enum AutoConsumeType
{
	AutoConsumeNone = 0,
	AutoConsumeHp = 1,
	AutoConsumeMp = 2,
	AutoConsumeCp = 3,
	AutoConsumeCpAdv = 4
};

struct AutoConsumeInfo
{
	INT32 itemId;
	AutoConsumeType type;
	double activateOnMissingPercent;
	double activateOnMissingFixed;
	UINT reuseDelay;
	INT32 requiredLevel;
	BOOL canUseInOlympiad;
};

class CItemAutoConsume
{
	BOOL m_enabled;
	map<INT32, AutoConsumeInfo*> m_items;
	void LoadData();
public:
	CItemAutoConsume();
	~CItemAutoConsume();
	void Init();
	BOOL IsAutoConsume(INT32 itemId);
	void TurnOn(User *pUser, INT32 itemId);
	void TurnOff(User *pUser, INT32 itemId, BOOL useLock = TRUE, BOOL outOfPots = FALSE);
	void OnTiggerHp(User *pUser);
	void OnTiggerMp(User *pUser);
	void OnTiggerCp(User *pUser);
	void OnTiggerCpAdv(User *pUser);
};

extern CItemAutoConsume g_ItemAutoConsume;
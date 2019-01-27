#pragma once

struct DigItemData
{
	int item_id;
	int min_count;
	int max_count;
	double chance; // x/1000000
};

struct MiningAreaData
{
	UINT id;
	int requiredWeaponId;
	vector<DigItemData> items; //up to 32 differen items
};

class CMiningSystem
{
	map<UINT, MiningAreaData*> m_data;
public:
	CMiningSystem();
	~CMiningSystem();
	void Init();
	void LoadData();
	void ClearData();
	bool CanDig(User* pUser);
	bool Dig(User *pUser, double baseChance);
};

extern CMiningSystem g_MiningSystem;
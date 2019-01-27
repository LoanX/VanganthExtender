#pragma once

#define ENCHANTED_SET_D_HEAVY	3611
#define ENCHANTED_SET_D_LIGHT	3612
#define ENCHANTED_SET_D_ROBE	3613
#define ENCHANTED_SET_C_HEAVY	3614
#define ENCHANTED_SET_C_LIGHT	3615
#define ENCHANTED_SET_C_ROBE	3616
#define ENCHANTED_SET_B_HEAVY	3617
#define ENCHANTED_SET_B_LIGHT	3618
#define ENCHANTED_SET_B_ROBE	3619
#define ENCHANTED_SET_A_HEAVY	3620
#define ENCHANTED_SET_A_LIGHT	3621
#define ENCHANTED_SET_A_ROBE	3622
#define ENCHANTED_SET_S_HEAVY	3623
#define ENCHANTED_SET_S_LIGHT	3624
#define ENCHANTED_SET_S_ROBE	3625

struct EnchantedSetInfo
{
	int set_id;
	int skill_id;
};

class CEnchantedSet
{
	map<int, EnchantedSetInfo*> mData;
public:
	void Initialize();
	void ReadData(string sFile);
	void Validate(User* pUser);
};

extern CEnchantedSet g_EnchantedSet;
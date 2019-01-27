#pragma once

struct ChampionNpcInfo
{
	UINT baseNpcId;
	UINT championNpcId;
	double chance;
	ChampionNpcInfo() : baseNpcId(0), championNpcId(0), chance(0.0) {};
};

class CChampionNpc
{
	bool m_Enabled;
	vector<ChampionNpcInfo*> m_Npc;
	void LoadINI();
public:
	void Init();
	inline bool IsEnabled() { return m_Enabled; };
	UINT GetNpcClassId(UINT baseNpc);
};

extern CChampionNpc g_ChampionNpc;
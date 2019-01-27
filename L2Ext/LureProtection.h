#pragma once

class CLureProtection
{
	bool m_Enabled;
	vector<UINT> m_Npcs;
	INT32 m_LureRange;
	
	void LoadINI();
public:
	void Init();
	void Handle(CNPC *pNpc);
	inline bool Enabled() { return m_Enabled; }
};

extern CLureProtection g_LureProtection;

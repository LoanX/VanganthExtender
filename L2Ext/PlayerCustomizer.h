#pragma once

class CPlayerCustomizer
{
	map<int, DWORD> m_PvpTitleColor;
	BOOL m_Enabled;
public:
	CPlayerCustomizer() {};
	~CPlayerCustomizer() {};
	void Init();
	void SetPvpTitleColor(int nPvpCount, DWORD nColor);
	DWORD GetPvpTitleColor(int nPvpCount);
	inline BOOL IsEnabled() { return m_Enabled; };
};

extern CPlayerCustomizer g_PlayerCustomizer;
#pragma once

class CCastleSiegeManager
{
	BOOL m_Enabled;
	map<UINT, UINT> m_Dates;
	UINT m_CycleTime;
	void LoadINI();
public:
	void Init();
	void TimerExpired();
	inline BOOL IsEnabled() { return m_Enabled; };
};

extern CCastleSiegeManager g_CastleSiegeManager;
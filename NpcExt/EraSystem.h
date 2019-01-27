#pragma once

/*
	Up to 32 eras
*/
struct EraInfo
{
	DWORD nCurrentEra;
	DWORD nCount;
	DWORD nDuration;
	DWORD nLastChange;
	DWORD StartTime;
	DWORD UseMoon;
	DWORD MoonState[32];
	wchar_t EraNames[32][25];
};

class CEraSystem
{
	CSLock m_lock;
	BOOL m_enabled;
	INT32 m_currentEra;
	INT32 m_count;
	INT32 m_duration;
	UINT m_lastChange;
	UINT m_startTime;
	UINT m_useMoon;
	UINT m_moonState[32];
	WCHAR m_eraNames[32][25];
	void LoadINI();
	void SaveINI();
public:
	enum STATE
	{
		NOTHING,
		START,
		ERA_CHANGE
	};
	CEraSystem();
	inline BOOL IsEnabled() { return m_enabled; };
	void Init();
	int GetEraIndex(PWCHAR wName);
	void TimerExpired();
	void SetCurrentEra(int nEra);
	STATE ValidateEra();
	void ProcessingDone(int nEraIndex, BOOL Spawned);
};

extern CEraSystem g_EraSystem;
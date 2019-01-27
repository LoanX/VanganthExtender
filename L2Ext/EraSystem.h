#pragma once



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

class CEraSystem : private CThreadSafe
{
	EraInfo Data;
	enum MOON
	{
		NOT_USED = -1,
		NORMAL = 0,
		DUSK = 1,
		DAWN = 2
	};
	MOON Moon;
public:
	CEraSystem();
	void Initialize(const unsigned char* packet);
	void TimerExpired();
	void ChangeEra(DWORD nEra, PWCHAR wEra, DWORD nLastChanged);
	void ProcessingFinished(int nEraIndex, PWCHAR wEra, bool Spawned);
	void ValidateMoon();
	void OnEnterWorld(User *pUser);
	void ForceChangeEraGM(User *pUser, DWORD nEra);
};

extern CEraSystem g_EraSystem;
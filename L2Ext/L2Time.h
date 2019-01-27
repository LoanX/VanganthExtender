#pragma once

class CL2Time
{
	CL2Time* pInstance;
public:
	CL2Time();
	~CL2Time();
	static void Initialize(); /* static func */
	static void TimerExpired(CL2Time* pL2Time, int id);
	void Init();
	long int GetTime();
	bool IsNight();
	void SetTime(UINT nTime);
};

extern CL2Time g_L2Time;
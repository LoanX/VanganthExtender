#pragma once

class CAntiDDoS
{
	CRITICAL_SECTION _lock;
	inline void Lock() { EnterCriticalSection(&_lock); };
	inline void Unlock() { LeaveCriticalSection(&_lock); };
	list<string> lUrl;
	BOOL enabled;
	BOOL announceToPlayers;
	BOOL disableLogin;
	BOOL kickPlayers;
	BOOL useProtectionSkill;
	DWORD checkTimeout;
	DWORD milisecondPingLimit;
	int skillId;
	wstring msgStart;
	wstring msgEnd;

	//Thread info
	HANDLE threadHandle;
	DWORD threadId;
	DWORD startTick;
	DWORD safeTickCount;

	BOOL underAttack;
	BOOL performedAction;

	bool IsWorldReachable();
	void ValidateState();
	void PerformAction(BOOL start);
public:
	ULONG Ping(const CHAR* address, int timeout = 500);
	void LoadINI();
	CAntiDDoS();
	inline int GetSkillId() { return skillId; };
	virtual ~CAntiDDoS();
	BOOL IsUnderAttack();
	void SetUnderAttack(BOOL value);
	void TimerExpired();

	static DWORD WINAPI ValidateNetworkThread(LPVOID param);
};

extern CAntiDDoS g_AntiDDoS;
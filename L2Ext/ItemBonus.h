#pragma once

class CItemBonus
{
	CRITICAL_SECTION _lock;
	inline void Lock() { EnterCriticalSection(&_lock); }
	inline void Unlock() { LeaveCriticalSection(&_lock); };
	map<INT32, double> items;
public:
	CItemBonus();
	~CItemBonus();
	void Add(INT32 itemId, double bonus);
	double Get(INT32 itemId);
	void GetAll(map<INT32, double>& bonusMap);
	void Clear();
};

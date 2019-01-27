#pragma once


class CThreadSafe
{
	CRITICAL_SECTION lock;
public:
	CThreadSafe() { InitializeCriticalSection(&lock); };
	virtual ~CThreadSafe() { DeleteCriticalSection(&lock);};
	__forceinline void Lock() { EnterCriticalSection(&lock); };
	__forceinline void Unlock() { LeaveCriticalSection(&lock); };
};
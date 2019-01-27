#pragma once
#include "class.h"

template<class Object>class CGlobalVariable
{
	CRITICAL_SECTION lock;
	inline void Lock() { EnterCriticalSection(&lock); };
	inline void Unlock() { LeaveCriticalSection(&lock); };
public:
	CGlobalVariable() { InitializeCriticalSection(&lock); };
	~CGlobalVariable() { DeleteCriticalSection(&lock); };
	void SetVariable(Object var)
	{
		Lock();
		Variable = var;
		Unlock();
	}
	Object GetVariable()
	{
		Object obj;
		Lock();
		obj = Variable;
		Unlock();
		return obj;
	}
private:
	Object Variable;
};

typedef CGlobalVariable<int> GlobalIntVariable;
extern GlobalIntVariable g_Era;


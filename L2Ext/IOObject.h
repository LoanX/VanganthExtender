#pragma once
#include "MemoryObject.h"

//0x20 size
class CIOObject : public MemoryObject
{
public:
	/* 0018 */ CSocket *pSocket;
	bool AddTimer(int nTimeSpan, int nTimerID)
	{
		typedef bool (*f)(CIOObject*, int, int);
		return f(0x6261D0L)(this, nTimeSpan, nTimerID);
	};
	static bool SetTimeStamp(long int* pTimeVar)
	{
		typedef bool (*f)(long int*);
		return f(0x61DE00L)(pTimeVar);
	}
	bool IsValidIOObject()
	{
		if(this)
		{
			try
			{
				if( (*(LPDWORD)this) == VT_CIOOBJECT )
				{
					return true;
				}
			}catch(...)
			{
				return false;
			}
		}
		return false;
	}
};
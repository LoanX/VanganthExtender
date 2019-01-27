#pragma once
#include "class.h"

class CHookGuard
{
	map<INT64, bool> mData;
public:
	CHookGuard() {};
	~CHookGuard() {};
	bool CanWrite(INT64 nAddress, int nLen)
	{
		map<INT64, bool>::iterator Iter = mData.begin();
		for(;Iter!=mData.end();Iter++)
		{
			if(Iter->first >= nAddress && Iter->first < (nAddress+nLen))
			{
				char szMsg[256];
				memset(szMsg, 0, 256);
				sprintf(szMsg, "Memory at address[0x%x] has been already changed!", Iter->first);
				MessageBoxA(NULL, szMsg, __FUNCTION__, MB_OK);
				return false;
			}
		}
		
		for(INT64 n = nAddress; n < (nAddress+nLen); n++)
			mData[n] = true;
		return true;
	}
};

extern CHookGuard g_HookGuard;
#include "StdAfx.h"


CIOBuffer* CIOBuffer::GetIOBuffer()
{
	typedef CIOBuffer* (*f)();
	return f(0x61E050L)();
}

void CIOBuffer::FreeIOBuffer()
{
	typedef void(*f)(CIOBuffer*);
	f(0x61E200L)(this);
}

bool CIOSocket::ValidPtr()
{
	if(this)
	{
		try
		{
			if(*(DWORD*)this == 0xA8EE78) //CUserSocket
				return true;
			if(*(DWORD*)this == 0xA04338) //CIOSocket
				return true;
			if(*(DWORD*)this == 0xA72168) //CSocket
				return true;
		}
		catch(...)
		{
			return false;
		}
	}
	return false;
}
#include "StdAfx.h"

bool CSocket::ValidUserSocket()
{
	if(this)
	{
		try
		{
			if(*(DWORD*)this == 0xA8EE78)
				if(this->s != -1)
					return true;
		}
		catch(...)
		{
			return false;
		}
	}
	return false;
}

CUserSocket* CSocket::CastUserSocket()
{
	if(this)
	{
		try
		{
			if(*(LPDWORD)this == 0xA8EE78) //CUserSocket
				return (CUserSocket*)this;

		}catch(...)
		{
		}
	}
	return 0;
}

void CSocket::Send(const char *format, ...)
{
	typedef void (*f)(CSocket*, const char*, va_list va);
	
	va_list va;
	va_start(va, format);
	f(0x7D5D40)(this, format, va);
	va_end(va);
}

void CSocket::SendV(const char *format, va_list va)
{
	typedef void (*f)(CSocket*, const char*, va_list);
	f(0x007D5D40L)(this, format, va);
}

void CSocket::Send(CServerPacket *pck)
{
	if(this)
	{
		Send("b", pck->GetSize(), pck->GetBuff());
	}
}

in_addr *CSocket::GetAddress(in_addr *pAddr)
{
	typedef in_addr*(*_f)(CSocket*, in_addr *);
	_f f = (_f) 0x007D53B0;
	return f(this, pAddr);
}



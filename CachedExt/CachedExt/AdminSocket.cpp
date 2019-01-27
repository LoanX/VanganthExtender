#include "AdminSocket.h"

void CAdminSocket::Send(UINT len, PCHAR buff)
{
	typedef void(__cdecl *f)(CAdminSocket*, const char*, UINT, PCHAR);
	f(0x407110L)(this, "b", len, buff);
}
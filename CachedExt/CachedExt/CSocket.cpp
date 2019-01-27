#include "CSocket.h"
#pragma comment(lib, "Ws2_32.lib")

/* use it only when it's needed (packet are bigger than 8192)*/
void CSocket::DirectSend(char *packet, int nSize)
{
	char Buff[16384];
	memset(Buff, 0, 16384);
	*(WORD*)Buff = (WORD)nSize;
	memcpy((Buff+2),packet, nSize);
	send(this->s, Buff, nSize, 0);
}

void CSocket::Lock(WCHAR *wszFile, int nLine)
{
	typedef void(__thiscall *lock)(CRITICAL_SECTION*, WCHAR*, int);
	lock _lock = (lock) 0x4422F0;
	_lock(&this->cs_Socket, wszFile, nLine);
}

void CSocket::Unlock()
{
	typedef void(__thiscall *unlock)(CRITICAL_SECTION*, WCHAR*, int);
	unlock _unlock = (unlock) 0x4425E0;
	_unlock(&this->cs_Socket, NULL, NULL);

}
void CSocket::Send(const char *format, ...)
{
	CHAR buff[8190];
	va_list va;
	va_start(va, format);
	typedef int(*f)(char*, int, const char*, va_list);
	int len = f(0x43E150L)(buff, 8190, format, va);
	va_end(va);
	Send(len, buff);
}

void CSocket::Send(UINT len, PCHAR buff)
{
	typedef void(__cdecl * f)(CSocket*, const char *format, UINT, PCHAR);
	f(0x467370L)(this, "b", len, buff);
}
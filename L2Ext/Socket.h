#pragma once
#include "IOSocket.h"
#include "Coder.h"

typedef bool (*Callback)(CSocket*, const unsigned char*);
typedef bool (*UserSocketCallback)(CUserSocket*, const unsigned char*);

//size 0x130
class CSocket : public CIOSocket
{
public:
	/* 00A8 */ LPVOID _unknA8;
	/* 00B0 */ in_addr addr;
	/* 00B4 */ int _unknB4;
	/* 00B8 */ LPVOID _unknB8;
	/* 00C0 */ LPVOID _unknC0;
	/* 00C8 */ LPVOID _unknC8;
	/* 00D0 */ LPVOID _unknD0;
	/* 00D8 */ LPVOID _unknD8;
	/* 00E0 */ LPVOID _unknE0;
	/* 00E8 */ LPVOID _unknE8;
	/* 00F0 */ LPVOID _unknF0;
	/* 00F8 */ LPVOID _unknF8;
	/* 0100 */ Callback* pHandler;
	/* 0108 */ int _unkn108;
	/* 010C */ int state; /* 2 - closed */
	/* 0110 */ int inPckSize;
	/* 0114 */ int _unkn114;
	/* 0118 */ LPVOID _unkn118;
	/* 0120 */ CCoder* pCoder;
	/* 0128 */ CCoder* _pCoder;

	bool ValidUserSocket();
	CUserSocket* CastUserSocket();
	in_addr *GetAddress(in_addr *pAddr);
	void Send(const char*format, ...);
	void SendV(const char *format, va_list va);
	void Send(CServerPacket *pck);
};

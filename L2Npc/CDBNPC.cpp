#include "CDBNPC.h"
#include "CLog.h"

CDBNPC::CDBNPC()
{
}

CDBNPC *CDBNPC::In()
{
	static CDBNPC DBNpc;
	return &DBNpc;
}

void CDBNPC::Send(DWORD *pCDBSocket, const char *format, ...)
{
	typedef void(__fastcall *__Send)(DWORD *pCDBSocket, const char *format, ...);
	__Send _Send = (__Send) 0x00451AE0;

	_Send(pCDBSocket, format);
}
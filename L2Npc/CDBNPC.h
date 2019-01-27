#ifndef _CDBNPC_
#define _CDBNPC_
	#ifndef _WINDOWS_
	#include <windows.h>
	#endif

class CDBNPC
{
public:
	CDBNPC();
	~CDBNPC(){ }
	static CDBNPC *In();
	static bool InitDBNPC();
	void SetSocketClosed();
	void SetSocketOpen();
	bool IsSocketOpen();
	void Send(DWORD *pCDBSocket, const char *format, ...);
private:
	bool bSocketClosed;
};
#endif
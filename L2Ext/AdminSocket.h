#pragma once
#include "TaskHandler.h"

class CClientData
{
public:
	CClientData() : s(INVALID_SOCKET), addrlen(sizeof(sockaddr)) {};
	SOCKET s;
	sockaddr addr;
	int addrlen;
};
class CAdminSocket : private CThreadSafe
{
	int ActiveConnection;
	int Port;
	SOCKET ListenSocket;
	HANDLE hAcceptThread;
	DWORD AcceptThreadID;
	int MaxClientCount;
	int Password;
public:
	CAdminSocket();
	void Init();
	SOCKET Accept(CClientData* pClientData);
	static DWORD WINAPI AcceptThread(LPVOID lpParam);
	static DWORD WINAPI HandleConnection( LPVOID lpParam );
	void IncreaseClientCount();
	void DecreaseClientCount();
	int GetClientCount();
	int GetMaxClientCount();
	int GetPassword();
};

extern CAdminSocket g_AdminSocket;
#include "StdAfx.h"

CAdminSocket::CAdminSocket()
{
	Port = 6891;
	ListenSocket = INVALID_SOCKET;
	ActiveConnection = 0;
	MaxClientCount = 20;
}

void CAdminSocket::IncreaseClientCount()
{
	Lock();
	ActiveConnection++;
	Unlock();
}

void CAdminSocket::DecreaseClientCount()
{
	Lock();
	ActiveConnection--;
	Unlock();
}

int CAdminSocket::GetClientCount()
{
	int nRet = 0;
	Lock();
	nRet = ActiveConnection;
	Unlock();
	return nRet;
}

int CAdminSocket::GetMaxClientCount()
{
	return MaxClientCount;
};

int CAdminSocket::GetPassword()
{
	return Password;
}

void CAdminSocket::Init()
{
	Port = g_Config.AdminSocketInfo.GetPort();
	MaxClientCount = g_Config.AdminSocketInfo.GetMaxActiveConnections();
	Password = g_Config.AdminSocketInfo.GetPassword();

	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ListenSocket == INVALID_SOCKET)
	{
		g_Log.Add(CLog::Error, "[%s] Create listen socket failed: %d", __FUNCTION__, WSAGetLastError());
		return;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(Port);

	if(bind( ListenSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR)
	{
		g_Log.Add(CLog::Error, "[%s] Bind socket failed: %d", __FUNCTION__, WSAGetLastError());
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		return;
	}

	if(listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		g_Log.Add(CLog::Error, "[%s] Listen socket failed: %d", __FUNCTION__, WSAGetLastError());
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		return;
	}

	g_Log.Add(CLog::Blue, "[%s] Server ready on port %d.", __FUNCTION__, Port);
	
	//create accept thread
	hAcceptThread = CreateThread(NULL, 0, CAdminSocket::AcceptThread, NULL, NULL, &AcceptThreadID);
};

SOCKET CAdminSocket::Accept(CClientData *pClientData)
{
	if(ListenSocket != INVALID_SOCKET)
		return accept(ListenSocket, &pClientData->addr, &pClientData->addrlen);
	return INVALID_SOCKET;
}

DWORD WINAPI CAdminSocket::AcceptThread(LPVOID lpParam)
{
	try
	{
		while(true)
		{
			if(g_AdminSocket.GetClientCount() < g_AdminSocket.GetMaxClientCount()) //Protection anty flood
			{
				CClientData Client;
				Client.s = g_AdminSocket.Accept(&Client);
				if(Client.s != INVALID_SOCKET)
				{
					CClientData* pClient = new CClientData();
					memcpy(pClient, &Client, sizeof(CClientData));
					g_AdminSocket.IncreaseClientCount();
					DWORD ThreadID = 0;
					HANDLE hThread = CreateThread(NULL, 0, CAdminSocket::HandleConnection, pClient, 0, &ThreadID);
				}else
					g_Log.Add(CLog::Error, "[%s] Invalid socket.", __FUNCTION__);
			}else
				g_Log.Add(CLog::Error, "[%s] Too many connected clients [%d].", __FUNCTION__, g_AdminSocket.GetClientCount());
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	return 0;
}

DWORD WINAPI CAdminSocket::HandleConnection(LPVOID lpParam)
{
	try
	{
		CClientData* pClient = (CClientData*)lpParam;
		sockaddr_in* pAddr = (sockaddr_in*)&pClient->addr;
		char *sIP = inet_ntoa(pAddr->sin_addr);
	//	g_Log.Add(CLog::Blue, "[%s] Ip[%s]", __FUNCTION__, sIP);
		//Receive data
		USHORT packetSize = 0;
		CHAR buffer[8192] = { 0 };
		int received = recv(pClient->s, buffer, 2, 0);
		while(received >= 2 && received != SOCKET_ERROR)
		{
			packetSize = (*(PUSHORT)buffer);
		//	g_Log.Add(CLog::Blue, "[%s] packetSize[%d] received[%d]", __FUNCTION__, packetSize, received);
			if(packetSize < 8192 && packetSize > 0)
			{
				while(received != packetSize)
				{
					int length = recv(pClient->s, &buffer[received], packetSize - received, 0);
				//	g_Log.Add(CLog::Blue, "[%s] receiving missing data - pack size[%d] received[%d] readed[%d]", __FUNCTION__, packetSize, received, length);
					if(length != SOCKET_ERROR && length > 0)
					{
						received += length;
					}else
					{
						break;
					}
				}

				if(received == packetSize)
				{
					//handle the packet
					//g_Log.Add(CLog::Blue, "[%s] handling packet size[%d]", __FUNCTION__, packetSize);
					g_TaskHandler.CreateTask((PCHAR)&buffer[2], pClient->s);
					memset(buffer, 0, sizeof(buffer));
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Error while receiving a packet(%d)!", __FUNCTION__, WSAGetLastError());
					break;
				}
			}else if(packetSize == 0)
			{
				break;
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid packet size[%d]", __FUNCTION__, packetSize);
				break;
			}
			received = recv(pClient->s, buffer, 2, 0);
		}
		Sleep(300);	//wait for reply to be sent
		closesocket(pClient->s);
		g_AdminSocket.DecreaseClientCount();
//		g_Log.Add(CLog::Blue, "[%s] Releasing socket [%d]", __FUNCTION__, g_AdminSocket.GetClientCount());
		if(pClient)
			delete pClient;
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	return 0;
}
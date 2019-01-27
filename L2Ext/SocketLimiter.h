#pragma once

class SocketLimiter
{
	static int nLastCheck;
public:
	static void Initialize();
	static SOCKET AcceptConnection(SOCKET ListenSocket, sockaddr *addr, int *addrlen);
	static void ValidateSend(CSocket *pSocket, const char *format, va_list va);
	static void SetPendingWriteLimit(int nLimit);
	static void TimerExpired();
	static bool ValidUserSocket(CSocket *pSocket);
	static void OnRead(CUserSocket *pSocket);
	static void Read(CUserSocket *pSocket, DWORD dwLeft);
	static void UserSocketBadPacketSizeLog(LPVOID lpInstance, UINT logType, const WCHAR* format, UINT64 sockFunc, int packetSize);
};

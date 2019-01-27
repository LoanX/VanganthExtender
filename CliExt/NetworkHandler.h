#pragma once
#include "ClientSocket.h"
#include "Packet.h"

typedef bool (__cdecl *HandlerCallback)(CClientSocket*);

//interlude 104h size
struct HandlerInfo
{
	HandlerCallback func;
	WCHAR wName[128];
};

struct NHandler
{
public:
	/* 00 */ DWORD _unkn00[18];
	/* 48 */ CClientSocket *pSocket;
};

typedef int (__cdecl *PacketHandler)(NHandler*, CInPacket& packet);

class CNetworkHandler
{
	UINT m_TickTimeout;
	UINT m_SeparatedTick;
	BOOL m_Initialized;
	time_t m_PingExTime;
	UINT m_RequestQuit;
	time_t m_RequestQuitTime;
	HANDLE m_SeparatedTickHandle;
	WCHAR m_RequestQuitModuleName[255];
	PacketHandler m_CustomHandler[256];
public:
	void Init();
	void Bind(UINT id, PacketHandler func);
	inline UINT GetTickTimeout() { return m_TickTimeout; };
	inline UINT GetRequestQuit() { return m_RequestQuit; };
	inline UINT GetSeparatedTick() { return m_SeparatedTick; };
	inline void SetRequestQuit(UINT value = 1, const WCHAR* wModuleName = 0) { m_RequestQuit = value; m_RequestQuitTime = time(0); if(wModuleName) { wcscpy(m_RequestQuitModuleName, wModuleName); }; };
	inline const WCHAR* GetRequestQuitModuleName() { return m_RequestQuitModuleName; };
	static int __thiscall OnTick(PVOID pInstance, float value);
	static int __thiscall OnGameTick(PVOID pInstance, float value);
	static void __cdecl SeparatedTick(void *args);
	static int __thiscall HandlePacket(NHandler *pNetwork, NPacket *pPacket);
	static void __thiscall OnConnect(PVOID pInstance, const char* sAddress, UINT port);
	static string WINAPI SendPostRequest(string sHost, string sPath, string params);
};

int CPHDummyPacket(NHandler *pNetwor, CInPacket& packet);
int CPHPingExPacket(NHandler *pNetwork, CInPacket& packet);
int CPHGetHWIDPacket(NHandler *pNetwor, CInPacket& packet);
int CPHPingPacket(NHandler *pNetwork, CInPacket& packet);
int CPHRequestL2NetworkVoteTime(NHandler* pNetwork, CInPacket& packet);
int CPHOpenWebsitePacket(NHandler *pNetwork, CInPacket& packet);
int CPHJobPacket(NHandler *pNetwork, CInPacket& packet);
int CPHMD5Packet(NHandler *pNetwork, CInPacket& packet);

extern CNetworkHandler g_NetworkHandler;
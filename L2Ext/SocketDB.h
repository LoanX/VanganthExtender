#pragma once

class CSocketData
{
public:
	enum STATUS
	{
		IDLE = 0,
		CHAR_SELECTION_SCREEN = 1,
		IN_WORLD = 2,
		LEAVING_WORLD = 3,
		OFFLINE_SHOPPING = 4
	};

	STATUS status;
	CIPAddress ip;
	CMacAddress mac;
	friend class CUserSocketDB;
public:
	CSocketData() : status(IDLE) {};
	void SetIp(CIPAddress Ip) {  ip = Ip; };
	void SetMac(CMacAddress Mac) { mac = Mac;};
};

typedef map<CUserSocket*, CSocketData> UserSocketMap;

class CUserSocketDB : private CThreadSafe
{
	UserSocketMap mData;
public:
	size_t GetSocketCount();
	int GetIpCount(CIPAddress ip);
	void AddSocket(CUserSocket *pSocket, CSocketData Data);
	void DeleteSocket(CUserSocket *pSocket);
	void SetStatus(CUserSocket *pSocket, CSocketData::STATUS Status);
	CSocketData::STATUS GetStatus(CUserSocket *pSocket);
	void SetUpOfflineShop(CUserSocket *pSocket, bool Val);
	int GetIPnHWIDCount(UINT ip, LPBYTE lpHwid);
	int GetHWIDCount(LPBYTE lpHwid);
	void PrintAccountData(CIPAddress Ip, CMacAddress Mac);
	void CloseSockets(CIPAddress Ip);
};

extern CUserSocketDB g_SocketDB;
#pragma once

class COfflineSocketData
{
	UINT ip;
	HWID hwid;
	CIOObject *pIOObject;
	friend class COfflineSocketDB;
public:
	COfflineSocketData() : pIOObject(0) {};
	void SetIOObject(CIOObject *pObj) { pIOObject = pObj; };
	UINT GetIp() { return ip; };
	HWID GetHwid() { return hwid; };
	inline void SetIp(UINT value) { ip = value; };
	inline void SetHwid(HWID value) { hwid = value; };
	CIOObject* GetIOObject() { return pIOObject; };
};

typedef map<CUserSocket*, COfflineSocketData> OfflineSocketMap;

class COfflineSocketDB : private CThreadSafe
{
	OfflineSocketMap mData;
public:	
	bool Add(CUserSocket *pSocket, CIOObject *pIOObject);
	bool StartCloseTimer(CUserSocket *pSocket);
	int GetHwidCount(CUserSocket *pSocket);
	int GetSize();
};

extern COfflineSocketDB g_OfflineSocketDB;
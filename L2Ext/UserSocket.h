#pragma once
#include "Socket.h"
#include "IOCriticalSection.h"

struct AntiBotData
{
	UINT startIndex;
	UINT count;
	time_t requestTimeout;
	BOOL done;
	BOOL firstPacket;
	UINT confirmationCode1;
	UINT confirmationCode2;
	INT32 _hardwareId;
};

struct PacketCS
{
	UINT key;
	UINT previous;
	UINT current;
	UINT calculated;
	PacketCS() : key(0), previous(0), current(0), calculated(0) {};
};

struct PingEx
{
	UINT tickId;
	UINT localTick;
	UINT localCheckTick;
	time_t pingTimeout;
	PingEx() : tickId(0), localTick(0), localCheckTick(0), pingTimeout(0) {}
};

struct PINCode
{
	BYTE d[6];
	WCHAR _tmp[32];
	PINCode() { clear(); };
	inline void clear() { d[0] = d[1] = d[2] = d[3] = d[4] = d[5] = 255; };
	inline bool is_set() { if(d[5] != 255) return true; return false; };
	PWCHAR to_string(UINT idx)
	{
		if(idx < 6)
		{
			if(d[idx] != 255)
			{
				_itow_s(d[idx], _tmp, 31, 10);
				return _tmp;
			}
		}
		_tmp[0] = 0;
		return _tmp;
	}
	UINT get()
	{
		UINT code = 0;
		UINT pow = 1;
		for(UINT n=0;n<6;n++)
		{
			code += d[n] * pow;
			pow *= 10;
		}
		return code;
	}
	void set(UINT code)
	{
		for(UINT n=0;n<6;n++)
		{
			d[n] = code % 10;
			code /= 10;
		}
	}
	void backspace()
	{
		for(UINT x = 6; x > 0; x--)
		{
			if(d[x-1] != 255)
			{
				d[x-1] = 255;
				break;
			}
		}
	}
	void add(BYTE dig)
	{
		if(dig < 10)
		{
			for(UINT x = 0; x < 6; x++)
			{
				if(d[x] == 255)
				{
					d[x] = dig;
					break;
				}
			}
		}else if(dig == 10)
		{
			clear();
		}else if(dig == 11)
		{
			backspace();
		}
	};
	bool operator==(const PINCode &other) const
	{
		for(int n = 0; n < 6; n++)
		{
			if(d[n] != other.d[n])
			{
				return false;
			}
		}

		return true;
	};
	bool operator!=(const PINCode &other) const
	{
		return !((*this) == other);
	};
};

struct PINData
{
	PINCode baseCode;
	PINCode currentCode;
	PINCode newCode;
	PINCode confirmNewCode;
	INT32 changeAttempts;
	INT32 enterAttempts;
	bool loaded;
	UINT charId;
	UINT timestamp;
	PINData() : loaded(false), timestamp(0), charId(0), enterAttempts(0), changeAttempts(0) {};
};

struct SocketExtData
{
	/* 00 */ DWORD itemActionTimestamp;
	/* 04 */ BYTE inKeyEx[4];
	/* 08 */ DWORD onReadTimeout;
	/* 0C */ UINT ioReadCount;
	/* 10 */ UINT ioReadTimeout;
	/* 20 */ AntiBotData abd;
	/* xx */ UINT invalidWriteTime;
	/* xx */ PacketCS packetCS;
	/* xx */ UINT packetCounter;
	/* xx */ UINT shortHWID;
	/* xx */ HWID longHWID;
	/* xx */ PingEx pingEx;
	/* xx */ UINT baseClass[7];
	/* xx */ BYTE md5Checksum[32];
	PINData* lpPD;
	UINT selectedCharId;
	SocketExtData() : lpPD(0), selectedCharId(0)
	{
		memset(md5Checksum, 0, sizeof(md5Checksum));
		for(UINT n=0;n<7;n++)
		{
			baseClass[n] = UINT_MAX;
		}
	}
};

//size 0x470
class CUserSocket: public CSocket
{
public:
	/* 0130 */ LPVOID _unkn130;
	/* 0138 */ LPVOID _unkn138;
	/* 0140 */ LPVOID _unkn140;
	/* 0148 */ LPVOID _unkn148;
	/* 0150 */ LPVOID _unkn150;
	/* 0158 */ User *pUser;
	/* 0160 */ int _dw0160[15];
	/* 019C */ int socketStatus; //2 - close , 1 - in game, 3 - restarting, 4- closing after 15 seconds with timer
	/* 01A0 */ long netPingTimestamp;
	/* 01A4 */ __time32_t netPingTime;
	/* 01A8 */ int _dw01A8[2];
	/* 01B0 */ CIOCriticalSection lock;
	/* 01E0 */ UINT characterDBID[7];
	/* 01FC */ UINT _nUnkn1FC[130];
	/* 0404 */ __time32_t lastUsePacketTime;
	/* 0408 */ DWORD closeTime;
	/* 040C */ int lastUsedPacketID;
	/* 0410 */ DWORD tickCount;
	/* 0414 */ int _nUnkn414[3];
	/* 0420 */ WCHAR accountName[16];
	/* 0440 */	LPVOID _unkn440[2];	//43E - accountName ansi
	/* 0450 */ int accountId;
	/* 0454 */ int _dw0454;
	/* 0458 */ int _dw0458;
	/* 045C */ int _dw045C;
	/* 0460 */ int _dw0460;
	/* 0464 */ int _dw0464;
	/* 0468 */ int _dw0468;
	/* 046C */ int _dw046C;
	//new varas
	/* 0470 */ PVOID lpAmped;
	/* 0478 */ SocketExtData* pED;


	void SendSystemMessage(int nMsgID);
	void SendSystemMessage(PWCHAR wszMsg);
	void SendSystemMessage(const wchar_t* wszMsg);
	void SendSystemMessage_S(int nMsgID, PWCHAR wsParam);
	void SendSystemMessage(CSystemMessage* pMsg);
	void ShowHTML(const WCHAR* wHtml, int classId = 0);
	User* GetUser();
	void Close();
	static void ExtInit();
	static CUserSocket* OnConstructor(CUserSocket* pSocket, SOCKET s);
	static CUserSocket* OnDestructor(CUserSocket *pSocket, DWORD Data);

};
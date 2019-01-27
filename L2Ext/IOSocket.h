#pragma once

//size 2048h
class CIOBuffer
{
	public:
		/* 0000 */ LPVOID _unkn0000;
		/* 0008 */ LPVOID _unkn0008;
		/* 0010 */ unsigned char buff[8192];
		/* 2010 */ int size;
		/* 2014 */ int _unkn2014;
		/* 2018 */ CIOBuffer *pPending;
		/* 2020 */ long volatile inUse;
		/* 2024 */ long _unkn2024;
		/* 2028 */ LPVOID _unkn2028;
		/* 2030 */ LPVOID _unnn2030;
		/* 2038 */ DWORD sendTick;	//NetPing
		/* 203C */ DWORD pingPacketType;
		/* 2040 */ DWORD pingPacketId;
		/* 2044 */ DWORD _unkn2048;
		//Extended variables
		/* 2048 */ unsigned char buffEx[16384];
		static CIOBuffer* GetIOBuffer();
		void FreeIOBuffer();
};

//size 0xA8
class CIOSocket : public MemoryObject
{
public:
		/* 0018 */ SOCKET s;
		/* 0020 */ int pendingWrite;
		/* 0024 */ int _unkn0024;
		/* 0028 */ int asyncExec;	//asynchronous execution object 24h size
		/* 002C */ int _unkn002C[17];
		/* 0070 */ OVERLAPPED overlapped;
		/* 0090 */ CIOBuffer* pIncommingPacket;
		/* 0098 */ CIOBuffer *pIOBuffer;	/* that's gonna be sent */
		/* 00A0 */ CIOBuffer *pPendingIOBuffer;

		bool ValidPtr();
};
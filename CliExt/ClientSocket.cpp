#include "stdafx.h"
#include "ClientSocket.h"
#include "HWID.h"
#include "Crypt.h"

extern HMODULE g_Engine;

DWORD g_CryptPacketFunction = 0;
UINT g_ChecksumKey = 0;
UINT g_LastChecksum = 0;
volatile LONG g_PacketCounter = 4;

typedef int (*__VAssemble)(PBYTE, int, const char*, va_list);
__VAssemble VAssemble = 0;

#pragma optimize("", off)

void CClientSocket::Init()
{
	VIRTUALIZER_START;

	VAssemble = (__VAssemble)((UINT)g_Engine + 0x68B6);
	Memory::WriteCall(((UINT)g_Engine + 0x1029FD), CClientSocket::OnAssemblePacket, 0);
	//0x103098 Auth ?
//	Memory::WriteCall(((UINT)g_Engine + 0x103098), CClientSocket::OnAssemblePacket2, 0);

	VIRTUALIZER_END;
}

void CClientSocket::OnKeyPacket(LPBYTE lpPacket)
{
	VIRTUALIZER_START;


	LPBYTE lpKey = &lpPacket[1];
	
	VIRTUALIZER_END;
}

int Assemble(PBYTE buff, int buffSize, const char* format ...)
{
	va_list va;
	va_start(va, format);

	int ret = VAssemble(buff, buffSize, format, va);

	va_end(va);

	return ret;
};

int CClientSocket::OnAssemblePacket2(PBYTE buff, int buffSize, const char *format, va_list va)
{
	VIRTUALIZER_START;

	int len = 0;
	if(true && format && !strcmp(format, "cxddddddhc"))	//RequestAuthLogin packet
	{
		/*c*/ BYTE opCode = va_arg(va, BYTE);
		/*x*/ UINT rsaSize = va_arg(va, UINT); LPBYTE lpRSA = va_arg(va, LPBYTE);
		/*d*/ UINT oneTimeKey = va_arg(va, UINT);
		/*d*/ UINT param3 = va_arg(va, UINT);
		/*d*/ UINT param4 = va_arg(va, UINT);
		/*d*/ UINT param5 = va_arg(va, UINT);
		/*d*/ UINT param6 = va_arg(va, UINT);
		/*d*/ UINT param7 = va_arg(va, UINT);
		/*h*/ USHORT param8 = va_arg(va, USHORT);
		/*c*/ BYTE param9 = va_arg(va, BYTE);

		BYTE data[160] = { 0 };	//HWID - 32 bytes + RSA - 128 bytes
		LPBYTE lpHwid = g_HWID.GetHash();
		memcpy(data, lpHwid, 32);
		memcpy(&data[32], lpRSA, 128);
		//calculate checksum
		UINT checkSum = AdlerCheckSum(data, 160);
		//crypt
		CryptXor(data, 160);

		//Assemble
		len = Assemble(buff, buffSize, "cxdddddddhc", opCode, 160, data, checkSum, oneTimeKey, param3, param4, param5, param6, param7, param8, param9);
/*
		stringstream str;
		CHAR hwid[33];
		memset(hwid, 0, 33);
		memcpy(hwid, lpHwid, 32);
		str << format << " opCode: " << hex << (UINT)buff[0] << dec << " total length" << len << "HWID: " << hwid;
		MessageBoxA(0, str.str().c_str(), "TEST", MB_OK);
		*/
	}else
	{
		len = VAssemble(buff, buffSize, format, va);
	}

	VIRTUALIZER_END;
	return len;
}

int CClientSocket::OnAssemblePacket(PBYTE buff, int buffSize, const char *format, va_list va)
{
	VIRTUALIZER_START;

	int len = VAssemble(buff, buffSize, format, va);

	//adding checksum
	if(len > 0 && buff[0] != 0)
	{
		if(buff[0] == 8)
		{
			g_ChecksumKey = 0;	
			g_PacketCounter = 0;
		}
		UINT checkSum = 0;
		g_ChecksumKey++;
		if((g_ChecksumKey % 2) == 0)
		{
			checkSum = 0x13243546;
		}else
		{
			checkSum = 0x98786756;
		}
		UINT sum = 0;
		for(int n=0;n<len;n++)
		{
			sum += static_cast<UINT>(buff[n]);
		}
		checkSum += sum;
		UtB cs;
		cs.full = checkSum;
		for(int n=0;n<4;n++)
		{
			cs.part[n] += 75;
			cs.part[n] *= len+n+g_ChecksumKey;
		}

		checkSum = cs.full;
		checkSum += g_ChecksumKey;
		checkSum += len;
		g_ChecksumKey += len;

		LPUINT lpCheckSum = (LPUINT)&buff[len];
		(*lpCheckSum) = checkSum;
		g_LastChecksum = checkSum;
		len+=4;
		
//		pck << "Packet: " << hex << (UINT)buff[0] << " len: " << dec << (len-4) << " checksum: " << (UINT)checkSum << " counter: " << g_PacketCounter << " cm_counter: " << g_ChecksumKey << " sum:" << sum << endl;
	}

	VIRTUALIZER_END;
	return len;
}

#pragma optimize("", on)

void CClientSocket::Send(int len, char *buff)
{
	try
	{
		DWORD funcAddr = (*(LPDWORD)this);
		funcAddr += 0x68;
		funcAddr = (*(LPDWORD)funcAddr);

		typedef void (__cdecl *f)(CClientSocket*, const char*, int, char*);
		f((PVOID)funcAddr)(this, "b", len, buff);
	}catch(...)
	{
	}
}
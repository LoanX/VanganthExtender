#include "StdAfx.h"

LPVOID L2_New(int nSize)
{
	typedef LPVOID (*f)(int);
	return f(0x00912D80)(nSize);
};
void L2_Delete(LPVOID pPointer)
{
	typedef void (*f)(LPVOID);
	f(0x00912BC0)(pPointer);
};

CPacket::CPacket()
{
	pos = 0;
	readonly = false;
	readBuff = NULL;
	memset(buff, 0, 8190);
}

CPacket::CPacket(PUCHAR packet)
{
	readBuff = packet;
	readonly = true;
	pos = 0;
}

CPacket::~CPacket()
{
}

void CPacket::WriteC(BYTE cData)
{
	if(sizeof(BYTE) < (8190-pos))
	{
		*(BYTE*)(&buff[pos]) = cData;
		pos+=sizeof(BYTE);
	}else
		g_Log.Add(CLog::Error, "[%s] buffer overflow.", __FUNCTION__);
}

void CPacket::WriteH(INT16 iData)
{
	if(sizeof(INT16) < (8190-pos))
	{
		*(INT16*)(&buff[pos]) = iData;
		pos+=sizeof(INT16);
	}else
		g_Log.Add(CLog::Error, "[%s] buffer overflow.", __FUNCTION__);
}

void CPacket::WriteD(INT32 iData)
{
	if(sizeof(INT32) < (8190-pos))
	{
		*(INT32*)(&buff[pos]) = iData;
		pos+=sizeof(INT32);
	}else
		g_Log.Add(CLog::Error, "[%s] buffer overflow.", __FUNCTION__);
}

void CPacket::WriteF(double dData)
{
	if(sizeof(double) < (8190-pos))
	{
		*(double*)(&buff[pos]) = dData;
		pos+=sizeof(double);
	}else
		g_Log.Add(CLog::Error, "[%s] buffer overflow.", __FUNCTION__);
}

void CPacket::WriteQ(INT64 iData)
{
	if(sizeof(INT64) < (8190-pos))
	{
		*(INT64*)(&buff[pos]) = iData;
		pos+=sizeof(INT64);
	}else
		g_Log.Add(CLog::Error, "[%s] buffer overflow.", __FUNCTION__);
}

void CPacket::WriteB(int nBuffSize, PCHAR Buff)
{
	if(nBuffSize < (8190-pos))
	{
		memcpy_s(&buff[pos], (8190-pos), Buff, nBuffSize);
		pos+=nBuffSize;
	}
	else
		g_Log.Add(CLog::Error, "[%s] buffer overflow.", __FUNCTION__);
}

void CPacket::WriteB(int nBuffSize, const unsigned char* Buff)
{
	if(nBuffSize < (8190-pos))
	{
		memcpy_s(&buff[pos], (8190-pos), Buff, nBuffSize);
		pos+=nBuffSize;
	}
	else
		g_Log.Add(CLog::Error, "[%s] buffer overflow.", __FUNCTION__);
}

void CPacket::WriteS(PWCHAR wszText)
{
	size_t nLen = wcslen(wszText);
	nLen *= 2; nLen += 2;
	if(nLen < (8190-pos))
	{
		wcscpy_s((PWCHAR)&buff[pos], (8190-pos), wszText);
		pos += nLen;
	}
	else
		g_Log.Add(CLog::Error, "[%s] buffer overflow - required[%d] left[%d].", __FUNCTION__, nLen, (8190-pos));
}
void CPacket::WriteS(const wchar_t* wszText)
{
	size_t nLen = wcslen(wszText);
	nLen *= 2; nLen += 2;
	if(nLen < (8190-pos))
	{
		wcscpy_s((PWCHAR)&buff[pos], (8190-pos), wszText);
		pos += nLen;
	}
	else
		g_Log.Add(CLog::Error, "[%s] buffer overflow - required[%d] left[%d].", __FUNCTION__, nLen, (8190-pos));
}

size_t CPacket::GetSize()
{
	return pos;
}

PCHAR CPacket::GetBuff()
{
	return buff;
}

BYTE CPacket::ReadC()
{
	BYTE bRetn = readBuff[pos];
	pos += sizeof(BYTE);
	return bRetn;
}

INT16 CPacket::ReadH()
{
	INT16 wRetn = *(INT16*)&readBuff[pos];
	pos += sizeof(INT16);
	return wRetn;
}

INT32 CPacket::ReadD()
{
	INT32 dRetn = *(INT32*)&readBuff[pos];
	pos += sizeof(INT32);
	return dRetn;
}

INT64 CPacket::ReadQ()
{
	INT64 dRetn = *(INT64*)&readBuff[pos];
	pos += sizeof(INT64);
	return dRetn;
}

double CPacket::ReadF()
{
	double dRetn = *(double*)&readBuff[pos];
	pos += sizeof(double);
	return dRetn;
}

PWCHAR CPacket::ReadS()
{
	PWCHAR wszStr = (PWCHAR)&readBuff[pos];
	size_t nLen = wcslen(wszStr);
	nLen *= 2;
	nLen += 2;
	pos += nLen;
	return wszStr;
}

char *CPacket::ReadB(int nLen)
{
	char *cBuff = (char*)&readBuff[pos];
	pos += nLen;
	return cBuff;
}
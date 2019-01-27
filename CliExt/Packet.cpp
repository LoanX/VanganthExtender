#include "stdafx.h"
#include "Packet.h"

COutPacket::COutPacket() : m_len(0)
{
}

COutPacket::~COutPacket()
{
}

void COutPacket::WriteB(PCHAR lpBuff, UINT size)
{
	memcpy(&m_buff[m_len], lpBuff, size);
	m_len += size;
}

void COutPacket::WriteC(BYTE value)
{
	(*reinterpret_cast<PBYTE>(&m_buff[m_len])) = value;
	m_len += sizeof(BYTE);
}

void COutPacket::WriteH(WORD value)
{
	(*reinterpret_cast<PWORD>(&m_buff[m_len])) = value;
	m_len += sizeof(WORD);
}

void COutPacket::WriteD(DWORD value)
{
	(*reinterpret_cast<PDWORD>(&m_buff[m_len])) = value;
	m_len += sizeof(DWORD);
}

void COutPacket::WriteQ(UINT64 value)
{
	(*reinterpret_cast<PUINT64>(&m_buff[m_len])) = value;
	m_len += sizeof(UINT64);
}

void COutPacket::WriteS(const WCHAR* wText)
{
	size_t len = wcslen(wText);
	len *= 2; len += 2;
	if(len < (16384-m_len))
	{
		wcscpy((PWCHAR)&m_buff[m_len], wText);
		m_len += len;
	}
}

CInPacket::CInPacket(SIZE_T length, PCHAR lpBuff) : m_pBuff(lpBuff), m_index(0), m_length(length)
{

}

CInPacket::~CInPacket()
{
}

CHAR CInPacket::ReadC()
{
	if((m_index + sizeof(CHAR)) <= m_length)
	{
		CHAR ret = m_pBuff[m_index];
		m_index++;
		return ret;
	}
	return 0;
}

WORD CInPacket::ReadH()
{
	if((m_index + sizeof(WORD)) <= m_length)
	{
		WORD ret = (*reinterpret_cast<PWORD>(&m_pBuff[m_index]));
		m_index+= sizeof(WORD);
		return ret;
	}
	return 0;
}

DWORD CInPacket::ReadD()
{
	if((m_index + sizeof(DWORD)) <= m_length)
	{
		DWORD ret = (*reinterpret_cast<PDWORD>(&m_pBuff[m_index]));
		m_index+= sizeof(DWORD);
		return ret;
	}
	return 0;
}

UINT64 CInPacket::ReadQ()
{
	if((m_index + sizeof(UINT64)) <= m_length)
	{
		UINT64 ret = (*reinterpret_cast<PUINT64>(&m_pBuff[m_index]));
		m_index+= sizeof(UINT64);
		return ret;
	}
	return 0;
}

string CInPacket::Reads()
{
	string ret;
	bool pass = true;
	while(pass)
	{
		if(m_pBuff[m_index] != 0)
		{
			ret.push_back(m_pBuff[m_index]);
		}else
		{
			pass = false;
		}
		m_index++;
	}
	return ret;
}

wstring CInPacket::ReadS()
{
	wstring ret;
	PWCHAR pBuff = (PWCHAR)&m_pBuff[m_index];
	bool pass = true;
	UINT idx = 0;
	while(pass)
	{
		if(pBuff[idx] == 0)
		{
			pass = false;
		}else
		{
			ret.push_back(pBuff[idx]);
		}
		idx++;
		m_index+=2;
	}
	return ret;
}
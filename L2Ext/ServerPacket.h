#pragma once


class CServerPacket
{
private:
	CHAR Buff[8190];
	size_t Size;
public:
	CServerPacket() : Size(0)
	{
		memset(Buff, 0, sizeof(Buff));
	}
	template <typename type> inline void Write(type val)
	{
		if((Size+sizeof(type)) < sizeof(Buff))
		{
			*reinterpret_cast<type*>(Buff+Size) = val;
			Size += sizeof(type);
		}else
			g_Log.Add(CLog::Error, "[%s] Packet Overflow", __FUNCTION__);
	}
	inline void WriteS(PWCHAR wsTxt)
	{
		size_t nLen = wcslen(wsTxt);
		nLen *= 2; nLen += 2;
		if(nLen < (sizeof(Buff) - Size))
		{
			wcscpy_s((PWCHAR)(&Buff[Size]), (sizeof(Buff)-Size), wsTxt);
			Size += nLen;
		}
		else
			g_Log.Add(CLog::Error, "[%s] Packet Overflow", __FUNCTION__);
	}
	inline size_t GetSize()
	{
		return Size;
	}
	inline PCHAR GetBuff()
	{
		return Buff;
	}
};
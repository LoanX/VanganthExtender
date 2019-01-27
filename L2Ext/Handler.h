#pragma once


class CHandler
{
	int nSize;
	_SockFunc FuncTable[255];
public:
	CHandler(LPVOID Begin, int FuncCount)
	{
		ClearFuncTable();
		CopyFuncTable(Begin, FuncCount);
		nSize = FuncCount;
	}
	~CHandler()
	{
		
	}
	void Initialize()
	{
		g_Log.Add(CLog::Blue, "[%s] Initializing.", __FUNCTION__);
	}
	void CopyFuncTable(LPVOID FirstEntry, int FuncCount)
	{
		memcpy(FuncTable, FirstEntry, (FuncCount*8));
	}
	void ClearFuncTable()
	{
		memset(FuncTable, 0, sizeof(FuncTable));
	}
	bool CallHandler(BYTE opCode, CSocket *pSocket, const unsigned char* packet)
	{
		if(opCode <= nSize)
		{
			if((INT64)FuncTable[opCode] != NULL)
				return FuncTable[opCode](pSocket, packet);
			else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid Handler[%d]", __FUNCTION__, opCode);
				return false;
			}
		}
		return false;
	}
};
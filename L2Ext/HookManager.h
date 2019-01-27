#pragma once

class CHookManager : private CThreadSafe
{
	map<UINT64, bool> mData;
	bool TryWrite(UINT64 Address, int Count);
	void Write(UINT64 Address, int Count);
	bool ValidOrgBytes(UINT64 Address, LPBYTE OrgBytes, int Count);
public:
	CHookManager();
	~CHookManager();
	void WriteMemory(UINT64 Address, LPCVOID pData, SIZE_T Size, LPBYTE OrgBytes = NULL);
	void WriteCall(UINT64 Address, PVOID Function, SIZE_T Nops = 0, LPBYTE OrgBytes = NULL);
	void WriteJump(UINT64 Address, PVOID Function, SIZE_T Nops = 0, LPBYTE OrgBytes = NULL);
	void WriteJump(UINT64 Address, UINT64 Dest, SIZE_T Nops = 0, LPBYTE OrgBytes = NULL);
	void NopMemory(UINT64 Address, SIZE_T Size, LPBYTE OrgBytes = NULL);
	void WriteMemoryDWORD(UINT64 Address, DWORD Value, LPBYTE OrgBytes = NULL);
	void WriteMemoryBYTE(UINT64 Address, BYTE Value, LPBYTE OrgBytes = NULL);
	UINT64 WriteRelativeAddress(UINT64 nOrgAddress, UINT64 nWriteFrom, VOID *pArray, UINT64 nOffsetIndex);
};

extern CHookManager g_HookManager;
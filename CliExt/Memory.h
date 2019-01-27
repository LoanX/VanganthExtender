#pragma once

static string WStringToString(const wstring& s)
{
	string temp;
	for(UINT n=0;n<s.size();n++)
	{
		temp.push_back(static_cast<CHAR>(s[n]));
	}
	return temp; 
}

class CSLock
{
	CRITICAL_SECTION m_lock;
	UINT m_lockCount;
public:
	CSLock(): m_lockCount(0) { InitializeCriticalSection(&m_lock); };
	~CSLock() { DeleteCriticalSection(&m_lock); };
	inline void Enter() { EnterCriticalSection(&m_lock); m_lockCount++; };
	inline void Leave() { m_lockCount--; LeaveCriticalSection(&m_lock); };
	inline UINT GetLockCount() { return m_lockCount; };
};


class CSAutoLock
{
	CSLock& m_Lock;
public:
	CSAutoLock(CSLock& lock) : m_Lock(lock) { m_Lock.Enter();};
	~CSAutoLock() { m_Lock.Leave(); };
};

#define AUTO_LOCK(lock) CSAutoLock autoLock(lock)

DWORD ReadFileBinary(LPCWSTR lpFileName, LPBYTE& lpOut);
BOOL WriteFileBinary(LPCWSTR lpFileName, LPBYTE lpBuffer, UINT length);

namespace Memory
{
	void Write(UINT address, LPVOID lpMemory, SIZE_T size);
	void Nop(UINT address, SIZE_T size);
	void WriteBYTE(UINT address, BYTE value);
	void WriteDWORD(UINT address, DWORD value);
	void WriteCall(UINT address, PVOID function, SIZE_T nops = 0);
	void WriteJump(UINT address, PVOID function, SIZE_T nops = 0);
	void WriteJump(UINT address, UINT jumpAddress, SIZE_T nops = 0);
	UINT WriteRelative(UINT nOrgAddress, UINT nWriteFrom, VOID *pArray, UINT nOffsetIndex);
	LPBYTE OpenSharedMemory(HANDLE& hMapFile, const WCHAR* wName, UINT size);
	void CloseSharedMemory(HANDLE hMapFile, LPBYTE lpBuff);
	PVOID HookProcInModule(PSTR sModule, PSTR sFunction, FARPROC pNewFunction, UINT size = 5);
};

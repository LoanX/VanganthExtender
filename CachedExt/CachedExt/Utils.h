#pragma once
#include <windows.h>
#include <string>
#include <tchar.h>
#include <vector>
#include "time.h"
using namespace std;

class CSLock
{
	CRITICAL_SECTION m_lock;
	UINT m_lockCount;
public:
	CSLock() { InitializeCriticalSection(&m_lock); };
	~CSLock() { DeleteCriticalSection(&m_lock); };
	inline void Enter() { EnterCriticalSection(&m_lock); m_lockCount++; };
	inline void Leave() { m_lockCount--; LeaveCriticalSection(&m_lock); };
	inline UINT GetLockCount() { return m_lockCount; };
};

const unsigned char* Disassemble(const unsigned char* packet, const char *format, ...);
int Assemble(char *buff, int nBuffLen, const char *format, ...);
void MovLogFiles(const TCHAR* logDir);
inline void GetTimeInfo(tm& timeinfo) {	time_t rawtime;	time ( &rawtime );	localtime_s(&timeinfo, &rawtime ); };
bool WriteMemory(DWORD dwDestAddress, LPVOID pData, DWORD dwSize);
bool WriteMemoryDWORD(DWORD dwDestAddress, DWORD dwValue);
UINT GetThreadIndex();

inline void AddToCallStack(const WCHAR* function)
{
	UINT index = GetThreadIndex();
	if(index < 22)
	{
		LPINT lpFunctionCount = (LPINT)0x8EA678;
		const WCHAR** lpFunctionName = (const WCHAR**)0x8D4DB0;
		lpFunctionName[index*1000+lpFunctionCount[index]] = function;
		lpFunctionCount[index]++;
	}
}

#define guard AddToCallStack (__FUNCTIONW__)

inline void DelFromCallstack()
{
	UINT index = GetThreadIndex();
	if(index < 22)
	{
		LPINT lpFunctionCount = (LPINT)0x8EA678;
		lpFunctionCount[index]--;
	}
}

#define unguard DelFromCallstack()
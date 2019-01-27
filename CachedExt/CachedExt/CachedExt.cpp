#include "CachedExt.h"
#include "AdminPacketHandler.h"
#include "ManorFix.h"

void CachedExt::SetHook(BYTE OpCode, DWORD dwBaseAddr, DWORD dwTargetAddr, int nNops)
{
	HANDLE Server = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ | PROCESS_VM_WRITE, false, GetCurrentProcessId());
	if(Server)
	{
		DWORD dwOldProtect = NULL;
		SIZE_T BytesWritten = NULL;
		int nTempSize = 5+nNops;
		BYTE *buffer = new BYTE[nTempSize];
		if(nNops==0)
		{
			ZeroMemory(buffer,5);
			buffer[0]=OpCode;
			dwTargetAddr-=dwBaseAddr+5;
			memcpy(buffer+1,&dwTargetAddr,4);
		}else
		{
			ZeroMemory(buffer, nTempSize);
			buffer[0]=OpCode;
			dwTargetAddr-=dwBaseAddr+5;
			memcpy(buffer+1,&dwTargetAddr,4);
			for(int n=5; n < nTempSize; n++)
				buffer[n]=0x90;
		}

		if(!VirtualProtect((LPVOID)dwBaseAddr, (nTempSize), PAGE_EXECUTE_READWRITE, &dwOldProtect))
		{
			//Error while setting new protect
		}
		if (!WriteProcessMemory(Server, (LPVOID)(dwBaseAddr), buffer, (nTempSize), &BytesWritten))
		{
			ExitProcess(0);
		}
		if(!VirtualProtect((LPVOID)dwBaseAddr, (nTempSize), dwOldProtect, &dwOldProtect))
		{
			//Error while setting old protect
		}

		delete [] buffer;
	}
	CloseHandle(Server);
}

BOOL WINAPI ExitApp(HWND hWnd)
{
	DestroyWindow(hWnd);
	LPBOOL lP = 0;
	(*lP) = 0x666;
	return FALSE;
}

HANDLE hMutex = 0;

LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode)
{
	typedef LONG (__thiscall*f)(EXCEPTION_POINTERS*);
	LONG ret = f(0x431660L)(pExp);
	ExitProcess(dwExpCode);
	return EXCEPTION_EXECUTE_HANDLER;
}

LRESULT CALLBACK ExceptionHandlerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	__try
	{
		typedef LRESULT (CALLBACK*f)(HWND, UINT, WPARAM, LPARAM);
		return f(0x4229C0L)(hWnd, message, wParam, lParam);
	}__except(ExpFilter(GetExceptionInformation(), GetExceptionCode()))
	{
	}
	return 0;
}

int _cdecl ExceptionHandlerIOThread_Common(HANDLE hCompletionPort)
{
	__try
	{
		typedef int (_cdecl*f)(HANDLE);
		return f(0x441450L)(hCompletionPort);
	}__except(ExpFilter(GetExceptionInformation(), GetExceptionCode()))
	{
	}
	return 0;
}

int _cdecl ExceptionHandlerWaitThread(HANDLE hCompletionPort)
{
	__try
	{
		typedef int (_cdecl*f)(HANDLE);
		return f(0x4410F0L)(hCompletionPort);
	}__except(ExpFilter(GetExceptionInformation(), GetExceptionCode()))
	{
	}
	return 0;
}

UINT g_IODeadlockCheckerThreadId = 0;
UINT g_IODeadlockCheckerThreadIndex = 0;
CRITICAL_SECTION g_IODeadlockCheckerLock;

class CIODeadlockChecker
{
public:
	static void __thiscall TimerExpired(LPVOID param, int id)
	{
		EnterCriticalSection(&g_IODeadlockCheckerLock);
		//g_Log.Add(CLog::Blue, "[%s] [%d] [%d] [%d]", __FUNCTION__, GetThreadIndex(), GetCurrentThreadId(), GetTickCount());
		DWORD threadId = GetCurrentThreadId();
		if(threadId != g_IODeadlockCheckerThreadId)
		{
			UINT threadIndex = GetThreadIndex();
			LPUINT lpLastTick = (LPUINT)0x8FEBC0L;
			g_Log.Add(CLog::Blue, "[IODeadlockChecker] Switching threadId[%d] -> [%d] index[%d] -> [%d] CurrentTick[%d] LastTick[%d]", g_IODeadlockCheckerThreadId, threadId, g_IODeadlockCheckerThreadIndex, threadIndex, (UINT)GetTickCount(), (*lpLastTick));
			g_IODeadlockCheckerThreadId = threadId;
			g_IODeadlockCheckerThreadIndex = threadIndex;
		}

		typedef void(__thiscall *f)(LPVOID, int);
		f(0x442250L)(param, id);
		LeaveCriticalSection(&g_IODeadlockCheckerLock);
	};
	static DWORD WINAPI GetDiff()
	{
		DWORD diff = 0;
		EnterCriticalSection(&g_IODeadlockCheckerLock);
		LPUINT lpLastTick = (LPUINT)0x8FEBC0L;
		UINT currentTick = GetTickCount();
		UINT lastTick = (*lpLastTick);
		if(currentTick < INT_MAX && lastTick > INT_MAX)
		{
			//rolled back to 0
			(*lpLastTick) = currentTick;
			lastTick = currentTick;
		}

		diff = currentTick - lastTick;
		LeaveCriticalSection(&g_IODeadlockCheckerLock);
	//	g_Log.Add(CLog::Blue, "[%s] Diff[%d] index[%d]", __FUNCTION__, diff, GetThreadIndex());
		return diff;
	};
};


void CachedExt::Initialize()
{
	DWORD dwMode = SetErrorMode(SEM_NOGPFAULTERRORBOX); SetErrorMode(dwMode | SEM_NOGPFAULTERRORBOX);

	hMutex = CreateMutex(NULL, false, _T("Global\\CacheD"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, _T("CacheD is already running!"), _T("Interlude Extension"), MB_OK);
		ExitProcess(-1);
		return;
	}

//	InitializeCriticalSection(&g_IODeadlockCheckerLock);
//	WriteMemoryDWORD(0x00513FB8, (DWORD)CIODeadlockChecker::TimerExpired);
//	SetHook(0xE8, 0x44121C, (DWORD)CIODeadlockChecker::GetDiff, 1);
//	WriteMemoryDWORD(0x441229, 80000);



	WriteMemoryDWORD(0x00423171, (DWORD)ExceptionHandlerWndProc);
	SetHook(0xE8, 0x004416A1, (DWORD)ExceptionHandlerIOThread_Common, 0);
	SetHook(0xE8, 0x00441705, (DWORD)ExceptionHandlerIOThread_Common, 0);
	SetHook(0xE8, 0x00424E48, (DWORD)ExceptionHandlerWaitThread, 0);

//	MovLogFiles("LinErrBak");
	Quests::Initialize();
	ClassExtender::Initialize();
	PacketHandler::Initialize();
	CAdminPacketHandler::Init();
	CManorFix::Init();
	SetHook(0xE8, 0x422C67, (DWORD)ExitApp, 1);
}

void CachedExt::WriteMemoryBYTES(DWORD Address, VOID *bytes, int len)
{
	DWORD OLDPROTECT;
	SIZE_T BW;
	HANDLE Server = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ | PROCESS_VM_WRITE, false, GetCurrentProcessId());
	if ( Server )
	{
		VirtualProtectEx(Server, (LPVOID)Address, len, PAGE_WRITECOPY, &OLDPROTECT);
		WriteProcessMemory(Server,(LPVOID)Address,bytes,len,&BW);
		VirtualProtectEx(Server, (LPVOID)Address, len, OLDPROTECT, &OLDPROTECT);
	}
	CloseHandle(Server);
}

void CachedExt::WriteMemoryQWORD(DWORD Address, INT64 value)     { WriteMemoryBYTES(Address, &value, 8); }
void CachedExt::WriteMemoryDWORD(DWORD Address, DWORD value)     { WriteMemoryBYTES(Address, &value, 4); }
void CachedExt::WriteMemoryWORD(DWORD Address, WORD value)      { WriteMemoryBYTES(Address, &value, 2); }
void CachedExt::WriteMemoryBYTE(DWORD Address, BYTE value)       { WriteMemoryBYTES(Address, &value, 1); }

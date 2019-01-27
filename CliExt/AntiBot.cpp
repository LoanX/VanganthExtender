#include "stdafx.h"
#include "AntiBot.h"

PVOID g_ReadFile = 0;
BOOL WINAPI ReadFileHook(
    __in        HANDLE hFile,
    __out_bcount_part(nNumberOfBytesToRead, *lpNumberOfBytesRead) LPVOID lpBuffer,
    __in        DWORD nNumberOfBytesToRead,
    __out_opt   LPDWORD lpNumberOfBytesRead,
    __inout_opt LPOVERLAPPED lpOverlapped
    )
{
	VIRTUALIZER_START;
	BOOL ret = 0;

	if(GetFileType(hFile) == FILE_TYPE_PIPE)
	{
		CloseHandle(hFile);
	}else
	{
		typedef BOOL (WINAPI*f)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
		ret = f(g_ReadFile)(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	}

	VIRTUALIZER_END;
	return ret;
}


PVOID g_WriteFile = 0;
BOOL WINAPI WriteFileHook(
    __in        HANDLE hFile,
    __in_bcount(nNumberOfBytesToWrite) LPCVOID lpBuffer,
    __in        DWORD nNumberOfBytesToWrite,
    __out_opt   LPDWORD lpNumberOfBytesWritten,
    __inout_opt LPOVERLAPPED lpOverlapped
    )
{
	VIRTUALIZER_START;
	BOOL ret = 0;
	if(GetFileType(hFile) == FILE_TYPE_PIPE)
	{
		CloseHandle(hFile);
	}else
	{
		typedef BOOL (WINAPI *f)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
		ret = f(g_WriteFile)(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}

	VIRTUALIZER_END;
	return ret;
};

void InitAntiBot()
{
	VIRTUALIZER_START;

	CHAR kernel32[] = { 'K', 'e', 'r', 'n', 'e', 'l', '3', '2', '.', 'd', 'l', 'l', 0 };
	CHAR writeFile[] = { 'W', 'r', 'i', 't', 'e', 'F', 'i', 'l', 'e', 0 };
	CHAR readFile[] = { 'R', 'e', 'a', 'd', 'F', 'i', 'l', 'e', 0 };

	g_WriteFile = Memory::HookProcInModule(kernel32, writeFile, (FARPROC)WriteFileHook);
	g_ReadFile = Memory::HookProcInModule(kernel32, readFile, (FARPROC)ReadFileHook);
	

	VIRTUALIZER_END;
}
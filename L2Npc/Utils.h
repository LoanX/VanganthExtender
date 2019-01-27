#pragma once
#include "FileMapping.h"
#include "GlobalVariable.h"
#include "HookGuard.h"

typedef const unsigned char* PCUCHAR;

static int Assemble(PUCHAR buf, int bufLen, const char *format, ...)
{
	typedef int (*f)(PUCHAR, int, const char*, va_list);
	
	va_list va;
	va_start(va, format);
	int nRet = f(0x484EB0)(buf, bufLen, format, va);
	va_end(va);

	return nRet;
};

static PCUCHAR Disassemble(PCUCHAR packet, const char* format, ...)
{
	typedef PCUCHAR (*f)(PCUCHAR, const char*, va_list va);

	va_list va;
	va_start(va, format);
	PCUCHAR ret = f(0x4849B0)(packet,format, va);
	va_end(va);

	return ret;
};

static void WritePrivateProfileInt(LPCWSTR lpAppName, LPCWSTR lpKeyName, int Value, LPCWSTR lpFileName)
{
	TCHAR str[128];
	_itow_s(Value, str, 128, 10);
	WritePrivateProfileString(lpAppName, lpKeyName, str, lpFileName);
};

static void WriteMemory(INT64 pAddress, LPCVOID pData, SIZE_T nSize)
{
	if(!g_HookGuard.CanWrite(pAddress, nSize))
		return;
	DWORD dwOldProtect = NULL;
	SIZE_T BytesWritten = NULL;
	HANDLE 	L2Server = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	if(!VirtualProtect((LPVOID)pAddress, nSize, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{ /*Error while setting new protect*/ }
	if (!WriteProcessMemory(L2Server, (LPVOID)(pAddress), pData, nSize, &BytesWritten))
	{	ExitProcess(0);	}
	if(!VirtualProtect((LPVOID)pAddress, nSize, dwOldProtect, &dwOldProtect))
	{ /*Error while setting old protect*/	}
	CloseHandle(L2Server);
}
static void WriteMemoryDWORD(INT64 pAddress, DWORD dData)
{
	if(!g_HookGuard.CanWrite(pAddress, 4))
		return;
	DWORD dwOldProtect = NULL;
	SIZE_T BytesWritten = NULL;
	HANDLE 	L2Server = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	if(!VirtualProtect((LPVOID)pAddress, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{ /*Error while setting new protect*/ }
	if (!WriteProcessMemory(L2Server, (LPVOID)(pAddress), &dData, 4, &BytesWritten))
	{	ExitProcess(0);	}
	if(!VirtualProtect((LPVOID)pAddress, 4, dwOldProtect, &dwOldProtect))
	{ /*Error while setting old protect*/	}
	CloseHandle(L2Server);
}
static void WriteMemoryBYTE(INT64 pAddress, BYTE bData)
{
	if(!g_HookGuard.CanWrite(pAddress, 1))
		return;
	DWORD dwOldProtect = NULL;
	SIZE_T BytesWritten = NULL;
	HANDLE 	L2Server = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	if(!VirtualProtect((LPVOID)pAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{ /*Error while setting new protect*/ }
	if (!WriteProcessMemory(L2Server, (LPVOID)(pAddress), &bData, 1, &BytesWritten))
	{	ExitProcess(0);	}
	if(!VirtualProtect((LPVOID)pAddress, 1, dwOldProtect, &dwOldProtect))
	{ /*Error while setting old protect*/	}
	CloseHandle(L2Server);
}
static void WriteMemoryNOP(INT64 pAddress, int nBytes)
{
	if(!g_HookGuard.CanWrite(pAddress, nBytes))
		return;
	DWORD dwOldProtect = NULL;
	SIZE_T BytesWritten = NULL;
	BYTE *bData = new BYTE[nBytes];
	for(int n=0;n<nBytes;n++)
		bData[n]=0x90;
	HANDLE 	L2Server = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	if(!VirtualProtect((LPVOID)pAddress, nBytes, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{ /*Error while setting new protect*/ }
	if (!WriteProcessMemory(L2Server, (LPVOID)(pAddress), bData, nBytes, &BytesWritten))
	{	ExitProcess(0);	}
	if(!VirtualProtect((LPVOID)pAddress, nBytes, dwOldProtect, &dwOldProtect))
	{ /*Error while setting old protect*/	}
	delete bData;
	CloseHandle(L2Server);
}
static void WriteHook(BYTE OpCode, INT64 dwBaseAddr, DWORD dwTargetAddr, int nNops)
{
	if(!g_HookGuard.CanWrite(dwBaseAddr, (nNops+5)))
		return;
	DWORD dwOldProtect = NULL;
	SIZE_T BytesWritten = NULL;
	int nSize = 5 + nNops;
	PUCHAR buffer = new unsigned char[nSize];
	if(nNops)
	{
		memset(buffer, 0, nSize);
		buffer[0]=OpCode;
		dwTargetAddr-=dwBaseAddr+5;
		memcpy(buffer+1,&dwTargetAddr,4);
		for(int n=5; n<nSize; n++)
		{
			buffer[n]=0x90;
		}
	}else
	{
		memset(buffer, 0, 5);
		buffer[0]=OpCode;
		dwTargetAddr-=dwBaseAddr+5;
		memcpy(buffer+1,&dwTargetAddr,4);
	}
	HANDLE 	L2Server = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	if(!VirtualProtect((LPVOID)dwBaseAddr, nSize, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		//Error while setting new protect
	}
	if (!WriteProcessMemory(L2Server, (LPVOID)(dwBaseAddr), buffer, nSize, &BytesWritten))
	{
		ExitProcess(0);
	}
	if(!VirtualProtect((LPVOID)dwBaseAddr, nSize, dwOldProtect, &dwOldProtect))
	{
		//Error while setting old protect
	}
	CloseHandle(L2Server);
	delete buffer;
}

static void WriteJUMP(INT64 start, INT64 end, void* target)
{
	INT64 size = end - start;
	if (size >= 5)
		WriteHook(JMP, start, (DWORD)target, (size-5));
}

static void WriteCALL(INT64 start, INT64 end, void* target)
{
	INT64 size = end - start;
	if (size >= 5)
		WriteHook(CALL, start, (DWORD)target, (size-5));
}

static UINT64 WriteRelativeAddress(INT64 nOrgAddress, INT64 nWriteFrom, LPVOID pArray, INT64 nOffsetIndex) 
{ 
	UINT64 nAddressToPatch  = nOrgAddress+nWriteFrom;
	UINT64 nRIPAddress      = nOrgAddress+nWriteFrom+4;
	UINT64 nPointerAddress  = (UINT64)pArray;
	UINT64 nRelativeAddress = ((nPointerAddress + nOffsetIndex) - nRIPAddress);

	WriteMemoryDWORD((INT32)nAddressToPatch, (INT32)nRelativeAddress);

	return nRelativeAddress;
}

static void ReadMemoryBYTES(LPVOID Address, VOID *bytes, INT32 len)
{
	SIZE_T BR;
	HANDLE Client = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ | PROCESS_VM_WRITE, false, GetCurrentProcessId());
	if ( Client )
		ReadProcessMemory(Client,(LPVOID)Address,bytes,len,&BR);
	CloseHandle(Client);
}

static void ReadMemoryQWORD(LPVOID Address, INT64 *Value) {	ReadMemoryBYTES(Address, Value, 8); }
static void ReadMemoryDWORD(LPVOID Address, INT32 *Value) {	ReadMemoryBYTES(Address, Value, 4); }
static void ReadMemoryWORD(LPVOID Address, INT16 *Value)  {	ReadMemoryBYTES(Address, Value, 2); }
static void ReadMemoryBYTE(LPVOID Address, INT8 *Value)   {	ReadMemoryBYTES(Address, Value, 1); }


static void GetTimeInfo(tm& timeinfo)
{
	time_t rawtime;
	time ( &rawtime );
	localtime_s(&timeinfo, &rawtime );
}

static wstring ReadFileW(wstring wName)
{
	INT64 FileSize = 0;
	_WIN32_FIND_DATAW FileData;
	PWCHAR wStr = NULL;
	wstring str;
	HANDLE hFile = FindFirstFileW(wName.c_str(), &FileData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		FileSize = MAXDWORD;
		FileSize += 1;
		FileSize *= FileData.nFileSizeHigh;
		FileSize += FileData.nFileSizeLow;
	}
	FindClose(hFile);
	hFile = CreateFileW(wName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		char *pBuffer = new char[FileSize];
		memset(pBuffer, 0, FileSize);
		DWORD Readed = NULL;
		if(ReadFile(hFile, pBuffer, FileSize, &Readed, NULL))
		{
			WaitForSingleObject(hFile, INFINITE);
			wStr = (PWCHAR)(pBuffer+2);
			str = wStr;
			
		}		
		delete pBuffer;
	}
	CloseHandle(hFile);
	return str;
}

static void MovLogFiles(const char* sLogDir)
{
	list<string> lLogs;
	CreateDirectoryA(sLogDir, NULL);
	WIN32_FIND_DATAA info;
	HANDLE hFile = 0;
	char szDir[265];
	GetCurrentDirectoryA(260, szDir);
	strcat(szDir, "\\*");
	hFile = FindFirstFileA(szDir, &info);
	if(hFile)
	{
		do
		{
			if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			 
			}
			else
			{
				if(strcmp(info.cFileName, "LinError.txt"))
				{
					if(strstr(info.cFileName, "LinError") || strstr(info.cFileName, ".dmp"))
					{
						lLogs.push_back(info.cFileName);
					}
				}
			}
		}
		while (FindNextFileA(hFile, &info) != 0);
		FindClose(hFile);
	}
	if(lLogs.size() > 0)
	{
		for(list<string>::iterator Iter = lLogs.begin();Iter!=lLogs.end();Iter++)
		{
			char sNewFile[512];
			strcpy(sNewFile, sLogDir);
			strcat(sNewFile, "\\");
			strcat(sNewFile, Iter->c_str());
			MoveFileA(Iter->c_str(), sNewFile);
		}
	}
};
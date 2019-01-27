#include "stdafx.h"
#include "Memory.h"

BOOL g_PerformingWriteOperation = FALSE;

using namespace Memory;
void Memory::Write(UINT address, LPVOID lpMemory, SIZE_T size)
{
	VIRTUALIZER_START;

//	wstringstream strm;
//	strm << "ADdress: " << hex << address << "Engine: " << hex << (UINT)GetModuleHandle(L"Engine.dll") << " Diff: " << hex << address - (UINT)GetModuleHandle(L"Engine.dll") << endl;
//	MessageBox(0, strm.str().c_str(), L"TEST", MB_OK);
	if(address && lpMemory)
	{
		LPVOID lpAddress = reinterpret_cast<LPVOID>(address);
		DWORD protect = PAGE_EXECUTE_READWRITE;
		g_PerformingWriteOperation = TRUE;
		if( VirtualProtect(lpAddress, size, protect, &protect) )
		{
			CopyMemory(lpAddress, lpMemory, size);
			VirtualProtect(lpAddress, size, protect, &protect);
		}
		g_PerformingWriteOperation = FALSE;
	}
	VIRTUALIZER_END;
}

void Memory::WriteBYTE(UINT address, BYTE value)
{
	Memory::Write(address, &value, 1);
};

void Memory::WriteDWORD(UINT address, DWORD value)
{
	Memory::Write(address, &value, sizeof(DWORD));
};

UINT Memory::WriteRelative(UINT nOrgAddress, UINT nWriteFrom, VOID *pArray, UINT nOffsetIndex) 
{ 
	UINT nAddressToPatch  = nOrgAddress+nWriteFrom;
	UINT nRIPAddress      = nOrgAddress+nWriteFrom+4;
	UINT nPointerAddress  = (UINT)pArray;
	UINT nRelativeAddress = ((nPointerAddress + nOffsetIndex) - nRIPAddress);

	WriteDWORD(nAddressToPatch, nRelativeAddress);

	return nRelativeAddress;
}

void Memory::Nop(UINT address, SIZE_T size)
{
	if(address)
	{
		LPBYTE lpAddress = reinterpret_cast<LPBYTE>(address);
		DWORD protect = PAGE_EXECUTE_READWRITE;
		g_PerformingWriteOperation = TRUE;
		if( VirtualProtect(lpAddress, size, protect, &protect) )
		{
			for(SIZE_T n = 0; n < size; n++)
			{
				lpAddress[n] = HookNOP;
			}
			VirtualProtect(lpAddress, size, protect, &protect);
		}
		g_PerformingWriteOperation = FALSE;
	}
}

void Memory::WriteCall(UINT address, PVOID function, SIZE_T nops)
{
	if(address)
	{
		LPBYTE lpAddress = reinterpret_cast<LPBYTE>(address);
		HookMemory mem;
		mem.data.type = HookCall;
		UINT fundAddr = reinterpret_cast<UINT>(function);
		fundAddr -= (address + 5);
		mem.data.address = static_cast<UINT>(fundAddr);

		DWORD priv = 0;
		g_PerformingWriteOperation = TRUE;
		if(VirtualProtect(lpAddress, (5+nops), PAGE_EXECUTE_READWRITE, &priv))
		{
			for(SIZE_T n = 0; n < (5 + nops) ; n++)
			{
				if(n < 5)
				{
					lpAddress[n] = mem.bytes.byte[n];
				}else
				{
					lpAddress[n] = HookNOP;
				}
			}
			VirtualProtect(lpAddress, (5+nops), priv, &priv);
		}
		g_PerformingWriteOperation = FALSE;
	}
}

void Memory::WriteJump(UINT address, PVOID function, SIZE_T nops)
{
	if(address)
	{
		LPBYTE lpAddress = reinterpret_cast<LPBYTE>(address);
		HookMemory mem;
		mem.data.type = HookJump;	//JMP
		UINT fundAddr = reinterpret_cast<UINT>(function);
		fundAddr -= (address + 5);
		mem.data.address = static_cast<UINT>(fundAddr);

		DWORD priv = 0;
		g_PerformingWriteOperation = TRUE;
		if(VirtualProtect(lpAddress, (5+nops), PAGE_EXECUTE_READWRITE, &priv))
		{
			for(SIZE_T n = 0; n < (5 + nops) ; n++)
			{
				if(n < 5)
				{
					lpAddress[n] = mem.bytes.byte[n];
				}else
				{
					lpAddress[n] = HookNOP;
				}
			}
			VirtualProtect(lpAddress, (5+nops), priv, &priv);
		}
		g_PerformingWriteOperation = FALSE;
	}
}

void Memory::WriteJump(UINT address, UINT jumpAddress, SIZE_T nops)
{
	if(address)
	{
		LPBYTE lpAddress = reinterpret_cast<LPBYTE>(address);
		HookMemory mem;
		mem.data.type = HookJump;	//JMP
		jumpAddress -= (address + 5);
		mem.data.address = static_cast<UINT>(jumpAddress);

		DWORD priv = 0;
		g_PerformingWriteOperation = TRUE;
		if(VirtualProtect(lpAddress, (5+nops), PAGE_EXECUTE_READWRITE, &priv))
		{
			for(SIZE_T n = 0; n < (5 + nops) ; n++)
			{
				if(n < 5)
				{
					lpAddress[n] = mem.bytes.byte[n];
				}else
				{
					lpAddress[n] = HookNOP;
				}
			}
			VirtualProtect(lpAddress, (5+nops), priv, &priv);
		}
		g_PerformingWriteOperation = FALSE;
	}
}

LPBYTE Memory::OpenSharedMemory(HANDLE& hMapFile, const WCHAR* wName, UINT size)
{
	if(hMapFile == 0)
	{
		hMapFile = OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, wName);
	}
	LPBYTE pBuf = 0;
	if(hMapFile == 0)
	{
		hMapFile = hMapFile = CreateFileMapping( INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, size, wName);
		pBuf = (LPBYTE) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
		if(pBuf)
		{
			memset(pBuf, 0, size);
		}
	}else
	{
		pBuf = (LPBYTE) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
	}
	return pBuf;
}

void Memory::CloseSharedMemory(HANDLE hMapFile, LPBYTE lpBuff)
{
   UnmapViewOfFile(lpBuff);
   CloseHandle(hMapFile);
}

PVOID Memory::HookProcInModule(PSTR sModule, PSTR sFunction, FARPROC pNewFunction, UINT size)
{
	VIRTUALIZER_START;
	HMODULE hModule = GetModuleHandleA(sModule);
	PVOID pNewProc = NULL;
	if(hModule)
	{
		LPBYTE pFunction = (LPBYTE)GetProcAddress(hModule, sFunction);
		if(pFunction)
		{			
			LPBYTE pMemory = (LPBYTE)VirtualAlloc(0, 32, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			if(pMemory)
			{
				for(UINT n=0;n<size;n++)
				{
					pMemory[n] = pFunction[n];
				}

				if(pFunction[0] == 0xE8 || pFunction[0] == 0xE9)
				{
					HookMemory hm;
					for(UINT n=0;n<size;n++)
					{
						pMemory[n] = pFunction[n];
						if(n<5)
						{
							hm.bytes.byte[n] = pFunction[n];
						}
					}
					//reallocate addr
					DWORD addr = hm.data.address;
					addr += ((DWORD)pFunction + 5);

					hm.data.address = addr - ((DWORD)pMemory + 5);

					for(int n=0;n<5;n++)
					{
						pMemory[n] = hm.bytes.byte[n];
					}
				}else if(pFunction[0] == 0x8D)
				{
					// mov bla , bla - usually 2-3 operations 5 bytes length 
					//do nothing just copy the bytes
				}else if(pFunction[0] == 0x6A)
				{
					//push XXh after this another push offset (so 2 + 5 bytes = 7 in total)
					if(size < 7)
					{
						size = 7;
						for(UINT n=0;n<size;n++)
						{
							pMemory[n] = pFunction[n];
						}
					}
				}else if(pFunction[0] == 0xFF && (pFunction[1] == 0x15 || pFunction[1] == 0x25))
				{
					//jump / call ds: 6 bytes length
					if(size < 6)
					{
						size = 6;
					}
					for(UINT n=0;n<size;n++)
					{
						pMemory[n] = pFunction[n];
					}
				}

				WriteJump((DWORD)&pMemory[size], (PVOID)((DWORD)pFunction + size), 0);
				WriteJump((DWORD)pFunction, (PVOID)(DWORD)pNewFunction, size - 5);
				pNewProc = pMemory;
			}
		}
	}
	VIRTUALIZER_END;
	return pNewProc;
}

DWORD ReadFileBinary(LPCWSTR lpFileName, LPBYTE& lpOut)
{
	UINT64 FileSize = 0;
	_WIN32_FIND_DATAW FileData;
	HANDLE hFile = FindFirstFile(lpFileName, &FileData);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		FileSize = MAXDWORD;
		FileSize += 1;
		FileSize *= FileData.nFileSizeHigh;
		FileSize += FileData.nFileSizeLow;
	}
	FindClose(hFile);
	if (FileSize > 0)
	{
		DWORD readed = 0;
		hFile = CreateFileW(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			lpOut = new BYTE[static_cast<UINT>(FileSize + 2)];
			memset(lpOut, 0, static_cast<UINT>(FileSize + 2));
			if (ReadFile(hFile, lpOut, static_cast<DWORD>(FileSize), &readed, NULL))
			{
				WaitForSingleObject(hFile, 5000);
			}
			CloseHandle(hFile);
		}
		return readed;
	}
	return 0;
}

BOOL WriteFileBinary(LPCWSTR lpFileName, LPBYTE lpBuffer, UINT length)
{
	BOOL ret = FALSE;
	HANDLE hFile = CreateFile(lpFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile)
	{

		DWORD written = 0;
		WriteFile(hFile, lpBuffer, length, &written, NULL);

		CloseHandle(hFile);
		ret = TRUE;
	}
	return ret;
}
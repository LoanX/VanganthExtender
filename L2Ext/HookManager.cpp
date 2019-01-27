#include "StdAfx.h"

CHookManager::CHookManager()
{
}

CHookManager::~CHookManager()
{
}

bool CHookManager::TryWrite(UINT64 Address, int Count)
{
	for(map<UINT64, bool>::iterator Iter = mData.begin();Iter!=mData.end();Iter++)
	{
		if(Iter->first >= Address && Iter->first < (Address+Count))
		{
			if(Iter->second)
			{
				ShowMessage("Hook Manager", "Memory at address[%p] has been already written (%p)!", Iter->first, Address);
				return false;
			}
		}
	}
	return true;
}

void CHookManager::Write(UINT64 Address, int Count)
{
	for(UINT64 n=Address; n<(Address+Count); n++)
	{
		mData[n] = true;
	}
}

bool CHookManager::ValidOrgBytes(UINT64 Address, LPBYTE OrgBytes, int Count)
{
	if(Address && OrgBytes && Count)
	{
		if(!memcmp((LPVOID)Address, OrgBytes, Count))
		{
				return true;
		}else
		{	
			ShowMessage("Hook Manager", "Invalid original memory at address [%p]", Address);
			return false;
		}
	}
	return true;
}

void CHookManager::WriteMemory(UINT64 Address, LPCVOID pData, SIZE_T Size, LPBYTE OrgBytes)
{
	if(Address && pData && Size)
	{
		Lock();
		if(ValidOrgBytes(Address, OrgBytes, Size))
		{
			if(TryWrite(Address, Size))
			{
				DWORD dwOldProtect = NULL;
				SIZE_T BytesWritten = NULL;
				HANDLE 	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
				if(hProcess)
				{
					if(!VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
					{ 
						ShowMessage("Hook Manager", "Failed to set new protection with VirtualProtect at address [%p]!", Address);
					}
					if (WriteProcessMemory(hProcess, (LPVOID)Address, pData, Size, &BytesWritten))
					{
						Write(Address, Size);
					}else
					{
						ShowMessage("Hook Manager", "Failed to write process memory at address[%p]!", Address);
						Unlock();
						ExitProcess(0);	
						return;
					}
					if(!VirtualProtect((LPVOID)Address, Size, dwOldProtect, &dwOldProtect))
					{ 
						ShowMessage("Hook Manager", "Failed to set old protection with VirtualProtect at address [%p]!", Address);
					}
					CloseHandle(hProcess);
				}else
				{
					ShowMessage("Hook Manager", "Failed to get process handle!");
					Unlock();
					ExitProcess(0);
					return;
				}

			}
		}
		Unlock();
	}
}

void CHookManager::WriteCall(UINT64 Address, PVOID Function, SIZE_T Nops, LPBYTE OrgBytes)
{
	if(Address && Function)
	{
		Lock();
		SIZE_T Size = 5 + Nops;
		if(ValidOrgBytes(Address, OrgBytes, Size))
		{
			if(TryWrite(Address, Size))
			{
				DWORD dwOldProtect = NULL;
				SIZE_T BytesWritten = NULL;

				PUCHAR Buffer = new unsigned char[Size];
				if(Buffer)
				{
					memset(Buffer, 0, Size);
					Buffer[0]=CALL;
					UINT functionAddr = (UINT)Function;
					functionAddr-=Address+5;
					memcpy(&Buffer[1], &functionAddr, 4);
				
					if(Nops)
					{		
						for(int n=5; n<Size; n++)
						{
							Buffer[n]=0x90;
						}
					}

					HANDLE 	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
					if(hProcess)
					{
						if(!VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
						{ 
							ShowMessage("Hook Manager", "Failed to set new protection with VirtualProtect at address [%p]!", Address);
						}
						if (WriteProcessMemory(hProcess, (LPVOID)Address, Buffer, Size, &BytesWritten))
						{
							Write(Address, Size);
						}else
						{
							ShowMessage("Hook Manager", "Failed to write process memory at address[%p]!", Address);
							Unlock();
							ExitProcess(0);	
							return;
						}
						if(!VirtualProtect((LPVOID)Address, Size, dwOldProtect, &dwOldProtect))
						{ 
							ShowMessage("Hook Manager", "Failed to set old protection with VirtualProtect at address [%p]!", Address);
						}
						CloseHandle(hProcess);
					}else
					{
						ShowMessage("Hook Manager", "Failed to get process handle!");
						Unlock();
						ExitProcess(0);
						return;
					}
					delete [] Buffer;
				}else
				{
					ShowMessage("Hook Manager", "Cannot allocate memory for buffer!");
					Unlock();
					ExitProcess(0);
					return;
				}

			}
		}
		Unlock();
	}
}

void CHookManager::WriteJump(UINT64 Address, PVOID Function, SIZE_T Nops, LPBYTE OrgBytes)
{
	if(Address && Function)
	{
		Lock();
		SIZE_T Size = 5 + Nops;
		if(ValidOrgBytes(Address, OrgBytes, Size))
		{
			if(TryWrite(Address, Size))
			{
				DWORD dwOldProtect = NULL;
				SIZE_T BytesWritten = NULL;

				PUCHAR Buffer = new unsigned char[Size];
				if(Buffer)
				{
					memset(Buffer, 0, Size);
					Buffer[0]=JMP;
					UINT functionAddr = (UINT)Function;
					functionAddr-=Address+5;
					memcpy(&Buffer[1], &functionAddr, 4);
				
					if(Nops)
					{		
						for(int n=5; n<Size; n++)
						{
							Buffer[n]=0x90;
						}
					}

					HANDLE 	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
					if(hProcess)
					{
						if(!VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
						{ 
							ShowMessage("Hook Manager", "Failed to set new protection with VirtualProtect at address [%p]!", Address);
						}
						if (WriteProcessMemory(hProcess, (LPVOID)Address, Buffer, Size, &BytesWritten))
						{
							Write(Address, Size);
						}else
						{
							ShowMessage("Hook Manager", "Failed to write process memory at address[%p]!", Address);
							Unlock();
							ExitProcess(0);	
							return;
						}
						if(!VirtualProtect((LPVOID)Address, Size, dwOldProtect, &dwOldProtect))
						{ 
							ShowMessage("Hook Manager", "Failed to set old protection with VirtualProtect at address [%p]!", Address);
						}
						CloseHandle(hProcess);
					}else
					{
						ShowMessage("Hook Manager", "Failed to get process handle!");
						Unlock();
						ExitProcess(0);
						return;
					}
					delete [] Buffer;
				}else
				{
					ShowMessage("Hook Manager", "Cannot allocate memory for buffer!");
					Unlock();
					ExitProcess(0);
					return;
				}

			}
		}
		Unlock();
	}
}

void CHookManager::WriteJump(UINT64 Address, UINT64 Dest, SIZE_T Nops, LPBYTE OrgBytes)
{
	if(Address && Dest)
	{
		Lock();
		SIZE_T Size = 5 + Nops;
		if(ValidOrgBytes(Address, OrgBytes, Size))
		{
			if(TryWrite(Address, Size))
			{
				DWORD dwOldProtect = NULL;
				SIZE_T BytesWritten = NULL;

				PUCHAR Buffer = new unsigned char[Size];
				if(Buffer)
				{
					memset(Buffer, 0, Size);
					Buffer[0]=JMP;
					UINT functionAddr = (UINT)Dest;
					functionAddr-=Address+5;
					memcpy(&Buffer[1], &functionAddr, 4);
				
					if(Nops)
					{		
						for(int n=5; n<Size; n++)
						{
							Buffer[n]=0x90;
						}
					}

					HANDLE 	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
					if(hProcess)
					{
						if(!VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
						{ 
							ShowMessage("Hook Manager", "Failed to set new protection with VirtualProtect at address [%p]!", Address);
						}
						if (WriteProcessMemory(hProcess, (LPVOID)Address, Buffer, Size, &BytesWritten))
						{
							Write(Address, Size);
						}else
						{
							ShowMessage("Hook Manager", "Failed to write process memory at address[%p]!", Address);
							Unlock();
							ExitProcess(0);	
							return;
						}
						if(!VirtualProtect((LPVOID)Address, Size, dwOldProtect, &dwOldProtect))
						{ 
							ShowMessage("Hook Manager", "Failed to set old protection with VirtualProtect at address [%p]!", Address);
						}
						CloseHandle(hProcess);
					}else
					{
						ShowMessage("Hook Manager", "Failed to get process handle!");
						Unlock();
						ExitProcess(0);
						return;
					}
					delete [] Buffer;
				}else
				{
					ShowMessage("Hook Manager", "Cannot allocate memory for buffer!");
					Unlock();
					ExitProcess(0);
					return;
				}

			}
		}
		Unlock();
	}
}

void CHookManager::NopMemory(UINT64 Address, SIZE_T Size, LPBYTE OrgBytes)
{
	if(Address && Size)
	{
		Lock();
		if(ValidOrgBytes(Address, OrgBytes, Size))
		{
			if(TryWrite(Address, Size))
			{				
				PUCHAR Buffer = new unsigned char[Size];
				if(Buffer)
				{
					memset(Buffer, 0x90, Size);
					DWORD dwOldProtect = NULL;
					SIZE_T BytesWritten = NULL;
					HANDLE 	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
					if(hProcess)
					{
						if(!VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
						{ 
							ShowMessage("Hook Manager", "Failed to set new protection with VirtualProtect at address [%p]!", Address);
						}
						if (WriteProcessMemory(hProcess, (LPVOID)Address, Buffer, Size, &BytesWritten))
						{
							Write(Address, Size);
						}else
						{
							ShowMessage("Hook Manager", "Failed to write process memory at address[%p]!", Address);
							Unlock();
							ExitProcess(0);	
							return;
						}
						if(!VirtualProtect((LPVOID)Address, Size, dwOldProtect, &dwOldProtect))
						{ 
							ShowMessage("Hook Manager", "Failed to set old protection with VirtualProtect at address [%p]!", Address);
						}
						CloseHandle(hProcess);
					}else
					{
						ShowMessage("Hook Manager", "Failed to get process handle!");
						Unlock();
						ExitProcess(0);
						return;
					}
					delete [] Buffer;
				}else
				{
					ShowMessage("Hook Manager", "Cannot allocate memory for nop buffer!");
					Unlock();
					ExitProcess(0);
					return;
				}
			}
		}
		Unlock();
	}
}

void CHookManager::WriteMemoryDWORD(UINT64 Address, DWORD Value, LPBYTE OrgBytes)
{
	if(Address)
	{
		SIZE_T Size = sizeof(DWORD);
		Lock();
		if(ValidOrgBytes(Address, OrgBytes, Size))
		{
			if(TryWrite(Address, Size))
			{
				DWORD dwOldProtect = NULL;
				SIZE_T BytesWritten = NULL;
				HANDLE 	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
				if(hProcess)
				{
					if(!VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
					{ 
						ShowMessage("Hook Manager", "Failed to set new protection with VirtualProtect at address [%p]!", Address);
					}
					if (WriteProcessMemory(hProcess, (LPVOID)Address, &Value, Size, &BytesWritten))
					{
						Write(Address, Size);
					}else
					{
						ShowMessage("Hook Manager", "Failed to write process memory at address[%p]!", Address);
						Unlock();
						ExitProcess(0);	
						return;
					}
					if(!VirtualProtect((LPVOID)Address, Size, dwOldProtect, &dwOldProtect))
					{ 
						ShowMessage("Hook Manager", "Failed to set old protection with VirtualProtect at address [%p]!", Address);
					}
					CloseHandle(hProcess);
				}else
				{
					ShowMessage("Hook Manager", "Failed to get process handle!");
					Unlock();
					ExitProcess(0);
					return;
				}

			}
		}
		Unlock();
	}
}

void CHookManager::WriteMemoryBYTE(UINT64 Address, BYTE Value, LPBYTE OrgBytes)
{
	if(Address)
	{
		SIZE_T Size = sizeof(BYTE);
		Lock();
		if(ValidOrgBytes(Address, OrgBytes, Size))
		{
			if(TryWrite(Address, Size))
			{
				DWORD dwOldProtect = NULL;
				SIZE_T BytesWritten = NULL;
				HANDLE 	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
				if(hProcess)
				{
					if(!VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
					{ 
						ShowMessage("Hook Manager", "Failed to set new protection with VirtualProtect at address [%p]!", Address);
					}
					if (WriteProcessMemory(hProcess, (LPVOID)Address, &Value, Size, &BytesWritten))
					{
						Write(Address, Size);
					}else
					{
						ShowMessage("Hook Manager", "Failed to write process memory at address[%p]!", Address);
						Unlock();
						ExitProcess(0);	
						return;
					}
					if(!VirtualProtect((LPVOID)Address, Size, dwOldProtect, &dwOldProtect))
					{ 
						ShowMessage("Hook Manager", "Failed to set old protection with VirtualProtect at address [%p]!", Address);
					}
					CloseHandle(hProcess);
				}else
				{
					ShowMessage("Hook Manager", "Failed to get process handle!");
					Unlock();
					ExitProcess(0);
					return;
				}

			}
		}
		Unlock();
	}
}

UINT64 CHookManager::WriteRelativeAddress(UINT64 nOrgAddress, UINT64 nWriteFrom, VOID *pArray, UINT64 nOffsetIndex) 
{ 
	UINT64 nAddressToPatch  = nOrgAddress+nWriteFrom;
	UINT64 nRIPAddress      = nOrgAddress+nWriteFrom+4;
	UINT64 nPointerAddress  = (UINT64)pArray;
	UINT64 nRelativeAddress = ((nPointerAddress + nOffsetIndex) - nRIPAddress);

	WriteMemoryDWORD((UINT32)nAddressToPatch, (UINT32)nRelativeAddress);

	return nRelativeAddress;
}

inline void WriteRelative(UINT64 baseAddr, LPVOID lpObject)
{
	DWORD relative = (DWORD)((UINT64)lpObject - (baseAddr + 4));
	DWORD priv = 0;
	if(VirtualProtect((LPVOID)baseAddr, 4, PAGE_EXECUTE_READWRITE, &priv))
	{
		(*(LPDWORD)baseAddr) = relative;
		DWORD old = 0;
		VirtualProtect((LPVOID)baseAddr, 4, priv, &old);
	}
}
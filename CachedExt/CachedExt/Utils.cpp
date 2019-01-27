#include "Utils.h"

UINT GetThreadIndex()
{
	typedef UINT (*f)();
	return f(0x49AE70L)();
}

const unsigned char* Disassemble(const unsigned char* packet, const char *format, ...)
{
	va_list va;
	va_start(va, format);
	typedef const unsigned char* (*f)(const unsigned char*, const char*, va_list);
	const unsigned char* ret = f(0x43F1F0L)(packet, format, va);
	va_end(va);
	return ret;
}

int Assemble(char *buff, int nBuffLen, const char *format, ...)
{
	va_list va;
	va_start(va, format);
	typedef int(*f)(char*, int, const char*, va_list);
	int nRet = f(0x43E150L)(buff, nBuffLen, format, va);
	va_end(va);
	return nRet;
}

void MovLogFiles(const TCHAR* logDir)
{
	vector<wstring> logFiles;
	CreateDirectory(logDir, 0);
	WIN32_FIND_DATAW info;
	HANDLE hFile = 0;
	TCHAR dirPath[265];
	GetCurrentDirectory(260, dirPath);
	_tcscat_s(dirPath, 265, _T("\\*"));
	hFile = FindFirstFile(dirPath, &info);
	if(hFile)
	{
		do
		{
			if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			 
			}
			else
			{
				if(_tcsstr(info.cFileName, _T("LinError.txt")))
				{
					logFiles.push_back(info.cFileName);
				}else if(_tcsstr(info.cFileName, _T("ExtError.txt")))
				{
					logFiles.push_back(info.cFileName);
				}
			}
		}
		while (FindNextFile(hFile, &info) != 0);
		FindClose(hFile);
	}
	if(logFiles.size() > 0)
	{
		for(UINT n=0;n<logFiles.size();n++)
		{
			TCHAR newFile[260];
			_tcscpy_s(newFile, 512, logDir);
			_tcscpy_s(newFile, 512, _T("\\"));
			_tcscpy_s(newFile, 512, logFiles[n].c_str());
			MoveFile(logFiles[n].c_str(), newFile);
		}
	}
};

bool WriteMemory(DWORD dwDestAddress, LPVOID pData, DWORD dwSize)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	if(hProcess)
	{
		DWORD dwOldProtect = 0;
		if(VirtualProtect((LPVOID)dwDestAddress, dwSize, PAGE_WRITECOPY, &dwOldProtect))
		{
			DWORD dwWritten = 0;
			WriteProcessMemory(hProcess, (LPVOID)dwDestAddress, pData, dwSize, &dwWritten);
			VirtualProtect((LPVOID)dwDestAddress, dwSize, dwOldProtect, &dwOldProtect);
		}
		CloseHandle(hProcess);
		return true;
	}else
	{
		MessageBox(NULL, _T("Cannot get process handle!"), _T("CacheD Extension"), MB_OK);
	}
	return false;
}

bool WriteMemoryDWORD(DWORD dwDestAddress, DWORD dwValue)
{
	return WriteMemory(dwDestAddress, &dwValue, sizeof(DWORD));
}
#include "stdafx.h"
#include "CryptFile.h"

wstring CCryptFile::GenerateRandomName()
{
	wstringstream nameStream;
	for(UINT n=0;n<5;n++)
	{
		nameStream << (UINT)(rand() % 10);
	}
	nameStream << L".tmp";
	return nameStream.str();
}

void CCryptFile::Release()
{
	if(m_Name.size() > 0)
	{
		DeleteFile(m_Name.c_str());
	}
}

wstring CCryptFile::Handle(LPCWSTR lpFileName)
{
	LPBYTE lpData = 0;
	DWORD size = ReadFileBinary(lpFileName, lpData);
	if(size > 0)
	{
		XorCrypt(lpData, size);
	}
	m_Name = GenerateRandomName();
	WriteFileBinary(m_Name.c_str(), lpData, size);
	return m_Name;
}

void CCryptFile::XorCrypt(LPBYTE lpBuff, UINT len)
{
	BYTE key = 0x81;
	for(UINT n=0;n<len;n++)
	{
		if((n%1024) == 0)
		{
			key = 0x81;
		}
		lpBuff[n] ^= key;
		key*=n;
		key+=n;
	}
}
DWORD CCryptFile::ReadFileBinary(LPCWSTR lpFileName, LPBYTE& lpOut)
{
	UINT64 FileSize = 0;
	_WIN32_FIND_DATAW FileData;
	HANDLE hFile = FindFirstFileW(lpFileName, &FileData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		FileSize = MAXDWORD;
		FileSize += 1;
		FileSize *= FileData.nFileSizeHigh;
		FileSize += FileData.nFileSizeLow;
	}
	FindClose(hFile);
	DWORD readed = 0;
	hFile = CreateFileW(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		lpOut = new BYTE[FileSize];
		memset(lpOut, 0, FileSize);
		if(ReadFile(hFile, lpOut, static_cast<DWORD>(FileSize), &readed, NULL))
		{
			WaitForSingleObject(hFile, 5000);
		}
	}
	CloseHandle(hFile);
	return readed;
}

BOOL CCryptFile::WriteFileBinary(LPCWSTR lpFileName, LPBYTE lpBuffer, UINT length)
{
	BOOL ret = FALSE;
	HANDLE hFile = CreateFileW(lpFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile)
	{
		
		DWORD written = 0;
		WriteFile(hFile, lpBuffer, length, &written, NULL);

		CloseHandle(hFile);
		ret = TRUE;
	}
	return ret;
}
#pragma once

#include "class.h"

class CFileMapping
{
	int BuffSize;
	std::wstring sName;
	HANDLE hMapFile;
	LPVOID pBuff;
	bool bReady;
	
	bool Map()
	{
		pBuff = MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,                   
                        0,                   
                        BuffSize);
		if(pBuff)
		{
			return true;
		}
		return false;
	}
public:
	CFileMapping() : hMapFile(NULL), pBuff(NULL), bReady(false) {}
	CFileMapping(int Size, std::wstring Name) : BuffSize(Size), sName(Name) {};
	~CFileMapping()
	{
		if(pBuff)
			UnmapViewOfFile(pBuff);
		if(hMapFile)
			CloseHandle(hMapFile);
	}
	bool IsReady()
	{
		return bReady;
	}
	void SetSize(int Size)
	{
		BuffSize = Size;
	}
	void SetName(std::wstring Name)
	{
		sName = Name;
	}
	LPVOID GetBuff()
	{
		return pBuff;
	}
	bool Open()
	{
		hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   sName.c_str());        // name of mapping object 

		if( hMapFile )
		{
			if(Map())
			{
				bReady = true;
				return true;
			}
		}
		return false;
	}
	bool Create()
	{
		hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security 
                 PAGE_READWRITE,          // read/write access
                 0,                       // maximum object size (high-order DWORD) 
                 BuffSize,                // maximum object size (low-order DWORD)  
				 sName.c_str());          // name of mapping object
		if( hMapFile )
		{
			if(Map())
			{
				bReady = true;
				return true;
			}
		}
		return false;
	}
};
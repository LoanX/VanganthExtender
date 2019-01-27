#pragma once
#pragma warning(disable:4800)
#include <windows.h>
#include "TCHAR.h"

class CConfig
{
private:
	CConfig(){ReadData();}
	CConfig(const CConfig &);
	CConfig& operator=(const CConfig&);
public:
	enum SYSTEM
	{
		PRIVATE_STORE_IN_DB
	};
	static CConfig& GetInstance()
	{
		static CConfig Config;
		return Config;
	}
	bool IsSet(SYSTEM System)
	{
		switch(System)
		{
		case PRIVATE_STORE_IN_DB:
			return PrivateStoreInDB;
			break;
		}
		return false;
	};
private:
	int PrivateStoreInDB;
	void ReadData()
	{
		TCHAR File[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,File);
		lstrcat(File, L"\\l2cache.ini");
		PrivateStoreInDB = GetPrivateProfileInt(_T("Setting"), _T("PrivateStoreInDB"), 0, File);
	}
};
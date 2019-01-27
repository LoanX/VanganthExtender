#pragma once
#include "Utils.h"


class CLogger
{
	CSLock m_Lock;
	HANDLE m_File;
public:
	CLogger();
	~CLogger();
	void Initialize();
	void Create(PWCHAR sFile, PWCHAR sDirectory);
	void GenerateFileName(PWCHAR sFile, PWCHAR sDirectory);
	void Add(const WCHAR* format, ...);
};
extern CLogger g_Logger;
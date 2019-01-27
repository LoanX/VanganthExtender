#include "StdAfx.h"

CLogger g_Logger;

CLogger::CLogger(const WCHAR* wFileBase)
{ 
	WCHAR wFile[260];
	memset(wFile, 0, sizeof(wFile));
	GenerateFileName(wFile, L"ExtLog", wFileBase);
	Create(wFile, L"ExtLog");
}

CLogger::~CLogger() 
{ 
	if(m_File != INVALID_HANDLE_VALUE)
		CloseHandle(m_File);
}

void CLogger::Initialize()
{
	Add(L"Initializing Logger ...");
}

void CLogger::Create(PWCHAR sFile, PWCHAR sDirectory)
{
	if(FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(sDirectory))
	{
		//directory exists
	}else
	{
		CreateDirectory(sDirectory, NULL);
	}
	m_Lock.Enter();
	m_File = CreateFile(sFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_File != INVALID_HANDLE_VALUE)
	{
		wstring encoding;
		encoding.insert(0, 1, 0xFEFF);
		DWORD written = 0;
		WriteFile(m_File, encoding.c_str(), (static_cast<DWORD>(encoding.size()) * 2), &written, 0);
	}
	m_Lock.Leave();
}

void CLogger::GenerateFileName(PWCHAR sFile, PWCHAR sDirectory, const WCHAR* wFileBase)
{
	tm timeinfo;
	GetTimeInfo(timeinfo);
	if(wFileBase)
	{
		swprintf_s(sFile, 255, L"%s/%0.4d-%0.2d-%0.2d-%0.2d-%0.2d-%s.log", sDirectory, (timeinfo.tm_year+1900), (timeinfo.tm_mon+1), timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, wFileBase);
	}else
	{
		swprintf_s(sFile, 255, L"%s/%0.4d-%0.2d-%0.2d-%0.2d-%0.2d.log", sDirectory, (timeinfo.tm_year+1900), (timeinfo.tm_mon+1), timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min);
	}
}

void CLogger::Add(const WCHAR* format, ...)
{
//	guard;
	if(m_File != INVALID_HANDLE_VALUE)
	{
		va_list va;
		va_start(va, format);
		WCHAR Log[4096];
		WCHAR FinalLog[4096];
		memset(FinalLog, 0, sizeof(FinalLog));
		tm timeinfo;
		GetTimeInfo(timeinfo);
		swprintf_s(Log, 4096, L"[%0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d] %s\r\n", (timeinfo.tm_year+1900), (timeinfo.tm_mon+1), timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, format);
		int len = wvsprintf(FinalLog, Log, va);
		DWORD written = 0;
		m_Lock.Enter();
		WriteFile(m_File, FinalLog, (len*2), &written, 0);
		m_Lock.Leave();
		va_end(va);
	}

//	unguard;
}
#include "stdafx.h"
#include "Log.h"

CLog g_Log;

CLog::CLog()
{
	m_lpOrgInstance = reinterpret_cast<LPVOID>(0x638C7D0L);
}

CLog::~CLog()
{
}

void CLog::Add(LogType type, const CHAR *format, ...)
{
	va_list va;
	va_start(va, format);
	typedef void(*f)(LPVOID, LogType, const CHAR*, va_list);
	f(0x4AAE80L)(m_lpOrgInstance, type, format, va);
	va_end(va);
}

void CLog::Add(LogType type, const WCHAR *format, ...)
{
	va_list va;
	va_start(va, format);
	typedef void(*f)(LPVOID, LogType, const WCHAR*, va_list);
	f(0x4AB6B0L)(m_lpOrgInstance, type, format, va);
	va_end(va);
}
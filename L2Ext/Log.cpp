#include "stdafx.h"
#include "Log.h"

CLog g_Log;

CLog::CLog()
{
	pInstance = (CLog*)0x06F90A90;
}

void CLog::Add(LogType Type, const char *format, ...)
{
	typedef void (*f)(CLog*, LogType, const char*, va_list);
	va_list va;
	va_start(va, format);
	f(0x0064CBB0L)(pInstance, Type, format, va);
	va_end(va);
}

void CLog::Add(LogType Type, const wchar_t *format, ...)
{
	typedef void (*f)(CLog*, LogType, const wchar_t*, va_list);
	va_list va;
	va_start(va, format);
	f(0x64D340L)(pInstance, Type, format, va);
	va_end(va);
}

void CLog::StackTrace(LogType type)
{
	typedef void (*f)(int);
	f(0x5B8E30L)(type);
}
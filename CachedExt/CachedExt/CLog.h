#pragma once

class CLog
{
	CLog *pInstance;
public:
	enum LogType
	{
		Black = 0,
		Blue = 1,
		Error = 2,
		In = 6,
		Db = 7,
		Audit = 8,
		Chat = 9
	};
	CLog() { pInstance = (CLog*) 0x8FF138; }
	void Add(LogType Type, const char* format, ...)
	{
		va_list va;
		va_start(va, format);
		typedef void (__thiscall *f)(CLog*, LogType, const char*, va_list);
		f(0x4464B0L)(pInstance, Type, format, va);
		va_end(va);
	}

	void AddU(LogType Type, const wchar_t* format, ...)
	{
		typedef void (__thiscall *_f) (CLog*, LogType, const wchar_t*, va_list);
		_f f = (_f)0x00446AC0;

		va_list va;
		va_start(va, format);
		f(pInstance,Type,format,va);
		va_end(va);
	};
	void CallStack()
	{
		typedef void(__cdecl *f)(UINT);
		f(0x430F10L)(CLog::Error);
	}
};

extern CLog g_Log;

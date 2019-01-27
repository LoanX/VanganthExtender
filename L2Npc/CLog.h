#pragma once

class CLog
{
	CLog *pLog;
	public:
		enum LogType{
			Black = 0,
			Blue = 1,
			Error = 2
		};
		CLog() { pLog = (CLog *)0x0638C7D0; };
		void Add(LogType lgType, char * szMessage, ...)
		{
			typedef void (*_f)(CLog *, LogType, char *, ...);
			_f f = (_f) 0x004AA950;

			f(this->pLog, lgType, szMessage);
		};
		void AddU(LogType lgType, wchar_t * wszMessage, ...)
		{
			typedef void (*_f)(CLog *, LogType, wchar_t *, ...);
			_f f = (_f) 0x004AAAE0;

			f(this->pLog, lgType, wszMessage);
		};
};

extern CLog g_Log;
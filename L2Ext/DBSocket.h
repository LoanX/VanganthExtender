#pragma once


class CDBSocket
{
public:
	void Send(const char* format, ...)
	{
		va_list va;
		va_start(va, format);
		SendV(format, va);
		va_end(va);
	}
	void SendV(const char *format, va_list va)
	{
		typedef void (*f)(CDBSocket*, const char*, va_list);
		f(0x54A810)(this, format, va);
	};
};
#pragma once


class COlympiadObserverEntry
{
public:
	void Broadcast(const char* format, va_list args)
	{
		typedef void(*f)(COlympiadObserverEntry*, const char*, va_list);
		f(0x6FFC30)(this, format, args);
	}
};
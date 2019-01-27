#pragma once
#include "class.h"
#include "..//L2Ext//NpcOpCode.h"

class CNpcServer
{
	CNpcServer* Instance;
public:
	CNpcServer() { Instance = (CNpcServer*) 0x328BBB0; }
	void Send(const char *format, ...)
	{
		typedef void(*f)(CNpcServer*, const char*, ...);
		f(0x581F50)(Instance, format);
	}
};

extern CNpcServer g_NpcServer;
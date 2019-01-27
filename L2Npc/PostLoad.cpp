#include "L2NPC.h"

void CPostLoad::Initialize()
{
	WriteHook(CALL, 0x5CA90B, (DWORD)LoadAfterAi, 0);
}

void CPostLoad::LoadAfterAi(const char *format)
{
	typedef void (*_f)(const char*);
	_f(0x582E30)(format);
	
	g_EraSystem.Initialize();
	//g_NpcServer.Send("cdd", 0x38, TEST_NPC_OPCODE, 456);
}
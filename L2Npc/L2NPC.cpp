#include "L2NPC.h"

CLog g_Log;
CHookGuard g_HookGuard;
Config* Config::pConfig;
Config gConfig;
GlobalIntVariable g_Era;
CEraSystem g_EraSystem;
CNpcServer g_NpcServer;

//olympiad points bug fix

LPVOID CNPCEventFactory_CreateDeletePreviousOlympiadPointReturned(LPVOID *pNPC, LPVOID *pTalker, int nPoint, int nReturned)
{
	if(nPoint > 0)
	{
		typedef LPVOID (*f)(LPVOID, LPVOID, int, int);
		return f(0x5428E0L)(pNPC, pTalker, nPoint, nReturned);
	}
	return 0;
}


//CNPC::GetObject(int sid) crash fix

bool CNPC_IsNpc(CNPC *pNpc)
{
	try
	{
		UINT64 addr = *(PUINT64)pNpc;	//vt
		addr += 0xE0;
		addr = *(PUINT64)addr;	//IsNpc
		typedef bool (*f)(CNPC*);
		return f(addr)(pNpc);
	}catch(...)
	{
	}
	return false;
}

void L2NPC::Initialize()
{
	MovLogFiles("LinErrBak");
	CNPC::OverrideFunctions();
	CPostLoad::Initialize();
	CSharedFactory::Initialize();
	CGlobalObject::Initialize();
	CTimer::Initialize();
	CL2ServerPacketHandler::Init();
	WriteHook(0xE8, 0x5DD420, (DWORD)CNPCEventFactory_CreateDeletePreviousOlympiadPointReturned, 0);
	WriteHook(0xE9, 0x5DD464, 0x5DD48C, 3);
	WriteHook(0xE8, 0x4CC863L, (DWORD)CNPC_IsNpc, 4);


}
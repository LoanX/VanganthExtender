// NpcExt.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "NPCEventFactory.h"
#include "PostLoad.h"
#include "BaseAction.h"
#include "Timer.h"
#include "ScriptAccessible.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL g_Initialized = FALSE;

void InitializeExtender()
{
	if(!g_Initialized)
	{
		//Fix the func at 0x4328A0
		WriteBYTE(0x4328A0, 0xC3);	//ret
		WriteNOP(0x4328A1, 15);

		WriteNOP(0x5D57CD, 5);	//UserObjectEvent_LogInvalidDelete
		WriteNOP(0x6012A7, 5);	//User_AddReverseHate_NoActiveUser
		
		MovLogFiles(L"ErrBak");
		
		CSharedFactory::Init();
		CNPC::ExtInit();
		CNPCEventFactory::Init();
		CPostLoad::Init();
		CBaseAction::Init();
		CTimer::Init();
		CScriptAccessible::Init();

		g_Initialized = TRUE;
	}
}

DLLEXPORT BOOL APIENTRY DllMain( HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			InitializeExtender();
			break;
		}

		case DLL_PROCESS_DETACH:
		{

			break;
		}
	}

	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


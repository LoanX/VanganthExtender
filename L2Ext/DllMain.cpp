#include "StdAfx.h"
#include "Packets.h"
#include "LogControl.h"
#include "MenuExtension.h"
#include "PostLoad.h"
#include "BuildCmdHandler.h"
#include "Augmentation.h"
#include "CreatureAction.h"
#include "PlayerAction.h"
#include "NpcAction.h"
#include "PetAction.h"
#include "OtherAction.h"
#include "MPCC.h"
#include "IOBufferExt.h"


#pragma optimize("", off)

bool g_Initialized = false;

DllExport BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
{
	VIRTUALIZER_TIGER_WHITE_START;
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			if(!g_Initialized)
			{
				g_Initialized = true;

				LPCWSTR lpCommandLine =  GetCommandLineW();
				BOOL resetKey = 0;
				if(lpCommandLine)
				{
					INT32 argCount = 0;
					LPWSTR* lpArgs = CommandLineToArgvW(lpCommandLine, &argCount);
					for(INT32 n=0;n<argCount;n++)
					{
						LPWSTR lpArg = lpArgs[n];
						try
						{
							wstring arg(lpArg);
							size_t pos = arg.find(L"-");
							while(pos != wstring::npos)
							{
								arg.erase(pos, 1);
								pos = arg.find(L"-");
							}
							pos = arg.find(L" ");
							while(pos != wstring::npos)
							{
								arg.erase(pos, 1);
								pos = arg.find(L" ");
							}
							if(arg == L"resetkey")
							{
								resetKey = TRUE;
							}
						}catch(...)
						{

						}
					}
				}

				if(resetKey)
				{
					typedef bool (__stdcall *UninstallKeyFn)(void);
					HINSTANCE libInst=LoadLibrary(L"ArmAccess.DLL");

					if (!libInst) return 0; 

					UninstallKeyFn unKey=(UninstallKeyFn)GetProcAddress(libInst, "UninstallKey");

					if (unKey!=0)
					{
					   unKey();
					   MessageBox(0, L"Your key has been removed!", L"INFO", MB_OK);
					   ExitProcess(0);
					}
					else
					   MessageBox(0, L"Cannot find ArmAccess!", L"INFO", MB_OK);

					FreeLibrary(libInst);
				}

				g_SplashScreen.Init(NULL, hInstance, IDB_BITMAP2);
				g_SplashScreen.Show();

				L2Ext::BaseData();
				L2Ext::FixNPCnItemID();
				L2Ext::EtcHook();
				g_CallStack.Init();
				User::ExtInit();
				CUserSocket::ExtInit();
				CPostLoad::Initialize();
				CSharedFactory::Initialize();
				ClassExtender::Initialize();
				PlayerAction::Initialize();
				NpcAction::Initialize();
				PetAction::Initialize();
				CreatureAction::Initialize();
				COtherAction::Initialize();
				CL2Time::Initialize();
				BuffSlots::Initialize();
				g_Augmentation.Initialize();
				Quests::Initialize();
				CLogControl::Initialize();
				CPacketFix::Initialize();
				CMPCC::Initialize();
				CFreightSystem::Initialize();
				CSkillFx::Initialize();
				CBuilderCommand::Initialize();
				CMenuExtension::Install();
				InitializeSkillEnchanting();

	#ifdef EXTEND_IOBUFFER
				CIOBufferExt::Init();
	#endif
	//			ShowMessage("CreatureSharedData", "Size %p, lastVar %p", sizeof(CreatureSharedData), offsetof(CreatureSharedData, CreatureSharedData::_nextFreeVariable));
	//			ShowMessage("CCreature", "Size %p, lastVar %p", sizeof(CCreature), offsetof(CCreature, CCreature::_unkn14F8));
	//			ShowMessage("User", "Size %p, lastVar %p", sizeof(User), offsetof(User, User::align8));
				DisableThreadLibraryCalls(hInstance);
			}
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
	}

	VIRTUALIZER_TIGER_WHITE_END;
	return true;
}


#pragma optimize("", on)
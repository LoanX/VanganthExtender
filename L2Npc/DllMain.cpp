#include "DllMain.h"

DllExport BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			L2NPC::Initialize();
			break;
		}

		case DLL_PROCESS_DETACH:
		{

			break;
		}
	}

	return true;
}
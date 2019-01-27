#include "DllMain.h"
#include "BuffSlot.h"
#include "Augmentation.h"

DllExport BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			CachedExt::Initialize();
			CBuffSlot::Init();
			CAugmentation::Init();
			const char* cachedVersion = "$Revision: 453 $";
			CachedExt::WriteMemoryBYTES(0x528E48, (LPVOID)cachedVersion, 16);
			break;
		}

		case DLL_PROCESS_DETACH:
		{

			break;
		}
	}

	return true;
}
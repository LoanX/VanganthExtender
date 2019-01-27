#ifndef _DLLMAIN_
	#define _DLLMAIN_

	#ifndef _CACHEDEXT_
		#include "CachedExt.h"
	#endif

	#define DllExport __declspec(dllexport)

DllExport BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved);

#endif
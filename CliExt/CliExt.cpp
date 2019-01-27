// CliExt.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "CliExt.h"
#include "ClientSocket.h"
#include "NetworkHandler.h"
#include "MemoryProtector.h"
#include "Crypt.h"
#include "MD5.h"

BOOL g_Initialized = FALSE;
HMODULE g_Engine = 0;
HMODULE g_CliExt = 0;

BYTE g_MD5Checksum[32] = { 0 };

HANDLE g_HardwareIdSM = 0;
LPBYTE g_lpHardwareIdSM = 0;
UINT g_AuthPort = 2106;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

CLIEXT_API BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitializeExtender(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#pragma optimize("", off)
void InitializeExtender(HMODULE hModule)
{
	if(g_Initialized == FALSE)
	{
		g_CliExt = hModule;
		TCHAR path[260];
		if(GetModuleFileName(0 , path, 260))
		{
			wstring wPath(path);
			size_t lastPos = 0;
			size_t temp = wPath.find(L"\\");
			while(temp != wstring::npos)
			{
				lastPos = temp;
				temp = wPath.find(L"\\", lastPos+1);
			}
			if(lastPos > 0)
			{
				wPath = wPath.substr(0, lastPos);
				SetCurrentDirectory(wPath.c_str());
			}
		}
		VIRTUALIZER_START;
		g_Initialized = TRUE;
		const TCHAR* section = _T("Setting");
		TCHAR configFile[256];
		GetCurrentDirectory(MAX_PATH, configFile);
		lstrcat(configFile, _T("\\Client.ini"));

		WCHAR engineName[11] = {L'E', L'n', L'g', L'i', L'n', L'e', L'.', L'd', L'l', L'l', 0 };
		g_Engine = GetModuleHandle(engineName);
		
		g_AuthPort = GetPrivateProfileInt(section, _T("AuthPort"), 2106, configFile);
		Memory::WriteDWORD(((UINT)g_Engine + 0x122085), g_AuthPort);

		CClientSocket::Init();
		g_NetworkHandler.Init();
		g_MemoryProtector.Init();

		WCHAR wCliExt[] = { L'C', L'l', L'i', L'E', L'x', L't', L'.', L'd', L'l', L'l', 0 };
		TCHAR pathToDll[260] = { 0 };
		GetCurrentDirectory(260, pathToDll);
		lstrcat(pathToDll, L"\\");
		lstrcat(pathToDll, wCliExt);
		//calculate dll md5 checksum
		LPBYTE lpFile = 0;
		UINT size = ReadFileBinary(pathToDll, lpFile);
		if (lpFile)
		{
			MD5 md5(lpFile, size);
			string checksum = md5.hexdigest();
			if (checksum.size() == 32)
			{
				for (UINT n = 0; n < 32; n++)
				{
					g_MD5Checksum[n] = static_cast<BYTE>(checksum[n]);
				}
			}
			delete [] lpFile;
		}

		VIRTUALIZER_END;
	}
}

#pragma optimize("", on)
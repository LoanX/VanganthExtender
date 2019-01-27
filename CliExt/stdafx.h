// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#define _WIN32_DCOM
#define WIN32_LEAN_AND_MEAN	
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"psapi.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Dbghelp.lib")
//#pragma comment(lib, "ntdll.lib")
#include <windows.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <shellapi.h>
#include <psapi.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <math.h>
#include <time.h>
#include <tchar.h>
#include "process.h"
#include <Dbghelp.h>
using namespace std;

#include "BaseStruct.h"
#include "BaseEnum.h"
#include "Memory.h"
#include "VirtualizerSDK.h"


extern UINT g_AuthPort;
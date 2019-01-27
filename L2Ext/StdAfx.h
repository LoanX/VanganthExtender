#pragma once

#ifndef _WIN64
#define _WIN64
#endif

#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0500	// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#ifndef __cplusplus
#define __cplusplus
#endif

#pragma warning(disable: 4996)
#pragma warning(disable: 4311)
#pragma warning(disable: 4800)
#pragma warning(disable: 4244)
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Version.lib")
#define _CRT_RAND_S

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#include <winsock2.h>
#include <shellapi.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <Iphlpapi.h>
#include <psapi.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "math.h"
#include <algorithm>
#include <process.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include "tchar.h"
#include <list>
#include <comdef.h>
#include <Wbemidl.h>
#include "ThreadSafe.h"
#include "Virtualizer.h"

#define JMP 0xE9
#define CALL 0xE8
#define EnterWorldPacketSize 107
#define MacAddressSize 17
#define HashSize 32
#define MAX_BUFF_SLOT 40
#define MAKELONGLONG(low, high)      ((INT64)(((LONG)((DWORD_PTR)(low) & 0xffffffff)) | ((INT64)((LONG)((DWORD_PTR)(high) & 0xffffffff))) << 32))
#define LOLONG(l)           ((LONG)((DWORD_PTR)(l) & 0xffffffff))
#define HILONG(l)           ((LONG)((DWORD_PTR)(l) >> 32))
#define ALIGN_QWORD(x) ((((x) + 7) >> 3) << 3)
#define ALIGN_OWORD(x) ((((x) + 15) >> 4) << 4)
#define PI 3.14159265
#define USE_CALL_TRACK 1
using namespace std;

class CLog;
class CObject;
class CCreature;
class CreatureSharedData;
class User;
class CNPC;
class CSummon;
class ClassExtender;
class CPet;
class CIOObject;
class CIOCriticalSection;
class CSocket;
class CUserSocket;
class CPledge;
class CPledgeEx;
class CCastle;
class CPledgeSharedData;
class CCreatureController;
class CParty;
class CItem;
class CItemInfo;
class CSharedItemData;
class CWeapon;
class CInventory;
class COlympiadUser;
class COlympiadField;
class CSharedNoblessInfo;
class ClanSys;
class CLog;
class CPacket;
class CDB;
class CSkillInfo;
class CSkillMod;
class CSkillAction;
class CAttackAction;
class AttackTargetDataType;
class CResidence;
class CResidenceData;
class CSubUnit;
class CAutoCast;
class CSharedRWLock;
class CWareHouseDeposit;
class CWareHouseWithdraw;
class CNpcSocket;
class CSystemMessage;
class MemoryObject;
class CServerPacket;
class CPacketFix;
class PlayerAction;
class CStoreItemData;


typedef map<int, CPledge*> PledgeMap;
typedef map<int, CCastle*> CastleMap;
typedef vector<CPledge*> PledgeVector;
typedef vector<User*> UserVector;
typedef list<User*> UserList;
typedef list<CSocket*> SocketList;
typedef list<CStoreItemData> StoreItemList;
typedef list<int> IntList;
typedef list<CItem*> ItemList;
typedef bool (*_NpcSockFunc)(CNpcSocket*, const unsigned char*);
typedef bool (*_SockFunc)(CSocket*, const unsigned char*);
typedef int ObjectFieldType;

#if defined UNICODE || defined _UNICODE
typedef wstring tstring;
typedef wstringstream   tstringstream;
#else
typedef string tstring;
typedef stringstream   tstringstream;
#endif

#define DllExport __declspec(dllexport)

enum DAY
{
	SUNDAY = 0,
	MONDAY = 1,
	TUESDAY = 2,
	WEDNESDAY = 3,
	THURSDAY = 4,
	FRIDAY = 5,
	SATURDAY = 6
};

extern TCHAR g_ConfigFile[260];

//comment it if you dont want to use it
#define EXTEND_IOBUFFER 1

//Additional Headers
#include "BaseEnum.h"
#include "BaseStruct.h"
#include "yywclex.h"
#include "yywcpars.h"
#include "Random.h"
#include "HookManager.h"
#include "TurboBoost.h"
#include "CallStack.h"
#include "Utils.h"
#include "Logger.h"
#include "SkillInfo.h"
#include "User.h"
#include "Door.h"
#include "BattleCamp.h"
#include "SSQSystem.h"
#include "ResidenceDB.h"
#include "ResidenceDBEx.h"
#include "SiegeAgit.h"
#include "PartyWaitingList.h"
#include "L2Ext.h"
#include "World.h"
#include "NpcMaker.h"
#include "SkillEnchantDB.h"
#include "ParserForSkillEnchant.h"
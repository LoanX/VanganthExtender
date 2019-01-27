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

#ifndef __cplusplus
#define __cplusplus
#endif

#pragma warning(disable: 4996)
#pragma warning(disable: 4311)
#pragma warning(disable: 4800)
#pragma warning(disable: 4244)
#pragma comment(lib, "Ws2_32.lib")

#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <time.h>
#include "math.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include "ThreadSafe.h"
#define JMP 0xE9
#define CALL 0xE8
#define EnterWorldPacketSize 107
#define MacAddressSize 17
#define HashSize 32
#define MAX_BUFF_SLOT 34
#define MAKELONGLONG(low, high)      ((INT64)(((LONG)((DWORD_PTR)(low) & 0xffffffff)) | ((INT64)((LONG)((DWORD_PTR)(high) & 0xffffffff))) << 32))
#define LOLONG(l)           ((LONG)((DWORD_PTR)(l) & 0xffffffff))
#define HILONG(l)           ((LONG)((DWORD_PTR)(l) >> 32))
#define ALIGN_QWORD(x) ((((x) + 7) >> 3) << 3)
#define ALIGN_OWORD(x) ((((x) + 15) >> 4) << 4)
#define PI 3.14159265
using namespace std;

class CLog;
class CObject;
class CCreature;
class CSharedData;
class CAbnormalData;
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
class vPosition;
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
typedef map<int, int> PledgeSkillMap;
typedef map<int, CCastle*> CastleMap;
typedef map<int, CSubUnit> SubUnitMap;
typedef vector<CPledge*> PledgeVector;
typedef vector<User*> UserVector;
typedef list<User*> UserList;
typedef list<CSocket*> SocketList;
typedef list<CStoreItemData> StoreItemList;
typedef bool (*_NpcSockFunc)(CNpcSocket*, PUCHAR);
typedef bool (*_SockFunc)(CSocket*, PUCHAR);
typedef INT64 Native;
typedef vPosition* lpvPosition;
typedef int ObjectFieldType;

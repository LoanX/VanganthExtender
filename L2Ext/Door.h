#pragma once
#include "StaticObject.h"
#include "IOCriticalSection.h"

//size 1E8h
class CDoor : public CStaticObject
{
public:
	/* 038 */ LPCIOCriticalSection pLock;
	/* 040 */ DWORD damageGrade;
	/* 044 */ DWORD _unkn044;
	/* 048 */ FVector pos;
	/* 060 */ DWORD _unkn060;
	/* 064 */ DWORD _unkn064;
	/* 068 */ FVector range[4];
	/* 0C8 */ DWORD _unkn0C8;
	/* 0CC */ DWORD _unkn0CC;
	/* 0D0 */ DWORD _unkn0D0;
	/* 0D4 */ DWORD _unkn0D4;
	/* 0D8 */ WCHAR wName[25];
	/* 13C */ DWORD _unkn13C;
	/* 140 */ DWORD _unkn140;
	/* 144 */ DWORD _unkn144;
	/* 148 */ DWORD _unkn148;
	/* 14C */ DWORD type;	/* Door type: parent_type = 2, normal_type = 3 */
	/* 150 */ DWORD _unkn150;
	/* 154 */ DWORD _unkn154;
	/* 158 */ DWORD _unkn158;
	/* 15C */ DWORD _unkn15C;
	/* 160 */ DWORD _unkn160;	//vector partent doors
	/* 1B8    double baseHp; */


	bool IsValidDoor();
	void Close(bool byParent = false);
	void Open(bool byParent = false);
	bool IsEnemyTo(CCreature *pAttacker);
	void SendDoorInfo(CUserSocket *pSocket);
};

class CDoorDB
{
	CDoorDB *instance;
public:
	CDoorDB();
	void AddDoor(const WCHAR* name, CDoor* pDoor);
	CDoor* GetDoor(WCHAR* name);
	CDoor* GetDoor(const WCHAR* name);
	CDoor* GetDoor(int id);
	bool IsEnemyTo(CCreature *pAttacker);
};

extern CDoorDB g_DoorDB;
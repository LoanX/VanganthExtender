#include "StdAfx.h"

bool CDoor::IsValidDoor()
{
	if(this)
	{
		try
		{
			if((*(LPDWORD)this) == 0x9E20F8)
				return true;
		}catch(...)
		{
		}
	}
	return false;
}

void CDoor::Close(bool byParent)
{
	typedef void(*f)(CDoor*, bool);
	f(0x59A3A0L)(this, byParent);
}

void CDoor::Open(bool byParent)
{
	typedef void(*f)(CDoor*, bool);
	f(0x59A140L)(this, byParent);
}

bool CDoor::IsEnemyTo(CCreature *pAttacker)
{
	typedef bool (*f)(CDoor*, CCreature*);
	return f(0x598EC0L)(this, pAttacker);
}

void CDoor::SendDoorInfo(CUserSocket *pSocket)
{
	if(this)
	{
		bool showHp = false;
		if(pSD->breakable && type!= 3)
		{
			if(pSD->hp < pSD->maxHp)
				showHp = true;
		}

		if(pSocket->ValidUserSocket())
		{
			bool isEnemyTo = false;
			if(User *pUser = pSocket->GetUser())
			{
				isEnemyTo = IsEnemyTo(pUser);
			}

			pSocket->Send("cddddddddd", 0x4C, nObjectID, pSD->id, 1, 1, isEnemyTo, (int)pSD->hp, (int)pSD->maxHp, showHp, damageGrade );
		}else
		{
			FVector position = pos;
			typedef void (*f)(INT64, int, DWORD, FVector*, int, const char* format, BYTE, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
			f(0x403DE0L)(0x0E427AA8L, 0, 0x8000, &pos, 16384, "cddddddddd", 0x4C, nObjectID, pSD->id, 1, 1, false, (int)pSD->hp, (int)pSD->maxHp, showHp, damageGrade );
		}
	}
}

//CDoorDB
CDoorDB::CDoorDB()
{
	instance = (CDoorDB*)0x155E250;
}

void CDoorDB::AddDoor(const WCHAR* name, CDoor* pDoor)
{
	typedef void (*f)(CDoorDB*, const WCHAR*, CDoor*);
	f(0x59C2F0L)(instance, name, pDoor);
}

CDoor* CDoorDB::GetDoor(WCHAR* name)
{
	typedef CDoor*(*f)(CDoorDB*, WCHAR*);
	return f(0x599800L)(instance, name);
}

CDoor* CDoorDB::GetDoor(const WCHAR* name)
{
	typedef CDoor*(*f)(CDoorDB*, const WCHAR*);
	return f(0x599800L)(instance, name);
}

CDoor* CDoorDB::GetDoor(int id)
{
	typedef CDoor*(*f)(CDoorDB*, int);
	return f(0x599B00L)(instance, id);
}


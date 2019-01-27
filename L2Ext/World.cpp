#include "StdAfx.h"

double arrRaceRadius[5][2][2];
double arrRaceHeight[5][2][2];

CWorld::CWorld()
{ 
	pInstance = (CWorld*) 0x0E8AA840;
	InitializeCriticalSection(&m_lock);
	
	
	arrRaceRadius[RaceDwarf][SexFemale][0] = 5.0;
	arrRaceHeight[RaceDwarf][SexFemale][0] = 19;
	arrRaceRadius[RaceDwarf][SexMale][0] = 9.0;
	arrRaceHeight[RaceDwarf][SexMale][0] = 18;

	arrRaceRadius[RaceDwarf][SexFemale][1] = 5.0;
	arrRaceHeight[RaceDwarf][SexFemale][1] = 19;
	arrRaceRadius[RaceDwarf][SexMale][1] = 9.0;
	arrRaceHeight[RaceDwarf][SexMale][1] = 18;


	arrRaceRadius[RaceOrc][SexFemale][0] = 7.0;
	arrRaceHeight[RaceOrc][SexFemale][0] = 27;
	arrRaceRadius[RaceOrc][SexMale][0] = 11;
	arrRaceHeight[RaceOrc][SexMale][0] = 28;
	arrRaceRadius[RaceOrc][SexFemale][1] = 8.0;
	arrRaceHeight[RaceOrc][SexFemale][1] = 25.5;
	arrRaceRadius[RaceOrc][SexMale][1] = 7;
	arrRaceHeight[RaceOrc][SexMale][1] = 27.5;


	arrRaceRadius[RaceDarkElf][SexFemale][0] = 7.0;
	arrRaceHeight[RaceDarkElf][SexFemale][0] = 23.5;
	arrRaceRadius[RaceDarkElf][SexMale][0] = 7.5;
	arrRaceHeight[RaceDarkElf][SexMale][0] = 24;
	arrRaceRadius[RaceDarkElf][SexFemale][1] = 7.0;
	arrRaceHeight[RaceDarkElf][SexFemale][1] = 23.5;
	arrRaceRadius[RaceDarkElf][SexMale][1] = 7.5;
	arrRaceHeight[RaceDarkElf][SexMale][1] = 24;

	arrRaceRadius[RaceElf][SexFemale][0] = 7.5;
	arrRaceHeight[RaceElf][SexFemale][0] = 23;
	arrRaceRadius[RaceElf][SexMale][0] = 7.5;
	arrRaceHeight[RaceElf][SexMale][0] = 24;
	arrRaceRadius[RaceElf][SexFemale][1] = 7.5;
	arrRaceHeight[RaceElf][SexFemale][1] = 23;
	arrRaceRadius[RaceElf][SexMale][1] = 7.5;
	arrRaceHeight[RaceElf][SexMale][1] = 24;

	arrRaceRadius[RaceHuman][SexFemale][0] = 8.0;
	arrRaceHeight[RaceHuman][SexFemale][0] = 23.5;
	arrRaceRadius[RaceHuman][SexMale][0] = 9.0;
	arrRaceHeight[RaceHuman][SexMale][0] = 23;
	arrRaceRadius[RaceHuman][SexFemale][1] = 6.5;
	arrRaceHeight[RaceHuman][SexFemale][1] = 22.5;
	arrRaceRadius[RaceHuman][SexMale][1] = 7.5;
	arrRaceHeight[RaceHuman][SexMale][1] = 22.8;

};

CWorld::~CWorld()
{
	DeleteCriticalSection(&m_lock);
}

void CWorld::AddToDelete(CItem *pItem)
{
	EnterCriticalSection(&m_lock);
	m_ItemsToDelete.push_back(pItem);
	LeaveCriticalSection(&m_lock);
}

void CWorld::DeleteWaitingItem(CItem *pItem)
{
	guard;
	EnterCriticalSection(&m_lock);
	for(list<CItem*>::iterator Iter = m_ItemsToDelete.begin(); Iter!= m_ItemsToDelete.end();Iter++)
	{
		CItem *pItemEx = (*Iter);
		if(pItemEx->IsValidItem())
		{
			if(pItem->nObjectID == pItemEx->nObjectID)
			{
				m_ItemsToDelete.erase(Iter);
				pItem->pSID->nItemState = 2;
				pItem->AddRef(__FILE__, __LINE__, 17);
				pItem->Delete();
				break;
			}
		}
	}
	LeaveCriticalSection(&m_lock);
	unguard;
}

void ASYNCHRONOUS_SERIALIZE_IMPL1(CWorld *pWorld, DeleteItemStruct* lpData)
{
	typedef void(*f)(CWorld*, DeleteItemStruct*);
	f(0x8B3BE0)(pWorld, lpData);
	if(lpData->pItem->IsValidItem())
	{
		g_World.DeleteWaitingItem(lpData->pItem);
	}
}

void CWorld::DeleteItemFromWorld(CItem *pItem, bool broadcast)
{
	if(pItem->IsValidItem() && broadcast)
	{
		FVector pos = pItem->pSID->position;

		CHAR buff[128];
		int size = Assemble(buff, 128, "cdd", 0x12, pItem->nObjectID, 0);
		
		L2Server::BroadcastToNeighborDist(NULL, &pos, 40960, size, buff);
	}

	DeleteItemStruct dis;
	dis.addr = 0xA55AD0;
	dis.pItem = pItem;

	typedef void (*f)(INT64, INT64, LPVOID, DeleteItemStruct*);
	f(0x778AD0L)(0x0E8AA858, 0x0E8AA840, ASYNCHRONOUS_SERIALIZE_IMPL1, &dis );
}

double CWorld::GetRadius(DWORD Race, DWORD Sex, DWORD Magic)
{	
	if(Race < 5 && Sex < 2 && Magic < 2)
	{
		return arrRaceRadius[Race][Sex][Magic];
	}
	return 9;
}

double CWorld::GetHeight(DWORD Race, DWORD Sex, DWORD Magic)
{
	if(Race < 5 && Sex < 2 && Magic < 2)
	{
		return arrRaceHeight[Race][Sex][Magic];
	}
	return 23;
}


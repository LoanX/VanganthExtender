#include "stdafx.h"
#include "ItemBonus.h"

CItemBonus::CItemBonus()
{
	InitializeCriticalSection(&_lock);
}

CItemBonus::~CItemBonus()
{
	DeleteCriticalSection(&_lock);
}

void CItemBonus::Clear()
{
	guard;
	Lock();
	items.clear();
	Unlock();
	unguard;
}

void CItemBonus::Add(INT32 itemId, double bonus)
{
	guard;
	Lock();
	map<INT32, double>::iterator it = items.find(itemId);
	if(it!=items.end())
	{
		it->second += bonus;
	}else
	{
		double value = 1.0 + bonus;
		items.insert(pair<INT32, double>(itemId, value));
	}
	Unlock();
	unguard;
}

double CItemBonus::Get(INT32 itemId)
{
	guard;
	double value = 1.0;
	Lock();
	map<INT32, double>::iterator it = items.find(itemId);
	if(it!=items.end())
	{
		value = it->second;
	}
	Unlock();
	unguard;
	return value;
}

void CItemBonus::GetAll(std::map<INT32,double> &bonusMap)
{
	guard;
	Lock();
	for(map<INT32, double>::iterator it = items.begin();it!=items.end();it++)
	{
		bonusMap.insert(pair<INT32, double>(it->first, it->second));
	}
	Unlock();
	unguard;
}


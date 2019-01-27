#include "StdAfx.h"

CItem * CInventory::GetFirstItemByClassID(int nItemID)
{
	if(this)
	{
		typedef CItem * ( * f)(CInventory*, int);
		return f(0x00613A10L)(this, nItemID);
	}
	return  NULL;
}

CItem *CInventory::GetNextItemByClassID(int nBeforeIndex)
{
	if(this)
	{
		typedef CItem* (*f)(CInventory*, int);
		return f(0x00613E50L)(this, nBeforeIndex);
	}
	return NULL;
}

CItem *CInventory::GetItemBySID(UINT nSID)
{
	if(this)
	{
		typedef CItem* (*f)(CInventory*, UINT);
		return f(0x00612880L)(this, nSID);
	}
	return NULL;
}

CItem * CInventory::GetItemByIndex(int nIndex)
{
	if(nIndex)
	{
		typedef CItem* (*f)(CInventory*, int);
		return f(0x612B40L)(this, nIndex);
	}
	return 0;
}

bool CInventory::HaveItemByClassID(int nItemID, int nAmount)
{
	typedef bool(*f)(CInventory*, int, int);
	return f(0x615750L)(this, nItemID, nAmount);
}

CItem *CInventory::GetItemByDBID(int nDBID)
{
	if(this)
	{
		typedef CItem*(*f)(CInventory*, int);
		return f(0x006125C0L)(this, nDBID);
	}
	return NULL;
}

int CInventory::GetAdenaAmount()
{
	typedef int (*f)(CInventory*);
	return f(0x006130F0L)(this);
}

bool CInventory::SetInventoryChanged(CItem *pItem, int nChangedType)
{
	typedef bool(* f)(CInventory*, CItem*, int);
	return f(0x0061C0B0L)(this, pItem, nChangedType);
}

bool CInventory::Pop(int nItemIndex, bool bFlag)
{
	typedef bool (*f)(CInventory*, int, bool);
	return f(0x006166C0L)(this, nItemIndex, bFlag);
}

void CInventory::WriteLock(PWCHAR File, int nLine)
{
	typedef void (*f)(CInventory*, PWCHAR, int);
	f(0x00442EF0L)(this, File, nLine);
}

void CInventory::WriteUnlock()
{
	typedef void (*f)(CInventory*);
	f(0x004438D0L)(this);
}

int CInventory::GetTotalCount()
{
	typedef int (*f)(CInventory*);
	return f(0x00616D30L)(this);
}

CCreature *CInventory::GetOwner()
{
	typedef CCreature* (*f)(CInventory*);
	return f(0x615140L)(this);
}

bool CInventory::CheckAddable(int nClassID, int nAmount)
{
	typedef bool (*f)(CInventory*, int, int);
	return f(0x618830L)(this, nClassID, nAmount);
}

INT32 CInventory::GetFirstItemIndex()
{
	typedef INT32 (*f)(CInventory*);
	return f(0x611940L)(this);
}

void CInventory::ReadLock(const WCHAR* file, int line)
{
	typedef void(*f)(CInventory*, const WCHAR*, int);
	f(0x4D2070L)(this, file, line);
}

void CInventory::ReadUnlock()
{
	typedef void(*f)(CInventory*);
	f(0x4D23C0L)(this);
}
#pragma once

#include "class.h"

class CWareHouse : public MemoryObject
{
public:
/* 004 */ DWORD n04;
/* 008 */ DWORD n08;
/* 00C */ DWORD n0C;
/* 010 */ DWORD n10;
/* 014 */ DWORD n14;
/* 018 */ DWORD n18;
/* 01C */ DWORD n1C;
/* 020 */ DWORD n20;
/* 024 */ DWORD n24;
/* 028 */ DWORD n28;
/* 02C */ DWORD n2C;
/* 030 */ DWORD n30;
/* 034 */ DWORD n34;
/* 038 */ DWORD n38;
/* 03C */ DWORD n3C;
/* 040 */ DWORD n40;
/* 044 */ DWORD n44;
/* 048 */ DWORD n48;
/* 04C */ DWORD n4C;
/* 050 */ DWORD n50;
/* 054 */ DWORD n54;
/* 058 */ DWORD n58;
/* 05C */ DWORD n5C;
/* 060 */ DWORD nItemCount;
/* 064 */ DWORD n64;
/* 068 */ DWORD nUserID;
/* 06C */ DWORD n6C;
/* 070 */ DWORD nWareHouseNum;
/* 074 */ DWORD n74;
/* 078 */ DWORD n78;
/* 07C */ DWORD n7C;
/* 080 */ DWORD n80;
/* 084 */ DWORD n84;
/* 088 */ DWORD n88;
/* 08C */ DWORD n8C;
public:
	CItemSP GetItem(int nItemID)
	{
		typedef CItemSP* (__thiscall *_gi)(CWareHouse*, CItemSP*, int);
		_gi gi = (_gi) 0x004BC120;
		CItemSP tmp;
		gi(this, &tmp, nItemID);
		return tmp;
	};
	CItemSP GetItemByType(int nItemType, int nNPCID)
	{
		typedef CItemSP* (__thiscall *f)(CWareHouse*, CItemSP*, int, int);
		CItemSP tmp;
		f(0x4B8C10L)(this, &tmp, nItemType, nNPCID);
		return tmp;
	};
	bool DeleteItem(int nItemID, int nItemAmount)
	{
		typedef bool (__thiscall *f)(CWareHouse*, int, int);
		return f(0x4B77A0L)(this, nItemID, nItemAmount);
	}
	void WriteLock()
	{
		typedef void(__thiscall *f)(CWareHouse*);
		f(0x4B1830L)(this);
	}
	void WriteUnlock()
	{
		typedef void(__thiscall *f)(CWareHouse*);
		f(0x4B18E0L)(this);
	}
	void ReadLock()
	{
		typedef void(__thiscall *f)(CWareHouse*);
		f(0x4B1990L)(this);
	}
	void ReadUnlock()
	{
		typedef void(__thiscall *f)(CWareHouse*);
		f(0x4B1A40L)(this);
	}
};
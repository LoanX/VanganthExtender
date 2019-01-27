#pragma once
#include "MemoryObject.h"

#pragma pack( push, 1 )
class CItem	: public MemoryObject
{
public:
	/* 004 */	DWORD nUnk004;
	/* 008 */	DWORD nUnk008;
	/* 00C */	DWORD nUnk00C;
	/* 010 */	DWORD nUnk010;
	/* 014 */	DWORD nUnk014;
	/* 018 */	DWORD nItemID;
	/* 01C */	DWORD nCharID;
	/* 020 */	DWORD nItemClassID;
	/* 024 */	DWORD nAmount;
	/* 028 */	DWORD nEnchant;
	/* 02C */	DWORD nEroded;
	/* 030 */	DWORD nBless;
	/* 034 */	DWORD nIdent;
	/* 038 */	DWORD nWished;
	/* 03C */	DWORD nWarehouse;
	/* 040 */	bool bUnkn040;
	/* 041 */	bool bNotSaved;	//For write cache option
	/* 042 */	bool bUnkn042;
	/* 043 */	bool bUnkn043;
	/* 044 */	DWORD nUnk044;
	/* 048 */	DWORD nUnk048;
	/* 04C */	DWORD nUnk04C;
	/* 050 */	DWORD nUnk050;
	/* 054 */	DWORD nUnk054;
//	/* 058 */	DWORD nAugmentationID;
//	/* 05C */	DWORD nManaLeft;
public:
	bool Save(bool bForceSave)
	{
		typedef bool (__thiscall *_save)(CItem*, bool);
		_save save = (_save) 0x00442C10;
		return save(this, bForceSave);
	}
};
#pragma pack( pop )
#include "StdAfx.h"

bool CPet::DropItem(CItem *pItem, int nAmount)
{
	typedef bool (__fastcall * __DropItem)(CPet*, CItem*, int nAmount, FVector*);
	__DropItem _DropItem = (__DropItem) 0x00749E70;
	return _DropItem(this, pItem, nAmount, &pSD->Pos);
}
bool CPet::GetItem(CItem *pItem, int nAmount)
{
	typedef bool(__fastcall *__ct)(CPet*, CItem*, int nAmount);
	__ct _ct = (__ct) 0x00748BE0;
	return _ct(this, pItem, nAmount);
}
bool CPet::UseItem(CItem *pItem, int nForce)
{
	typedef bool (__fastcall * UseItemS) (CPet*, CItem*, int nForce);
	UseItemS _UseItem = (UseItemS) 0x004F9C40;
	if(pItem)
		return _UseItem(this, pItem, nForce);
	else return false;
}
#include "StdAfx.h"
void CSummon::ChangeTarget(CCreature *pObject, bool ByUser)
{
	typedef void (__fastcall *__CT)(CCreature*, CCreature*, bool bByUser);
	__CT _CT = (__CT) 0x004EEA70;
	_CT(this, pObject, ByUser);
}
User *CSummon::GetMaster()
{
	typedef User*(__fastcall *__GetMaster)(CSummon *);
	__GetMaster _GetMaster = (__GetMaster) 0x0074CAA0;
	if(this)
		return _GetMaster(this);
	return NULL;
}

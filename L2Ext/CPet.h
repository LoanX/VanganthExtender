#pragma once
#include "CCreature.h"

class CPet: public CSummon
{
public:
	bool DropItem(CItem *pItem, int nAmount);
	bool GetItem(CItem *pItem, int nAmount);
	bool UseItem(CItem *pItem, int nForce);
};
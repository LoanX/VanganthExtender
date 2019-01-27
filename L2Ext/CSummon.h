#pragma once

#include "CCreature.h"

class CSummon: public CNPC
{
public:
	void ChangeTarget(CCreature *pCreature, bool ByUser);
	User *GetMaster();
};
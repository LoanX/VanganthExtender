#include "StdAfx.h"

bool CreatureSharedData::RemoveMemo(int nQuestID)
{
	typedef bool (*_rm)(CreatureSharedData*, int);
	_rm rm = (_rm) 0x00429D60;
	return rm(this, nQuestID);
}

void CreatureSharedData::DeleteAbnormalStatus(int nAbnormalType)
{
	typedef void (*das)(CreatureSharedData*, int);
	das _das = (das) 0x004D9510;
	_das(this, nAbnormalType);
}

void CreatureSharedData::AddAbnormalStatus(int nAbnormalType, int nLevel)
{
	typedef void(*aas)(CreatureSharedData*, int, int);
	aas _aas = (aas) 0x004D9640;
	_aas(this, nAbnormalType, nLevel);
}

bool CreatureSharedData::SetMemo(int QuestID, int Slot, int State)
{
	typedef bool (*f)(CreatureSharedData*, int, int, int);
	return f(0x431E10)(this, QuestID, Slot, State);
}
#include "L2Ext.h"

int GetMaxAbnormals(CCreature *pCreature, CSkillInfo *pSI)
{
		return BuffSlots::GetMax(pCreature, pSI);
};

void BuffSlots::Initialize()
{
	WriteHook(JMP, 0x5057FF, (DWORD)CheckBuffAmount, 1);
};

int BuffSlots::GetMax(CCreature *pCreature, CSkillInfo *pSI)
{
	int nAmount = 20; //Standard amount of buffs
	if(pCreature->ValidCreature())
	{
		int nMaxBuffSlot = pCreature->GetMaxBuffSlot(); //Divine Inpspiration
		int nBuffAmount = pCreature->GetBuffAmount();
		int nDeBuffAmount = pCreature->GetDebuffAmount();
		nAmount = nMaxBuffSlot;
		try
		{
			if(pSI)
			{
				if(pSI->nDebuff)
					return 34;
				else
				{
					nAmount = nMaxBuffSlot+nDeBuffAmount;
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}

	}
	return nAmount;
};
#include "StdAfx.h"

UINT g_BaseBuffLimit = 20;
UINT g_SelfBuffLimit = 0;

int GetMaxAbnormals(CCreature *pCreature, CSkillInfo *pSI)
{
	return BuffSlots::GetMax(pCreature, pSI);
}

void BuffSlots::Initialize()
{
	g_HookManager.WriteJump( 0x5057FF, CheckBuffAmount, 1);
	g_BaseBuffLimit = GetPrivateProfileInt(_T("SkillFactory"), _T("BaseBuffLimit"), 20, g_ConfigFile);
	g_SelfBuffLimit = GetPrivateProfileInt(_T("SkillFactory"), _T("SelfBuffLimit"), 0, g_ConfigFile);
	if( (g_BaseBuffLimit + g_SelfBuffLimit) > 40)
	{
		g_BaseBuffLimit = 40;
		g_SelfBuffLimit = 0;
	}
}
int BuffSlots::GetMax(CCreature *pCreature, CSkillInfo *pSI)
{
	int nAmount = 20; //Standard amount of buffs
	if(pCreature->ValidCreature())
	{
		pCreature->abnormalStatusLock.Lock(__FILEW__, __LINE__);
		
		UINT nMaxBuffSlot = g_BaseBuffLimit;

		try
		{
			//divine inspiration
			nMaxBuffSlot += pCreature->pSD->pExData->SkillMod.nEnlargeBuffSlots;
		}catch(...)
		{
			nMaxBuffSlot = g_BaseBuffLimit;
		}
		
		UINT nBuffAmount = 0, nDeBuffAmount = 0, nSelfBuffAmount = 0;
		if(g_SelfBuffLimit > 0)
		{
			pCreature->GetBuffDebuffAmount(nBuffAmount, nDeBuffAmount, nSelfBuffAmount);
		}else
		{
			pCreature->GetBuffDebuffAmount(nBuffAmount, nDeBuffAmount);
		}
		UINT nAbnormalAmount = pCreature->GetAbnormalAmount();
		
		try
		{
			if(nAbnormalAmount > 39)
			{
				nAmount = 40;
			}else if(pSI)
			{
				if(pSI->nDebuff == 0)
				{	//BUFF
					if(g_SelfBuffLimit != 0 && pSI->IsSelfBuff())
					{
						//selfbuff
						if(nSelfBuffAmount >= g_SelfBuffLimit)
						{
							nAmount = nAbnormalAmount;
						}else
						{
							nAmount = 40;
						}
					}else if(nBuffAmount >= nMaxBuffSlot)
					{
						nAmount = nAbnormalAmount;
					}else
					{
						nAmount = 40;
					}
				}else if(pSI->nDebuff == 1)
				{	//DEBUFF
					if(nDeBuffAmount >= 10)
					{
						nAmount = nAbnormalAmount;	//34
					}else
					{
						nAmount = 40;
					}
				}else if(pSI->nDebuff == 2)
				{
					nAmount = 40;
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
		pCreature->abnormalStatusLock.Unlock();
	}
	return nAmount;
}
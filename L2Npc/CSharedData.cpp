#include "L2NPC.h"

void CFusionSkill::CheckTimeOut()
{
	DWORD nTime = time(NULL);
	DWORD nTimeOut = 20; //20 seconds;
	for(int n=0;n<2;n++)
	{
		for(int m=0;m<3;m++)
		{
			if(nCasterSID[n][m])
			{
				if((nStartTime[n][m] + nTimeOut) < nTime)
				{
					nCasterSID[n][m] = NULL;
					nStartTime[n][m] = NULL;
				}
			}
		}
	}
}

int CFusionSkill::GetLevel(ForceType Type)
{
	int nLevel = 0;
	for(int n=0;n<3;n++)
	{
		if(nCasterSID[Type][n])
			nLevel++;
	}
	return nLevel;
}

bool CFusionSkill::Add(int nCasterID, ForceType Type)
{
	for(int n=0;n<3;n++)
	{
		if(nCasterSID[Type][n] == NULL)
		{
			nCasterSID[Type][n] = nCasterID;
			nStartTime[Type][n] = time(NULL);
			return true;
		}
	}
	return false;
}

void CFusionSkill::Del(int nCasterID)
{
	for(int n=0;n<2;n++)
	{
		for(int m=0;m<3;m++)
		{
			if(nCasterSID[n][m] == nCasterID)
			{
				nCasterSID[n][m] = NULL;
				nStartTime[n][m] = NULL;
				return;
			}
		}
	}
}

void CFusionSkill::Clear()
{
	for(int n=0;n<2;n++)
	{
		for(int m=0;m<3;m++)
		{
			nCasterSID[n][m] = NULL;
			nStartTime[n][m] = NULL;
		}
	}
}

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
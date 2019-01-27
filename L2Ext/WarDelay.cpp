#include "StdAfx.h"

void CWarDelay::DelDelay(int nPledgeID)
{
	if(nPledgeID)
	{
		for(int n=0;n<30;n++)
		{
			if(Info[n].nPledgeID == nPledgeID)
			{
				Info[n].nPledgeID = 0;
				Info[n].StartTime = 0;
				break;
			}
		}
	}
}

void CWarDelay::StartDelay(int nPledgeID)
{
	for(int n=0;n<30;n++)
	{
		if(Info[n].nPledgeID == nPledgeID)
		{
			Info[n].StartTime = (DWORD)time(NULL);
			break;
		}else if(Info[n].nPledgeID == 0)
		{
			Info[n].nPledgeID = nPledgeID;
			Info[n].StartTime = (DWORD)time(NULL);
			break;
		}
	}
}

bool CWarDelay::HasDelay(int nPledgeID)
	{
		DWORD nTimeout = g_Config.ClanInfo.GetWarDelay(); 
		for(int n=0;n<30;n++)
		{
			if(Info[n].nPledgeID == nPledgeID)
			{
				if((Info[n].StartTime + nTimeout) > (DWORD)time(NULL))
					return true;
				else
					return false;
			}
		}
		return false;
	}
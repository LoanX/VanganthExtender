#include "StdAfx.h"
#include "DB.h"

bool CAgit::ResetOwner(bool AffectOnlyAgit)
{
	if(this && IsAgit())
	{
		if(pRD)
		{
			if(pRD->nPledgeID > 0)
			{
				CPledge *pPledge = g_PledgeDB.GetPledge(pRD->nPledgeID);
				if(pPledge)
				{
					if(!AffectOnlyAgit)
					{
						if(pPledge->pSD)
							pPledge->pSD->nAgitID = 0;
						pPledge->pAgit = NULL;
						pPledge->Save(7);
					}
					SetPledge(NULL);
					g_DB.RequestOwnAgit(pRD->nResidenceID, 0);
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Pledge not loaded yet[%d]", __FUNCTION__, pRD->nPledgeID);
				}
			}
		}
	}
	return false;
}
#include "StdAfx.h"

CHerbAbnormalMgr::CHerbAbnormalMgr()
{
	InitializeCriticalSection(&_lock);
	Clear(-1);
}

CHerbAbnormalMgr::~CHerbAbnormalMgr()
{
	DeleteCriticalSection(&_lock);
}

void CHerbAbnormalMgr::Clear(unsigned int idx)
{
	EnterCriticalSection(&_lock);
	if(idx < 8)
	{
		data[idx].casterSID = 0;
		data[idx].expireTime = 0;
		data[idx].partyBuff = 0;
		data[idx].pSI = NULL;
		data[idx].selfBuff = 0;
		data[idx].herbExpireTime = NULL;
		count--;
	}else
	{
		for(int n=0;n<8;n++)
		{
			data[n].casterSID = 0;
			data[n].expireTime = 0;
			data[n].partyBuff = 0;
			data[n].pSI = NULL;
			data[n].selfBuff = 0;
			data[n].herbExpireTime = NULL;
		}
		count = 0;
	}
	LeaveCriticalSection(&_lock);
}

bool CHerbAbnormalMgr::AddAbnormal(AbnormalData abnormal)
{
	EnterCriticalSection(&_lock);
	if(count < 8)
	{
		for(int n=0;n<8;n++)
		{
			if(data[n].pSI == NULL)
			{
				data[n].casterSID = abnormal.casterSID;
				data[n].expireTime = abnormal.expireTime;
				data[n].partyBuff = abnormal.partyBuff;
				data[n].pSI = abnormal.pSI;
				data[n].selfBuff = abnormal.selfBuff;
				data[n].herbExpireTime = (time(NULL) + 130);
				count++;
				break;
			}
		}
	}
	LeaveCriticalSection(&_lock);
	return false;
}

void CHerbAbnormalMgr::UpdateExpireTime(int abnormalType)
{
	EnterCriticalSection(&_lock);
	if(count > 0)
	{
		for(int n=0;n<8;n++)
		{
			if(data[n].pSI != NULL)
			{
				if(data[n].pSI->nAbnormalType == abnormalType)
				{
					data[n].herbExpireTime = (time(NULL) + 130);
					break;
				}
			}
		}
	}
	LeaveCriticalSection(&_lock);
}

bool CHerbAbnormalMgr::RestoreAbnormal(CCreature *pCreature, int AbnormalType)
{
	if(pCreature->ValidCreature())
	{
		EnterCriticalSection(&_lock);
		if(count > 0)
		{
			for(int n=0;n<8;n++)
			{
				if(data[n].pSI != NULL)
				{
					if(data[n].pSI->nAbnormalType == AbnormalType)
					{
						AbnormalData abnormal = data[n];
						pCreature->vAbnormalStatus.push_back(abnormal);
						pCreature->pSD->AddAbnormalStatus(AbnormalType, abnormal.pSI->nAbnormalLevel);
						Clear(n);
						break;
					}
				}
			}
		}
		LeaveCriticalSection(&_lock);
	}
	return false;
}

void CHerbAbnormalMgr::TimerExpired(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		EnterCriticalSection(&_lock);
		if(count > 0)
		{
			time_t currentTime = time(NULL);
			for(int n=0;n<8;n++)
			{
				if(data[n].pSI != NULL)
				{
					if(data[n].herbExpireTime < currentTime)
					{
						Clear(n);
					}
				}
			}
		}
		LeaveCriticalSection(&_lock);
	}
}
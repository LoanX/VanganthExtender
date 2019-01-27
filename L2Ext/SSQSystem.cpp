#include "StdAfx.h"

SSQSystem* SSQSystem::GetInstance()
{
	typedef SSQSystem* (*f)();
	return f(0x7D6130L)();		
}

int SSQSystem::GetWinner()
{
	if(this)
	{
		try
		{
			typedef int (*f)(SSQSystem*);
			return f(0x443AE0L)(this);
		}catch(...)
		{
			EXCEPTION_LOG;
		}
	}
	return 0;
}

int SSQSystem::GetSealOwner(int nSealNum)
{
	if(this)
	{
		try
		{
			typedef int (*f)(SSQSystem*, int);
			return f(0x443BB0L)(this, nSealNum);
		}catch(...)
		{
			EXCEPTION_LOG;
		}
	}
	return 0;
}

void SSQSystem::ValidateCpSkill(User *pUser)
{
	try
	{
		if(pUser->ValidUser())
		{
			bool Validate = false;
			int Owner = SSQSystem::GetInstance()->GetSealOwner(SSQSeal::seal3);
			if(Owner != SSQPart::none)
			{
				if(pUser->pSD->nSSQPart > 0)
				{
					if(pUser->pSD->nSSQPart == Owner)
					{
						//won
						if(pUser->HaveSkill(5075, 1))
						{
							pUser->DeleteSkill(5075);
							Validate = true;
						}
						if(!pUser->HaveSkill(5074, 1))
						{
							pUser->AcquireSkill(5074, 1);
							Validate = true;
						}
					}else
					{
						//lost
						if(pUser->HaveSkill(5074, 1))
						{
							pUser->DeleteSkill(5074);
							Validate = true;
						}
						if(!pUser->HaveSkill(5075, 1))
						{
							pUser->AcquireSkill(5075, 1);
							Validate = true;
						}
					}
				}else
				{
					//strife got owner but player didnt participe
					if(pUser->HaveSkill(5074, 1))
					{
						pUser->DeleteSkill(5074);
						Validate = true;
					}
					if(pUser->HaveSkill(5075, 1))
					{
						pUser->DeleteSkill(5075);
						Validate = true;
					}
				}
			}else
			{
				//no1 owns strife
				if(pUser->HaveSkill(5074, 1))
				{
					pUser->DeleteSkill(5074);
					Validate = true;
				}
				if(pUser->HaveSkill(5075, 1))
				{
					pUser->DeleteSkill(5075);
					Validate = true;
				}
			}
			if(Validate)
			{
				pUser->lpSharedDataLock->Lock(__FILEW__, __LINE__);
				pUser->ValidateAllOnSIM();
				pUser->lpSharedDataLock->Unlock();
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
}
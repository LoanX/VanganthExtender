#pragma once
#include "Job.h"

class CJobHandler
{
	CSLock m_Lock;
	vector<CJob*> m_Jobs;
	time_t m_ExpireTime;
public:
	CJobHandler();
	virtual void AddJob(CJob *pJob);
	virtual void TimerExpired();
	virtual ~CJobHandler();
	inline time_t GetExpireTime() { return m_ExpireTime; };
	inline SIZE_T GetJobCount() { return m_Jobs.size(); };
};

extern CJobHandler g_JobHandler;
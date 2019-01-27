#pragma once

struct HerbAbnormalData : AbnormalData
{
	time_t herbExpireTime;
};


class CHerbAbnormalMgr
{
	int count;
	HerbAbnormalData data[8];
	CRITICAL_SECTION _lock;
public:
	CHerbAbnormalMgr();
	~CHerbAbnormalMgr();
	bool AddAbnormal(AbnormalData abnormal);
	void UpdateExpireTime(int abnormalType);
	void Clear(unsigned int idx);
	bool RestoreAbnormal(CCreature *pCreature, int AbnormalType);
	void TimerExpired(CCreature *pCreature);
};
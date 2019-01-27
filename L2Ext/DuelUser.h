#pragma once


class CDuelUser
{
	UINT userServerId;
	User *pUserO;
	double Stat[3]; //HP,CP,MP
	AbnormalDataVector vBuff;
	int AbnormalTable[40][2];
	FVector Position;
	bool Blocked;
	int Team;
	friend class CDuel;
public:
	enum STAT
	{
		HP = 0,
		CP = 1,
		MP = 2
	};
	CDuelUser(User *pUser, int nTeam);
	CDuelUser() : userServerId(0), Team(0), pUserO(NULL), Blocked(false)
	{
		Stat[HP]=0; Stat[CP]=0; Stat[MP]=0;
		memset(AbnormalTable, 0, sizeof(AbnormalTable));
	}
	User *GetUser()
	{
		return pUserO;
	}
	bool IsBlocked()
	{
		return Blocked;
	}
	void SetBlocked(bool bFlag)
	{
		Blocked = bFlag;
	}
	void Block();
	void RestoreData();
	void Send(PCHAR Buff, int Size);
	void StartFight();
	void DoSocialAction(SocialActionType Action);
	void DoEndJob();
};
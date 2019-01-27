#pragma once
 

class CUserStatData
{
public:
	int CharID;
	int PledgeID;
	int KillCount;
	int DeathCount;
	double DamageTaken;
	double DamageDealt;
	CUserStatData(): CharID(0), PledgeID(0), KillCount(0), DeathCount(0), DamageTaken(0), DamageDealt(0) {}
};
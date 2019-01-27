#pragma once

class CSkillMod
{
public:
	CSkillMod();
	~CSkillMod();
	void Clear();

	int nAvoidMeleeSkill;
	int nCancelAttackerTarget;
	int nReflectMeleeSkillDmg;
	int nSelfResurrection;
	int nEnlargeBuffSlots;
	int nDecreaseLostExp;
	int nBlockIconAction;
	int nImproveHeal;
	int nAntiBuff;
	double fDiggerMultipler;
	double fSongDanceMpMultipler;
	double fSongDanceReuseMultipler;
	BOOL semiInvisible;
	double bonusExp;
	double bonusSp;
	double bonusItemDrop;
	double bonusItemDropOnlyChance;
	double bonusAdena;
	double bonusSpoil;
	double bonusHarvesting;
	double bonusRb;
	double bonusRbOnlyChance;
	double bonusEpicRb;
	double resistStun;
	double regenCpMul;
	double regenCpAdd;
};

class CTargetGroundLoc
{
public:
	int nX;
	int nY;
	int nZ;
	CTargetGroundLoc() : nX(0), nY(0), nZ(0) {};
	void Set(int x, int y, int z){nX=x;nY=y;nZ=z;};
	void Clear(){nX=0;nY=0;nZ=0;};
	bool IsSet()
	{
		if(nX==0 && nY==0 && nZ==0)
			return false;
		else
			return true;
	};
};

class CSummonBetrayData
{
public:
	CSummonBetrayData() : ActiveTimer(false), Betraying(false) {};
	bool ActiveTimer;
	bool Betraying;
};

class CSummonFriend
{
public:
	UINT nSummonerID;
	int nX;
	int nY;
	int nZ;
	CSummonFriend() : nSummonerID(0), nX(0), nY(0), nZ(0) {};
	void Set(int nObjectID, int x, int y, int z)
	{
		nSummonerID = nObjectID;
		nX = x;
		nY = y;
		nZ = z;
	}
	void Clear()
	{
		nSummonerID = 0;
		nX = 0;
		nY = 0;
		nZ = 0;
	}
};

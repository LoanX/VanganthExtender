#pragma once

enum RESIDENCE
{
	NOT_RESIDENCE = 0,
	GLUDIO = 1,
	DION = 2,
	GIRAN = 3,
	OREN = 4,
	ADEN = 5,
	INNADRIL = 6,
	GODDARD = 7,
	RUNE = 8,
	SCHUTTGARD = 9,
	FORTRESS_OF_DEAD = 20,
	DEVASTATED_CASTLE = 34,
	CLAN_HALL = 999
};

class CResidenceData
{
public:
	/* 0000 */ int _n0000;
	/* 0004 */ int _n0004;
	/* 0008 */ int _n0008;
	/* 000C */ int _n000C;
	/* 0010 */ int _n0010;
	/* 0014 */ int _n0014;
	/* 0018 */ int _n0018;
	/* 001C */ int _n001C;
	/* 0020 */ int _n0020;
	/* 0024 */ int _n0024;
	/* 0028 */ int _n0028;
	/* 002C */ int _n002C;
	/* 0030 */ int _n0030;
	/* 0034 */ int _unk0034;		
	/* 0038 */ int _unk0038;
	/* 003C */ int _unk003C;
	/* 0040 */ int _unk0040;
	/* 0044 */ int _unk0044;
	/* 0048 */ int _unk0048;
	/* 004C */ int _unk004C;
	/* 0050 */ int nResidenceID;
	/* 0054 */ int nPledgeID;
	/* 0058 */ int _unk0058;
	/* 005C */ int _unk005C;
	/* 0060 */ __time32_t SiegeDate;
	/* 0064 */ int _unk0064;
	/* 0068 */ int nState;
};

class CResidence : public MemoryObject
{
public:
	/*018*/	CResidenceData *pRD;
	/*020*/	bool bPaused;
	/*021*/ bool bSiegeContinued;
	/*022*/ bool bCastleLordReplaced;
	/*023*/ bool bunkn23;
	/*024*/ __time32_t siegePeriod;
	/*028*/ __time32_t lostTime;

	bool IsCastle();
	bool IsAgit();
	bool IsSiegeTime();
	static RESIDENCE CastType(int Residence);
	CPledge *GetPledge();
	bool IsOffendingPledge(int id);
	bool IsDefendingPledge(int id);
	void SetPledge(CPledge *pPledge);
	void ChangeResidenceState(UINT state);
};
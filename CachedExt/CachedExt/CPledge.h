#pragma once

#pragma pack( push, 1 )

class CMemberStatInfo
{
public:
	int CharID;
	int KillCount;
	int DeathCount;
	int DamageDealt;
	int DamageTaken;
	CMemberStatInfo() : CharID(0), KillCount(0), DeathCount(0), DamageDealt(0), DamageTaken(0) {};
};

class CPledge
{
public:
	/* 000 */ int n000;
	/* 004 */ int n004;
	/* 008 */ int n008;
	/* 00C */ int n00C;
	/* 010 */ int n010;
	/* 014 */ int nPledgeID;
	/* 018 */ WCHAR wszClanName[24];
	/* 048 */ int nMasterID;
	/* 04C */ WCHAR wszMasterName[24];
	/* 07C */ int nVar5;
	/* 080 */ int challenge_time;
	/* 084 */ int now_war_id;
	/* 088 */ int n088;
	/* 08C */ int oust_time;
	/* 090 */ int skill_level;
	/* 094 */ int private_flag;
	/* 098 */ int status;
	/* 09C */ int rank;
	/* 0A0 */ int castle_id;
	/* 0A4 */ int agit_id;
	/* 0A8 */ int root_name_value;
	/* 0AC */ int crest_id;
	/* 0B0 */ int emblem_id;
	/* 0B4 */ int is_guilty;
	/* 0B8 */ int dismiss_reserved_time;
	/* 0BC */ int alliance_ousted_time;
	/* 0C0 */ int alliance_withdraw_time;
	/* 0C4 */ int alliance_dismiss_time;
	/* 0C8 */ int nVar6;
	/* 0CC */ int n0CC;
	/* 0D0 */ int n0D0;
	/* 0D4 */ int n0D4;
	/* 0D8 */ int n0D8;
	/* 0DC */ int n0DC;
	/* 0E0 */ int n0E0;
	/* 0E4 */ int n0E4;
	/* 0E8 */ int n0E8;
	/* 0EC */ int n0EC;
	/* 0F0 */ int n0F0;
	/* 0F4 */ int n0F4;
	/* 0F8 */ int n0F8;
	/* 0FC */ int n0FC;
	/* 100 */ __int8 unk100[0x48];
	/* 148 */
};
#pragma pack( pop )

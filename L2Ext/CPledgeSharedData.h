#pragma once

#include "PledgeEx.h"
#pragma pack( push, 1 )

struct MemberInfo
{
	int nDBID;
	int nObjectID;
};

// size: Original 0x268 bytes

class CPledgeSharedData
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
	/* 0050 */ int _unk0050;
	/* 0054 */ WORD _unk0054;
	/* 0056 */ WORD _unk0056;
	/* 0058 */ int _unk0058;
	/* 005C */ int _unk005C;
	/* 0060 */ int _unk0060;
	/* 0064 */ int nDBID;
	/* 0068 */ WCHAR wszClanName[24];
	/* 0098 */ UINT nLeaderDBID;
	/* 009C */ WCHAR wszClanLeaderName[25];
	/* 00CE */ WORD _unk00CE;
	/* 00D0 */ int _unk00D0;
	/* 00D4 */ int _unk00D4;
	/* 00D8 */ int _unk00D8;
	/* 00DC */ int nActiveLevel;
	/* 00E0 */ int _n00E0;
	/* 00E4 */ int nLevel;
	/* 00E8 */ int nCrestID;
	/* 00EC */ int _n00EC;
	/* 00F0 */ int _n00F0;
	/* 00F4 */ int _n00F4;
	/* 00F8 */ int nState; //Peace/War/Dissolving - 3
	/* 00FC */ int nIsGuilty;
	/* 0100 */ int _unk0100;
	/* 0104 */ int nCastleID;
	/* 0108 */ int nAgitID;
	/* 010C */ int _unk010C;
	/* 0110 */ int _unk0110;
	/* 0114 */ int _unk0114;
	/* 0118 */ int nMemberCount;
	/* 011C */ int _unk011C;
	/* 0120 */ int _unk0120;
	/* 0124 */ MemberInfo MemberData[160];
//	/* 0264 */ int _nEndofSharedData; //0x0FFFFFFF
	/* 0624 */ int nReputation;
	/* 0628 */ INT64 PowerData[10];
	/* 0678 */ CPledgeEx *pPledgeEx;
	/* 0680 */ LPVOID pNotUsed680;
	/* 0688 */ int nFortressID;
	/* 068C */ WCHAR wsSubMaster100[25];
	/* 06BE */ WCHAR wsSubMaster200[25];
	/* 06F0 */ WCHAR wsSubMaster1001[25];
	/* 0722 */ WCHAR wsSubMaster1002[25];
	/* 0754 */ WCHAR wsSubMaster2001[25];
	/* 0786 */ WCHAR wsSubMaster2002[25];
	/* 07B8 */ int nHasAcademy;
	/* 07BC */ int nHasSubUnit100;
	/* 07C0 */ int nHasSubUnit200;
	/* 07C4 */ int nHasSubUnit1001;
	/* 07C8 */ int nHasSubUnit1002;
	/* 07CC */ int nHasSubUnit2001;
	/* 07D0 */ int nHasSubUnit2002;
	/* 07D4 */ int nDelegateCLID;
	/* 07D8 */ int nTotalCount;
};
#pragma pack( pop )

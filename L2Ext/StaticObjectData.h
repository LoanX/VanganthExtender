#pragma once

#pragma pack( push, 1 )

//size A8h
struct StaticObjectData
{
	/* 000 */ FVector pos;
	/* 018 */ DWORD _unkn018;
	/* 01C */ DWORD _unkn01C;
	/* 020 */ DWORD _unkn020;
	/* 024 */ DWORD _unkn024;
	/* 028 */ DWORD _unkn028;
	/* 02C */ DWORD _unkn02C;
	/* 030 */ DWORD _unkn030;
	/* 034 */ DWORD _unkn034;
	/* 038 */ DWORD _unkn038;
	/* 03C */ DWORD _unkn03C;
	/* 040 */ DWORD _unkn040;
	/* 044 */ DWORD _unkn044;
	/* 048 */ DWORD _unkn048;
	/* 04C */ DWORD _unkn04C;
	/* 050 */ DWORD _unkn050;
	/* 054 */ DWORD _unkn054;
	/* 058 */ DWORD _unkn058;
	/* 05C */ DWORD _unkn05C;
	/* 060 */ DWORD _unkn060;
	/* 064 */ BYTE level;
	/* 065 */ bool breakable;
	/* 066 */ bool _unkn66;
	/* 067 */ bool _unkn67;
	/* 068 */ double hp;
	/* 070 */ double maxHp;
	/* 078 */ DWORD _unkn078;
	/* 07C */ DWORD _unkn07C;
	/* 080 */ DWORD _unkn080;
	/* 084 */ DWORD _unkn084;
	/* 088 */ DWORD _unkn088;
	/* 08C */ DWORD _unkn08C;
	/* 090 */ DWORD _unkn090;
	/* 094 */ DWORD _unkn094;
	/* 098 */ DWORD isOpen;		/* default status */
	/* 09C */ DWORD id;	/* staticObjectId */
	/* 0A0 */ DWORD _unkn0A0;
	/* 0A4 */ DWORD _unkn0A4;
};

#pragma pack( pop )
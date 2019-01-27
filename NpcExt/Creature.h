#pragma once
#include "Object.h"

//vt 683C98h
class CCreature : public CObject
{
public:
	/* 38 */ UINT _unkn38;
	/* 3C */ UINT _unkn3C;
	/* 40 */ UINT _unkn40;
	/* 44 */ UINT _unkn44;
	/* 48 */ UINT _unkn48;
	/* 4C */ UINT _unkn4C;
	/* 50 */ CSharedCreatureData *pSD;
	/* 58 */ LPVOID _unkn58;
	/* 60 */ LPVOID _unkn60;
	/* 68 */ LPVOID _unkn68;	//some struct starts here
	/* 70 */ LPVOID _unkn70;
	/* 78 */ LPVOID _unkn78;
	/* 80 */ LPVOID _unkn80;
	/* 88 */ LPVOID _unkn88;
	/* 90 */ LPVOID _unkn90;
	/* 98 */ LPVOID _unkn98;
	/* A0 */ LPVOID _unknA0;
	/* A8 */ LPVOID _unknA8;
	/* B0 */ LPVOID _unknB0;
	/* B8 */ RWLock lock;
	/* F0 */ LPVOID _unknF0;
	/* F8 */ LPVOID _unknF8;
	/* 100 */ LPVOID _unkn100;
	/* 108 */ BYTE _unkn108;
	/* 109 */ BYTE _unkn109;
	/* 10A */ BYTE _unkn10A;
	/* 10B */ BYTE _unkn10B;
	/* 10C */ UINT _unkn10C;
};
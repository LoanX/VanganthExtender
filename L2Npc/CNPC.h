#pragma once
#include "Utils.h"

class CreatureSharedData;

class CNPC
{
	/* 000 */ LPVOID p000;
	/* 008 */ LPVOID p008;
	/* 010 */ LPVOID p010;
	/* 018 */ LPVOID p018;
	/* 020 */ LPVOID p020;
	/* 028 */ LPVOID p028;
	/* 030 */ LPVOID p030;
	/* 038 */ LPVOID p038;
	/* 040 */ LPVOID p040;
	/* 048 */ LPVOID p048;
	/* 050 */ CreatureSharedData *pSD;
public:
	static void OverrideFunctions();
	static int GetTimeHour();
	static CNPC* GetObject(int nObjectID);
	static bool IsNewbie2(CNPC* pNPC, CreatureSharedData* pSD);
	static void AddUseSkillDesire(CNPC* pNPC, CreatureSharedData* pSD, int SkillNameID, int SkillType, int ActionMoveType, float value);
};
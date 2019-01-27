#pragma once
#pragma warning(disable: 4996)

#include <windows.h>
#include <time.h>
#include "math.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#define JMP 0xE9
#define CALL 0xE8

#define MAKELONGLONG(low, high)      ((INT64)(((LONG)((DWORD_PTR)(low) & 0xffffffff)) | ((INT64)((LONG)((DWORD_PTR)(high) & 0xffffffff))) << 32))
#define LOLONG(l)           ((LONG)((DWORD_PTR)(l) & 0xffffffff))
#define HILONG(l)           ((LONG)((DWORD_PTR)(l) >> 32))

using namespace std;

class CCreature;
class CObject;
class CNpcMarker;
class CSkillInfo;
class User;

struct AbnormalData
{
	CSkillInfo *pSI;
	int castTime;
	int selfBuff; //it contains info if it's dance/song (256) + if it's selfcasted (1/0)
	int casterSID;
	int partyBuff;	// for party buffs it's 9
};

struct FVector
{
	double x;
	double y;
	double z;
};

struct IVector
{
	int x;
	int y;
	int z;

	IVector() : x(0), y(0), z(0) {}
	IVector(int X, int Y, int Z): x(X), y(Y), z(Z)	{}
	IVector(double X, double Y, double Z): x(X), y(Y), z(Z) {}
	IVector(FVector pos) : x(pos.x), y(pos.y), z(pos.z) {}
};


class CSkillKey
{
public:
	WORD skill_id;
	BYTE  level;
	CSkillKey(WORD id = 0, BYTE lvl = 1)
		: skill_id(id), level(lvl) {}
	bool operator< (const CSkillKey& other) const
	{
		if (skill_id < other.skill_id)
			return true;
		if (skill_id > other.skill_id)
			return false;
		return level < other.level;
	}
};

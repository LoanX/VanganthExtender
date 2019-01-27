#pragma once
#pragma pack( push, 1 )

struct RWLock
{
	/* 0000 */  CRITICAL_SECTION cs;
	/* 0028 */  HANDLE      hSemaphore;
	/* 0030 */  INT32       _unk0030;
	/* 0034 */  INT32       _pad0034;
};

union HookMem
{
	struct
	{
		BYTE p[5];
	}byte;
	struct
	{
		BYTE opCode;
		DWORD addr;
	}full;
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
	IVector(double X, double Y, double Z): x((int)X), y((int)Y), z((int)Z) {}
	IVector(FVector pos) : x((int)pos.x), y((int)pos.y), z((int)pos.z) {}
};

struct FAngle
{
	int heading;
};

union Augmentation
{
	struct 
	{
		WORD effectA;
		WORD effectB;
	}Effect;
	DWORD Full;
	Augmentation() : Full(0) {};
};

union SkillPchId
{
	UINT pchId;
	struct 
	{
		BYTE level;
		WORD skillId;
	};
	SkillPchId() : pchId(0) {};
	SkillPchId(UINT id) : pchId(id) {};
	SkillPchId(int id, BYTE lvl) : skillId(id), level(lvl) {};
	bool operator == (const SkillPchId& other) const
	{
		if(pchId == other.pchId)
			return true;
		return false;
	}
	bool operator != (const SkillPchId& other) const
	{
		if(pchId != other.pchId)
			return true;
		return false;
	}
	bool operator < (const SkillPchId& other) const
	{
		if(pchId < other.pchId)
			return true;
		return false;
	}
};

union IP
{
	UINT full;
	BYTE part[4];
};

#pragma pack( pop )
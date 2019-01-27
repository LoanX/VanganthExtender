#pragma once
#pragma pack( push, 1 )

union HWID
{
	BYTE hash[32];
	UINT64 part[4];

	bool operator == (const HWID& other) const
	{
		if(part[0] == other.part[0] && part[1] == other.part[1] && part[2] == other.part[2] && part[3] == other.part[3])
		{
			return true;
		}
		return false;
	}
	bool operator != (const HWID& other) const
	{
		if(part[0] != other.part[0] || part[1] == other.part[1] || part[2] == other.part[2] || part[3] == other.part[3])
		{
			return true;
		}
		return false;
	}
	bool operator < (const HWID& other) const
	{
		if(part[0] < other.part[0])
			return true;
		return false;
	}

	inline bool IsNull()
	{
		if(part[0] == 0 && part[1] == 0 && part[2] == 0 && part[3] == 0)
		{
			return true;
		}
		return false;
	}
	inline bool IsEqual(HWID& other)
	{
		if(part[0] == other.part[0] && part[1] == other.part[1] && part[2] == other.part[2] && part[3] == other.part[3])
		{
			return true;
		}
		return false;
	}
	HWID() { clear(); };
	HWID(LPBYTE lpHwid) { memcpy(hash, lpHwid, 32); };
	HWID(const unsigned char* lpHwid) { memcpy(hash, lpHwid, 32); };
	HWID(wstring value)
	{
		if(value.size() == 32)
		{
			for(UINT n=0;n<32;n++)
			{
				hash[n] = static_cast<BYTE>(value[n]);
			}
		}else
		{
			clear();
		}
	}
	void clear() { part[0] = part[1] = part[2] = part[3] = 0; };
	wstring ToString()
	{
		wstring str;
		for(UINT n=0;n<32;n++)
		{
			str.push_back((WCHAR)hash[n]);
		}
		return str;
	}

};


union UtB
{
	UINT full;
	BYTE part[4];
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
	union
	{
		double v[3];
		struct
		{
			double x, y, z;
		};
	};
	FVector() : x(0), y(0), z(0) {}
	FVector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	FVector(const FVector& other) : x(other.x), y(other.y), z(other.z) {}
	inline void set(double i, double j, double k) { x=i; y=j; z=k; }
	// Operator Overloads
	inline bool operator == (const FVector& other)
	{
		if(x == other.x && y == other.y && z == other.z)
		{
			return true;
		}
		return false;
	}
	inline FVector& operator *= (double Right)
	{
		x *= Right;
		y *= Right;
		z *= Right;
		return *this;
	}

	inline FVector& operator /= (double Right)
	{
		x /= Right;
		y /= Right;
		z /= Right;
		return *this;
	}

	inline FVector& operator *= (UINT Right)
	{
		x *= Right;
		y *= Right;
		z *= Right;
		return *this;
	}

	inline FVector& operator /= (UINT Right)
	{
		x /= Right;
		y /= Right;
		z /= Right;
		return *this;
	}

	inline FVector& operator += (const FVector &Right)
	{
		x += Right.x;
		y += Right.y;
		z += Right.z;
		return *this;
	}

	inline FVector& operator -= (const FVector &Right)
	{
		x -= Right.x;
		y -= Right.y;
		z -= Right.z;
		return *this;
	}

	inline FVector operator + (double Right) const
	{
		return FVector(x + Right,
			y + Right,
			z + Right);
	}
	inline FVector operator * (double Right) const
	{
		return FVector(x * Right,
			y * Right,
			z * Right);
	}

	inline FVector operator / (double Right) const
	{
		return FVector(x / Right,
			y / Right,
			z / Right);
	}

	inline FVector operator + (const FVector &Right) const
	{
		return FVector(x + Right.x,
			y + Right.y,
			z + Right.z);
	}

	inline FVector operator - (const FVector &Right) const
	{
		return FVector(x - Right.x,
			y - Right.y,
			z - Right.z);
	}

	inline FVector operator - (const FVector &V)
	{
		return FVector(-V.x, -V.y, -V.z);
	}

	inline double Distance(const FVector &other)
	{
		const double XDiff = other.x - x;
		const double YDiff = other.y - y;
		const double ZDiff = other.z - z;
		return sqrt(XDiff * XDiff + YDiff * YDiff + ZDiff * ZDiff);
	}

	inline double Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	inline FVector Cross( const FVector &other) const
	{
		//
		FVector r;
		r.x = y*other.z - z*other.y;
		r.y = z*other.x - x*other.z;
		r.z = x*other.y - y*other.x;
		return r;
	}

	inline FVector Normalize() const
	{
		double Len = Length();
		if(Len == 0.0f)
		{
			return FVector(x, y, z);
		}
		else
		{
			double Factor = 1.0f / Len;
			return FVector(x * Factor, y * Factor, z * Factor);
		}
	}
	inline FVector Join(const FVector& other) const
	{
		FVector r;
		r.x = other.x - x;
		r.y = other.y - y;
		r.z = other.z - z;
		return r;
	}
	inline double Dot(const FVector &Right) const
	{
		return (x * Right.x + y * Right.y + z * Right.z);
	}
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
	INT32 Distance(const IVector& other)
	{
		return static_cast<INT32>( sqrt( pow( (double)(other.x-x), 2) + pow( (double)(other.y-y), 2) + pow( (double)(other.z-z), 2) ) );		
	}
};

struct FAngle
{
	int heading;
};

//18h
struct AbnormalData
{
	/* 00 */ CSkillInfo *pSI;
	/* 08 */ DWORD expireTime;	//buffExpireTime
	/* 0C */ int selfBuff; //it contains info if it's dance/song (256) + if it's selfcasted (1/0)
	/* 10 */ int casterSID;
	/* 14 */ int partyBuff;	// for party buffs it's 9
	AbnormalData() : pSI(0), expireTime(0), selfBuff(0), casterSID(0), partyBuff(0) {};
};

typedef vector <AbnormalData> AbnormalDataVector;

union Augmentation
{
	struct 
	{
		USHORT effectA;
		USHORT effectB;
	}Part;
	UINT Full;
	Augmentation() : Full(0) {};
	Augmentation(UINT augmentation) : Full(augmentation) {};
	Augmentation(USHORT first, USHORT second) { Part.effectA = first; Part.effectB = second; };
};

struct SkillPchIdS
{
	BYTE level;
	WORD skillId;
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
	SkillPchId(int id, BYTE lvl) { pchId = 0; skillId = id; level = lvl; };
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

struct TerritoryData
{
	int territoryId[TERRITORY_COUNT];
	TerritoryData()
	{
		clear();
	}
	void clear()
	{
		for(int n=0;n<TERRITORY_COUNT; n++)
		{
			territoryId[n] = 0;
		}
	}
};

struct ItemReq
{
	int itemId;
	int amount;
	ItemReq() : itemId(0), amount(0) {};
};

struct PledgeSkillInfo
{
	int skillId;
	int skillLevel;
	int requiredPledgeLevel;
	int reputationConsume;
	vector<ItemReq> itemConsume;
	UINT requiredPledgeClass;
};

#pragma pack( pop )
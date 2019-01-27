#pragma once

class  SkillEnchantInfo
{
public:

	/* 0x00 */		int     skill_id;               // case 2
	/* 0x04 */		int     new_enchant_skill_level;        // case 5
	/* 0x08 */		int     org_skill_level;        // case 2
	/* 0x0Ch */		int     enchant_skill_level;    // case 1
	/* 0x10h */		int     route_id;               // case 3 
	/* 0x14h */		int     enchant_id;             // case 4
	/* 0x18h */		int     exp;                    // case 6
	/* 0x1Ch */		int     sp;                             // case 7
	///* 0x20h */		int     dw20;                   // case 8  = 0BAADF00Dh
	/* 0x20h */		std::vector<int> item_need;  
	///* 0x30h */		__int64 i30;                    // = 01C90DC10h => int_1C90DC10 dd 0ABABABABh
	///* 0x38h */		__int64 i38;                    // = 01C90DC10h => int_1C90DC10 dd 0ABABABABh
	///* 0x40h */		__int64 i40;					// case 8
	/* 0x40h */		std::vector<int> item_num_needed;
	///* 0x50h */		__int64 i50;
	///* 0x58h */		__int64 i58;
	/* 0x60h */		int     probArr[5];
};

class CSkillEnchantDB
{
	public:

		// CSkillEnchantDB map doesn't use CSkillKey (fortunatelly),
		// but rather a pair of ints to store skill id and level
		typedef std::pair<int, int> EnchKey;

		struct EnchKeyLess
		{
			bool operator() (const EnchKey& left, const EnchKey& right) const
			{
				if (left.first < right.first) return true;
				if (left.first > right.first) return false;
				return left.second < right.second;
			}
		};

		typedef const std::map<EnchKey, SkillEnchantInfo*, EnchKeyLess> SkillEnchantMap;
		typedef SkillEnchantMap::const_iterator SkillEnchantIterator;

		// Calling Constructor
		CSkillEnchantDB(){};
		
		// Calling Destructor
		~CSkillEnchantDB(){};
	
		/* 0x00  */ void * p00;
		/* 0x08  */ void * p08;
		/* 0x10  */ void * p10;
		/* 0x18  */ void * p18;
		/* 0x20 */	SkillEnchantMap m_SkillEnchantInfo;
		/* 0x28 */	list<int> list_28;
		/* 0x38 */	list<int> list_38;

		static CSkillEnchantDB* Static();

		SkillEnchantInfo* GetSkillEnchantInfo(int nSkillId, int nLevel);
		int GetProbIndex(int nUserLevel);
		bool IsLearnableLevel(int nUserLevel);
};

extern CSkillEnchantDB g_SkillEnchantDB;
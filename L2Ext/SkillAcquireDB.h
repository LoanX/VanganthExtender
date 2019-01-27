#pragma once

struct RequiredItemList
{
	int count;
	int data[10][2];	/* 0-9 index  sub-index 0-1 itemId/count*/
	void Init()
	{
		count = 0;
		for(int n=0;n<10;n++)
		{
			data[n][0] = 0;
			data[n][1] = 0;
		}
	}
	void AddItem(int itemId, int itemCount)
	{
		if(count < 10)
		{
			data[count][0] = itemId;
			data[count][1] = itemCount;
			count++;
		}
	}
};

struct AcquireSkillInfo
{
	wstring name;
	int skillId;
	int level;
	int requiredSp;
	int requiredLevel;
	bool autoGet;
	RequiredItemList requiredItems;
};

class CAcquirableSkills
{
	map<CSkillKey, AcquireSkillInfo> mData;
public:
	void Add(AcquireSkillInfo data);
	void Include(CAcquirableSkills& other);
	bool CanAcquire(int level, int skillId, int skillLevel);
	map<int, int> GetList(int level);
};

class CSkillAcquireDB
{
protected:
	CRITICAL_SECTION _lock;
private:
	map<int, CAcquirableSkills> mData; /* <class_id, available_skills> */
	map<int, int> mSkillLevel;
	map<int, int> mNotEnchantedSkillLevel;
	list<int> lSkillId;
	inline void Lock() { EnterCriticalSection(&_lock); };
	inline void Unlock() { LeaveCriticalSection(&_lock); };
	int GetClassIdFromName(const wchar_t* wName);
public:
	CSkillAcquireDB();
	~CSkillAcquireDB();
	void Init();
	void Load();
	bool IsInDatabase(int skillId);
	int GetMaxSkillLevel(int skillId);
	int GetMaxNotEnchantedSkillLevel(int skillId);
	int GetMaxAcquirableLevel(int classId, int level, int skillId);
	bool CanAcquireMaxLevel(int classId, int level, int skillId);
	bool CanAcquire(int classId, int level, int skillId, int skillLevel);
	map<int, int> GetAcquirableSkills(int classId, int level);
	void ValidateSkills(User *pUser);
};

extern CSkillAcquireDB g_SkillAcquireDB;
#pragma once

class CNpcDbInfo
{
	UINT m_NpcClassId;
	wstring m_NpcName;
	time_t m_StartTime;
	time_t m_EndTime;
	IVector m_SpawnPos;
	UINT m_Heading;
	UINT m_NpcObjectId;
public:
	CNpcDbInfo() : m_NpcClassId(0), m_StartTime(0), m_EndTime(0), m_Heading(0), m_NpcObjectId(0) {};
	void Set(UINT npcClassId, wstring name, time_t startTime, time_t endTime, int x, int y, int z, UINT heading);
	inline UINT GetClassId() { return m_NpcClassId; };
	inline wstring GetName() { return m_NpcName; };
	inline time_t GetStartTime() { return m_StartTime; };
	inline time_t GetEndTime() { return m_EndTime; };
	inline IVector GetSpawnPos() { return m_SpawnPos; };
	inline UINT GetHeading() { return m_Heading; };
	inline UINT GetObjectId() { return m_NpcObjectId; };
	inline void SetObjectId(UINT value) { m_NpcObjectId = value; };
};

class CDBNpcMaker
{
	CSLock m_Lock;
	bool m_Enabled;
	vector<CNpcDbInfo*> m_Npc;

	void LoadINI();
	void SpawnNpc(CNpcDbInfo* pInfo);
	void DespawnNpc(CNpcDbInfo *pInfo);
public:
	void Init();
	void OnDBLoad(UINT npcClassId, wstring name, time_t startTime, time_t endTime, int x, int y, int z, UINT heading);
	void TimerExpired();
};

extern CDBNpcMaker g_DBNpcMaker;
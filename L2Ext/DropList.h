#pragma once

namespace NpcData
{
	struct ItemDropInfo
	{
		INT32 itemId;
		INT32 minCount;
		INT32 maxCount;
		double chance;
	};

	struct GroupDropInfo
	{
		vector<ItemDropInfo> items;
		double chance;
		double itemsChanceSum;
		BOOL adenaGroup;
	};

	struct NpcDropInfo
	{
		INT32 classId;	// npcId + 1000000
		vector<ItemDropInfo> itemMakeList;
		vector<ItemDropInfo> additionalMakeList;
		vector<GroupDropInfo> additionalMakeMultiList;
	};

	class CDropList
	{
		BOOL m_enabled;
		BOOL m_debugMsg;
		double m_raidBossItemRate;
		map<INT32, NpcDropInfo*> m_data;
		void Release();
		void ParseNpcData(wstring line);
		bool ParseItemMakeList(vector<ItemDropInfo>& itemMakeList, wstring data);
		bool ParseAdditionalMakeList(vector<ItemDropInfo>& additionalMakeList, wstring data);
		bool ParseAdditionalMakeMultiList(vector<GroupDropInfo>& additionalMakeMultiList, wstring data);
		void LoadData();
	public:
		CDropList();
		void Init();
		~CDropList();
		inline BOOL IsEnabled() { return m_enabled; };
		void OnDie(CNPC *pNpc, CCreature *pEnemy);
	};
};

extern NpcData::CDropList g_DropList;
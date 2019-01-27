#pragma once


struct NpcInfo
{
	INT32 id;
	INT32 classId;
	wstring name;
	wstring clientName;
	NpcInfo() : id(0), classId(0) {}
};

class CNpcDb
{
	vector<NpcInfo> m_DB;
	void LoadData();
public:
	void Init();
	wstring GetClientNameByName(wstring name);
	wstring GetClientNameByClassId(UINT classId);
};

extern CNpcDb g_NpcDb;
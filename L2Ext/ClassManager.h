#pragma once

class CClassManager
{
	BOOL m_enabled;
	INT32 m_requiredItemId[3];
	INT32 m_requiredItemCount[3];
	BOOL m_changeEnabled[3];
	ItemReq m_ChangeReward[3];
	BOOL m_Nobless;
	INT32 m_NoblessPriceId;
	INT32 m_NoblessPriceAmount;
	BOOL m_SubClass;
	INT32 m_SubClassPriceId;
	INT32 m_SubClassPriceAmount;

	map<INT32, wstring> m_class_name;
	void LoadClassName();
public:
	CClassManager();
	~CClassManager();
	void Init();
	wstring GetName(INT32 classId);
	void GetAvailable(int baseClassId, list<int>& lAvailable);
	bool CanChange(User *pUser, int newClass);
	void RequestShowHtml(User *pUser);
	void RequestChange(User *pUser, int newClass);
	void RequestNobless(User *pUser);
	void RequestSubClass(User *pUser);
};

extern CClassManager g_ClassManager;
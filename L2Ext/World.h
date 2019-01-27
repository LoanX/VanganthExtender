#pragma once

struct DeleteItemStruct
{
	INT64 addr;
	CItem *pItem;
};

class CWorld
{
	CWorld* pInstance;
	list<CItem*> m_ItemsToDelete;
	CRITICAL_SECTION m_lock;
public:
	CWorld();
	~CWorld();
	void AddToDelete(CItem *pItem);
	void DeleteWaitingItem(CItem *pItem);
	bool InRange(FVector org, FVector dest, int range)
	{
		typedef bool (*f)(CWorld*, FVector, FVector, int);
		return f(0x8AE6C0L)(pInstance, org, dest, range);
	}
	bool GodsVoice(PWCHAR wStr)
	{
		typedef bool (*f)(CWorld*, PWCHAR);
		return f(0x8AF830L)(pInstance, wStr);
	}
	bool PutItem(CItem *pNewItem, FVector vPos, CItem *pTemp = NULL)
	{
		typedef bool (*f)(CWorld*, CItem*, FVector, CItem*);
		return f(0x8B70A0L)(pInstance, pNewItem, vPos, pTemp);
	}
	bool PickItem(CItem *pItem, CCreature *pCreature)
	{
		typedef bool (*f)(CWorld*, CItem*, CCreature*);
		return f(0x8BEAC0L)(pInstance, pItem, pCreature);
	}
	double GetHeight(DWORD Race, DWORD Sex, DWORD Magic);
	double GetRadius(DWORD Race, DWORD Sex, DWORD Magic);
	void DeleteItemFromWorld(CItem *pItem, bool broadcast = false);
};

extern CWorld g_World;
#pragma once


struct RWLock
{
	/* 0000 */  CRITICAL_SECTION cs;
	/* 0028 */  HANDLE      hSemaphore;
	/* 0030 */  INT32       _unk0030;
	/* 0034 */  INT32       _pad0034;
	void ReadLock(const WCHAR* file = 0, int line = 0)
	{
		typedef void(*f)(RWLock*, const WCHAR*, int);
		f(0x781880L)(this, file, line);
	}
	void ReadUnlock(const WCHAR* file = 0, int line = 0)
	{
		typedef void(*f)(RWLock*, const WCHAR*, int);
		f(0x781CF0L)(this, file, line);
	}
};

class CInventory
{
public:
	enum InventoryUpdateType
	{
		ADD = 1,
		UPDATE = 2,
		DEL = 3
	};
/*000*/	int nCreatureIndex;
/*004*/	int nUnkn;
/*008*/ CSharedRWLock *pLock;
/*010*/ int nCurrentWeight;
/*014*/ int nUnkn014;
/*018*/ LPVOID pUnkn018; //perhaps item map
/*020*/ int nUnkn20;
/*024*/ int nUnkn24;
/*028*/ int nUnkn28;
/*030*/ RWLock rwlock;
public:
	CItem *GetFirstItemByClassID(int nItemID);
	CItem *GetNextItemByClassID(int nBeforeIndex);
	CItem *GetItemByIndex(int nIndex);
	CItem *GetItemBySID(UINT nSID);
	int GetAdenaAmount();
	bool HaveItemByClassID(int nItemID, int nAmount);
	CItem *GetItemByDBID(int nDBID);
	bool SetInventoryChanged(CItem *pItem, int nChangedType);
	int GetTotalCount();
	CCreature *GetOwner();
	INT32 GetFirstItemIndex();
	bool Pop(int nItemIndex, bool bFlag);
	void WriteLock(PWCHAR File, int nLine);
	void WriteUnlock();
	void ReadLock(const WCHAR* file = __FILEW__, int line = __LINE__);
	void ReadUnlock();
	bool CheckAddable(int nClassID, int nAmount);
};
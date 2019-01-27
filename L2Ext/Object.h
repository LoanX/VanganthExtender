#pragma once 
#include "IOObject.h"

class CSmartID
{
public:
	unsigned int object_sid;
};

class CObjName
{
public:
	LPVOID pPointer;
	WCHAR sName[25];
	CObjName(PWCHAR wsName) : pPointer(0)
	{
		wcscpy_s(sName, 25, wsName);
	}
};

class CWorld;

class CObject: public MemoryObject
{
public:
	/* 0018 */ CWorld *pWorld;
	/* 0020 */ UINT nDBID;
	/* 0024 */ UINT nObjectID;
	/* 0028 */ int nObjectType;
	/* 002C */ int unk02C;

	bool IsValid(DWORD dwVTable);	

	User* GetUser()
	{
		if(IsValid(VT_USER))
		{
			return (User*)this;
		}
		return NULL;
	}
	CNPC* CastNPC()
	{
		return (CNPC*)this;
	}

	CSummon* CastSummon()
	{
		return (CSummon*)this;
	}

	CPet* CastPet()
	{
		return (CPet*)this;
	}

	CItem* CastItem()
	{
		return (CItem*)this;
	}

	CCreature* CastCreature()
	{
		return (CCreature*)this;
	}

	CPledge* CastPledge()
	{
		return (CPledge*)this;
	};
	static CObject *CreateObject(int nSize, BYTE bOpt, PWCHAR wszName)
	{
		typedef CObject* (*f)(INT64, int, BYTE, PWCHAR);
		return f(0x670140L)(0x01557E00, nSize, bOpt, wszName);
	}
	static CObject* GetObjectBySID(int nObjectSID)
	{
		typedef CObject*(*f)(UINT);
		return f(0x6F4920L)(nObjectSID);
	};

	CParty* SafeCastParty();
	User* SafeCastUser();
	CSummon* SafeCastSummon();
	CItem* SafeCastItem();
	CCreature* SafeCastCreature();
};

class CObjectSP
{
public:
	CObject* pObject;
	int ObjectType;
	CObjectSP()
	{
		pObject = NULL;
		ObjectType = NULL;
    }
	~CObjectSP()
	{
		if (pObject)
			pObject->Release(__FUNCTION__, __LINE__, ObjectType, false);
		pObject = NULL;
	}
	static CObject* FindObject(CObjectSP *pObjectSP, int nObjectSID)
	{
		typedef CObject* (*_find)(INT64, CObjectSP*, int);
		_find find = (_find) 0x00436660;
		return find(0x0E4120A0, pObjectSP, nObjectSID);
	}
	static User* GetUserByDBID(CObjectSP *pObjectSP, int nDBID)
	{
		typedef User*(__fastcall *_GetU)(INT64, CObjectSP*, int);
		_GetU GetU = (_GetU) 0x00479090;
		return GetU(0x0E4E7160, pObjectSP, nDBID);
	}
	static User* GetUserByName(CObjectSP *pObjectSP, PWCHAR sName)
	{
		typedef User*(__fastcall *_GetU)(INT64, CObjectSP*, CObjName*);
		_GetU GetU = (_GetU) 0x00479E20;
		CObjName ObjName(sName);
		return GetU(0x0E6C70A0, pObjectSP, &ObjName);
	};
	static CItem* GetItemBySID(CObjectSP *pObjectSP, int nItemSID)
	{
		typedef CItem*(__fastcall *_GetI)(INT64, CObjectSP*, int);
		_GetI GetI = (_GetI) 0x004745F0;
		return GetI(0x0E412130, pObjectSP, nItemSID);
	}
	static void GetPledgeByName(CObjectSP *pObjectSP, PWCHAR wsName)
	{
		typedef void (*_gpbn)(CObjectSP*, PWCHAR);
		_gpbn gpbn = (_gpbn) 0x008848F0;
		gpbn(pObjectSP, wsName);
	}
};
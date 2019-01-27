#pragma once


class CWareHouseWithdraw
{
public:
	/* 000 */ INT64 nUnkn000;
	/* 008 */ INT64 nUnkn008;
	/* 010 */ INT64 nUnkn010;
	/* 018 */ int nNpcObjectID;
	/* 01C */ int nNpcClassID; 
	/* 020 */ int nNpcServerIndex;
	/* 024 */ int nUnkn024;
	/* 028 */ int nUnkn028;
	/* 02C */ int nWareHouseType; //3 - freight
	/* 030 */ int nNpcClassID1;
	/* 034 */ int nUserSID;
	/* 038 */ int nNpcSID;
public:
	bool IsValid()
	{
		if(this)
		{
			try
			{
				if((*(LPDWORD)this) == 0xA9C028)
					return true;
			}catch(...)
			{
				return false;
			}
		}
		return false;
	}
	static CWareHouseWithdraw *CreateObject(int nSize = 0x88)
	{
		typedef CWareHouseWithdraw* (*_create)(int);
		_create create = (_create) 0x00894160;
		return create(nSize);
	};
	int GetClassId(int nItemDBID)
	{
		typedef int (*_gcid)(CWareHouseWithdraw*, int);
		_gcid gcid = (_gcid) 0x008AC8B0;
		return gcid(this, nItemDBID);
	};
	CWareHouseWithdraw *Constructor(User *pUser, CNPC *pNPC, int nWareHouseType, int nNpcClassID)
	{
		typedef CWareHouseWithdraw*(*_const)(CWareHouseWithdraw*, User*, CNPC*, int, int);
		_const constr = (_const) 0x008ACEF0;
		return constr(this, pUser, pNPC, nWareHouseType, nNpcClassID);
	};
	void SetNpcData(CNPC *pNPC)
	{
		typedef void (*_snpc)(CWareHouseWithdraw*, CNPC*);
		_snpc snpc = (_snpc) 0x006CD2D0;
		snpc(this, pNPC);
	};
};
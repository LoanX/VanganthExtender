#pragma once


class CWareHouseDeposit
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
				if(*(LPDWORD)this == 0xA9B3B8)
					return true;
			}catch(...)
			{
				return false;
			}
		}
		return false;
	}
	bool IsInRange()
	{
		typedef bool(*f)(CWareHouseDeposit*);
		return f(0x8A8870L)(this);
	}
};
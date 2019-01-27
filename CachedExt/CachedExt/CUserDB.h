#pragma once
#include "class.h"

class CUserDB
{
	CUserDB* pInstance;
public:
	CUserDB()
	{
		pInstance = (CUserDB*) 0x1E95A58;
	}
	CUserSP GetUser(int nUserID, bool bLoadIfNotExist)
	{
		typedef CUserSP* (__thiscall *_gu)(CUserDB*, CUserSP*, int, bool);
		_gu gu = (_gu) 0x004AFCF0;
		CUserSP tmp;
		gu(pInstance, &tmp, nUserID, bLoadIfNotExist);
		return tmp;
	}
};

extern CUserDB g_UserDB;
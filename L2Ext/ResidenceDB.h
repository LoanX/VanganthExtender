#pragma once
#include "Residence.h"

class CResidenceDB
{
	CResidenceDB *pInstance;
public:
	CResidenceDB()
	{
		pInstance = (CResidenceDB*)0x0E285BC0;
	}
	~CResidenceDB() {};
	CResidence *GetResidence(int nResidenceID)
	{
		typedef CResidence* (*_f)(CResidenceDB*, int);
		_f f = (_f) 0x0077D0E0;
		return f(pInstance, nResidenceID);
	}
	CResidence *GetResidence(PWCHAR wsName)
	{
		typedef CResidence* (*_f)(CResidenceDB*, PWCHAR);
		_f f = (_f) 0x0077CDE0;
		return f(pInstance, wsName);
	}
	CResidenceData* RequireResidenceData(int id)
	{
		typedef CResidenceData* (*f)(CResidenceDB*, int);
		return f(0x779570L)(pInstance, id);
	}

};

extern CResidenceDB g_ResidenceDB;
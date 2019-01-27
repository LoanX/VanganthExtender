#pragma once
#include "Object.h"
#include "GeneralObject.h"
#include "IOCriticalSection.h"

//A8 size
class CContributeData : public CGeneralObject
{
public:
/* 30 */	CIOCriticalSection _lock;
/* 60 */	map<UINT, double> accumulatedContribute;	//map<ObjectId, power>
/* 78 */	LPVOID _unkn078;
/* 80 */	LPVOID _unkn080;
/* 88 */	LPVOID _unkn088;
/* 90 */	LPVOID _unkn090;
/* 98 */	UINT contributorId;
/* 9C */	int contributorLevel;
/* A0 */	UINT contributePower;
/* A4 */	UINT alignA4;

	void Clear()
	{
		typedef void (*f)(CContributeData*);
		f(0x6F52F0L)(this);
	}
};
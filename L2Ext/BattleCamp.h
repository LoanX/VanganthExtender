#pragma once
#include "StaticObject.h"

//0xA8 size
class CBattleCamp : public CStaticObject
{
public:
	/*038*/ LPVOID pUnkn038;
	/*040*/ CPledge* pPledge;
	/*048*/ int nPledgeId;
	/*04C*/ int nUnkn4C;	//align 8
	/*050*/ CResidence* pResidence;
	/*058*/ int nResidenceId;
	/*05C*/ int nUnkn5C;	//align 8
	/*060*/ LPVOID pUnkn060;
	/*068*/ LPVOID pUnkn068;
	/*070*/ int nUnkn70;
	/*074*/ WCHAR wPledgeName[26];
};
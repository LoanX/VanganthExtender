#include "StdAfx.h"

CPartyRoom* CPartyRoom::FindByMember(User *pUser)
{
	struct sTmp
	{
		User *pTmpUser;
		int nWorkType;
	} user_sp;
	user_sp.pTmpUser = pUser;
	user_sp.nWorkType = 167;
	pUser->AddRef(__FILE__, __LINE__, 167);
	CPartyRoomSP party_room_sp;
	typedef void (*f)(INT64, CPartyRoomSP*, sTmp*);
	f(0x73A200L)((*(PINT64)0x0BCEAE40), &party_room_sp, &user_sp);
	return party_room_sp.get();
}
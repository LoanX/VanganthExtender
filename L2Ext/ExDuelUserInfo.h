#pragma once

#include "ServerPacket.h"

class CExDuelUserInfo : public CServerPacket
{
public:
	CExDuelUserInfo(User *pUser)
	{
		Write((BYTE)0xFE);
		Write((WORD)0x4F);
		if(pUser->ValidUser())
		{
			WriteS(pUser->pSD->wszName);
			Write((DWORD)pUser->nObjectID);
			Write((DWORD)pUser->pSD->nClass);
			Write((DWORD)pUser->pSD->nLevel);
			Write((int)pUser->pSD->fHP);
			Write((int)pUser->pSD->MaxHP);
			Write((int)pUser->pSD->fMP);
			Write((int)pUser->pSD->MaxMP);
			Write((int)pUser->pSD->fCP);
			Write((int)pUser->pSD->MaxCP);
		}
	};
};
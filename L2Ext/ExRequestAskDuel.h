#pragma once

#include "ServerPacket.h"

class CExRequestAskDuel : public CServerPacket
{
public:
	CExRequestAskDuel(PWCHAR wszName, bool bParty)
	{
		Write((BYTE)0xFE);
		Write((WORD)0x4B);
		WriteS(wszName);
		Write((DWORD)bParty);
	};
};
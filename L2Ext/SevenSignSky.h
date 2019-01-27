#pragma once
#include "ServerPacket.h"

class CSevenSignSky : public CServerPacket
{
public:
	enum TYPE
	{
		NORMAL = 0,
		DUSK = 1,
		DAWN = 2
	};
	CSevenSignSky(int Type)
	{
		Write((BYTE)0xF8);
		switch(Type)
		{
		case DUSK:
			Write((WORD)257);
			break;
		case DAWN:
			Write((WORD)258);
			break;
		default:
			Write((WORD)256);
			break;
		}
	};
};
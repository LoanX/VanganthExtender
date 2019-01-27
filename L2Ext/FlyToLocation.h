#pragma once

#include "ServerPacket.h"

class CFlyToLocation : public CServerPacket
{
public:
	enum TYPE
	{
		THROW_UP = 0,
		THROW_HORIZONTAL = 1,
		DUMMY = 2,
		CHARGE = 3
	};
public:
	CFlyToLocation(int nObjectSID, int nDestX, int nDestY, int nDestZ, int nSrcX, int nSrcY, int nSrcZ, int nType)
	{
		Write((BYTE)0xC5);
		Write((INT32)nObjectSID);
		Write((INT32)nDestX);
		Write((INT32)nDestY);
		Write((INT32)nDestZ);
		Write((INT32)nSrcX);
		Write((INT32)nSrcY);
		Write((INT32)nSrcZ);
		Write((INT32)nType);
		
	};
};
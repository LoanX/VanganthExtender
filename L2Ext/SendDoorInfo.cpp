#include "StdAfx.h"
#include "Packets.h"

void CPacketFix::SendDoorInfo(CDoor* pDoor, CUserSocket* pSocket)
{
	pDoor->SendDoorInfo(pSocket);
}
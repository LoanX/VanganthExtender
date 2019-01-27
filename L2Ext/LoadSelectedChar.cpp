#include "StdAfx.h"
#include "Packets.h"
#include "SubStack.h"

void CPacketFix::LoadCharSelectedPacket(CUserSocket *pSocket, const char *format, ...)
{
	CPacket packet;
	va_list va;
	va_start(va, format);
/* c */packet.WriteC(va_arg(va, unsigned char));
/* S */packet.WriteS(va_arg(va, PWCHAR));
		UINT charId = va_arg(va, UINT);
/* d */packet.WriteD(charId);
/* S */packet.WriteS(va_arg(va, PWCHAR));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));

		UINT baseClass = va_arg(va, UINT);
		if(g_SubStack.IsEnabled())
		{
			UINT charIndex = UINT_MAX;
			for(UINT n=0;n<7;n++)
			{
				if(pSocket->characterDBID[n] == charId)
				{
					charIndex = n;
					break;
				}
			}
			if(charIndex != UINT_MAX && charIndex < 7)
			{
				if(pSocket->pED->baseClass[charIndex] != UINT_MAX)
				{
					baseClass = pSocket->pED->baseClass[charIndex];
				}
			}
		}

/* d */packet.WriteD(baseClass);

/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* f */packet.WriteF(va_arg(va, double));
/* f */packet.WriteF(va_arg(va, double));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteQ(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */UINT nTime = (va_arg(va, UINT));
		packet.WriteD(L2Server::GetL2Time());
/* d */packet.WriteD(va_arg(va, UINT));
/*BuffSize*/ UINT nBuffSize = va_arg(va, UINT);
/*b*/ char *szPrivilege = va_arg(va, char*);
		packet.WriteD(0);
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
/* d */packet.WriteD(va_arg(va, UINT));
	va_end(va);
	pSocket->Send("b", packet.GetSize(), packet.GetBuff());
}
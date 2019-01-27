#pragma once
#include "MultiPartyCommandChannelRegistry.h"

class CMPCC
{
public:
	static void Initialize();
	static void Identify(CSocket *pSocket, const char *format, ...);
	static int IdentifyAssemble(PCHAR buff, int nLen, const char *format, PWCHAR wsName, int nMemberCount);
	static void TeleportMPCC(User *pMaster, int x, int y, int z);
	static void TeleportMPCCByIndex(User *pMaster, UINT index, int x, int y, int z);
	static bool ShowPartyMemberInfo(User *pUser, UINT leaderId);
};
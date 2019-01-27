#pragma once
#include "AdminSocket.h"

typedef bool (__cdecl *AdminCallback)(CAdminSocket *pSocket, const unsigned char* packet);

class CAdminPacketHandler
{
public:
	static void Init();
	static bool __cdecl TestAdminPacket(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl SetHwidBan(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl SetAccountLock(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl SetCharacterLock(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl SetAdditionalBox(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl ItemDeliveryPacket(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl AddPledgeSkill(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl SetNameColor(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl SetTitleColor(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl SetCharacterPIN(CAdminSocket *pSocket, const unsigned char* packet);
	static bool __cdecl SetCharacterPIN2(CAdminSocket *pSocket, const unsigned char* packet);
};
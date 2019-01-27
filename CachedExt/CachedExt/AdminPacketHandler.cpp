#include "AdminPacketHandler.h"
#include "CachedExt.h"
#include "Query.h"

AdminCallback g_AdminPacketHandler[256];

CSocket** g_ServerSocket = (CSocket**)0x1E94948;

void CAdminPacketHandler::Init()
{
	AdminCallback* lpOrg = (AdminCallback*)0x55FB50;
	UINT index = 0;
	for(;index<110;)
	{
		g_AdminPacketHandler[index] = lpOrg[index++];
	}
	g_AdminPacketHandler[index++] = SetHwidBan;	//110
	g_AdminPacketHandler[index++] = SetAccountLock;	//111
	g_AdminPacketHandler[index++] = SetCharacterLock;	//112
	g_AdminPacketHandler[index++] = SetAdditionalBox;	//113
	g_AdminPacketHandler[index++] = ItemDeliveryPacket;	//114
	index++;	//SetCharacterPin 115
	g_AdminPacketHandler[index++] = AddPledgeSkill;	//116
	g_AdminPacketHandler[index++] = SetNameColor;	//117
	g_AdminPacketHandler[index++] = SetTitleColor;	//118
	g_AdminPacketHandler[index++] = SetCharacterPIN;	//119
	g_AdminPacketHandler[index++] = SetCharacterPIN2;	//120

	CachedExt::WriteMemoryBYTE(0x406F8D, static_cast<BYTE>(index));
	CachedExt::WriteMemoryDWORD(0x40699B, (DWORD)g_AdminPacketHandler);
}

bool __cdecl CAdminPacketHandler::TestAdminPacket(CAdminSocket *pSocket, const unsigned char* packet)
{
	g_Log.Add(CLog::Blue, "[%s] called!", __FUNCTION__);
	return false;
}


bool __cdecl CAdminPacketHandler::SetCharacterPIN2(CAdminSocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0;
	UINT pin = 0;
	Disassemble(packet, "dd", &charId, &pin);
	bool succeed = false;

	CQuery query(L"EXEC lin_SetPIN ?, ?, ?");
	query.AddParam(charId);
	query.AddParam(pin);
	query.AddParam((UINT)time(0));
	DBConn *pCon = new DBConn();
	if(pCon->Execute(query.Get()))
	{
		succeed = true;
	}
	delete pCon;
	if(succeed)
	{
		CSocket *pServer = (*g_ServerSocket);
		if(pServer)
		{
			CHAR buffEx[8190];
			UINT lenEx = Assemble(buffEx, sizeof(buffEx), "cddd", 0x22, CO_REPLY_LOAD_CHARACTER_PIN, charId, pin);
			pServer->Send(lenEx, buffEx);
		}
	}

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 120, succeed);
	pSocket->Send(len, buff);
	unguard;
	return false;
}

bool __cdecl CAdminPacketHandler::SetCharacterPIN(CAdminSocket *pSocket, const unsigned char* packet)
{
	guard;

	WCHAR pinHash[33] = { 0 };
	UINT charId = 0;

	bool succeed = false;

	packet = Disassemble(packet, "d", &charId);
	for(UINT n=0;n<32;n++)
	{
		pinHash[n] = packet[n];
	}

	CQuery query(L"SELECT COUNT(*) FROM [CharacterLock] WHERE [char_id] = ?");
	query.AddParam(charId);

	DBConn *pCon = new DBConn();
	UINT exist = 0;
	pCon->Bind(&exist);
	if(pCon->Execute(query.Get()))
	{
		pCon->Fetch();
	}

	pCon->ResetParam();

	if(exist)
	{
		query.Set(L"UPDATE [CharacterLock] SET [code_hash] = '?' WHERE [char_id] = ?");
		query.AddParam(pinHash);
		query.AddParam(charId);
		if(pCon->Execute(query.Get()))
		{
			succeed = true;
			g_Logger.Add(L"[%S] Updated CharacterLock for char_id[%d]", __FUNCTION__, charId);
		}
	}else
	{
		query.Set(L"INSERT INTO [CharacterLock] ([code_hash], [hardware_id], [char_id]) VALUES ('?', '?', ?)");
		query.AddParam(pinHash);
		query.AddParam(L"00000000000000000000000000000000");
		query.AddParam(charId);
		if(pCon->Execute(query.Get()))
		{
			succeed = true;
			g_Logger.Add(L"[%S] Added CharacterLock for char_id[%d]", __FUNCTION__, charId);
		}
	}
	delete pCon;

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 119, succeed);
	pSocket->Send(len, buff);

	unguard;

	return false;
}

bool __cdecl CAdminPacketHandler::SetTitleColor(CAdminSocket *pSocket, const unsigned char* packet)
{
	UINT charId = 0, titleColor = 0;
	bool succeed = false;
	Disassemble(packet, "dd", &charId, &titleColor);
	
	if( charId > 0 )
	{
		CQuery query(L"UPDATE [user_data] SET [title_color] = ? WHERE [char_id] = ? ");
		query.AddParam(titleColor);
		query.AddParam(charId);
		DBConn* pDBConn = new DBConn;
		if(pDBConn->Execute(query.Get()))
		{
			succeed = true;
			g_Logger.Add(L"Setting charId[%d] title color[%d]", charId, titleColor);
		}

		delete pDBConn;
	}

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 118, succeed);
	pSocket->Send(len, buff);
	return false;
}

bool __cdecl CAdminPacketHandler::SetNameColor(CAdminSocket *pSocket, const unsigned char* packet)
{
	UINT charId = 0, nameColor = 0;
	bool succeed = false;
	Disassemble(packet, "dd", &charId, &nameColor);
	
	if( charId > 0 )
	{
		CQuery query(L"UPDATE [user_data] SET [name_color] = ? WHERE [char_id] = ? ");
		query.AddParam(nameColor);
		query.AddParam(charId);
		DBConn* pDBConn = new DBConn;
		if(pDBConn->Execute(query.Get()))
		{
			succeed = true;
			g_Logger.Add(L"Setting charId[%d] name color[%d]", charId, nameColor);
		}

		delete pDBConn;
	}

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 117, succeed);
	pSocket->Send(len, buff);
	return false;
}


bool __cdecl CAdminPacketHandler::AddPledgeSkill(CAdminSocket *pSocket, const unsigned char* packet)
{
	UINT pledgeId = 0;
	INT32 skillId = 0, skillLevel = 0;
	bool succeed = false;
	Disassemble(packet, "ddd", &pledgeId, &skillId, &skillLevel);
	if(pledgeId > 0 && skillId >0 && skillLevel > 0)
	{
		CQuery query(L"EXEC lin_AddPledgeSkill ?, ?, ?");
		query.AddParam(pledgeId);
		query.AddParam(skillId);
		query.AddParam(skillLevel);
		DBConn* pDBConn = new DBConn;
		if(pDBConn->Execute(query.Get()))
		{
			succeed = true;
			CSocket *pServer = (*g_ServerSocket);
			if(pServer)
			{
				CHAR buffEx[8190];
				UINT lenEx = Assemble(buffEx, sizeof(buffEx), "cddddd", 0x22, CO_REPLY_LOAD_PLEDGE_SKILLS, pledgeId, 1, skillId, skillLevel);
				pServer->Send(lenEx, buffEx);
				succeed = true;
			}
		}

		delete pDBConn;
	}

	if(succeed)
	{
		g_Logger.Add(L"Admin Packet AddPledgeSkill - pledgeId[%d] skillId[%d] skillLevel[%d]", pledgeId, skillId, skillLevel);
	}

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 116, succeed);
	pSocket->Send(len, buff);
	return false;
}

bool __cdecl CAdminPacketHandler::ItemDeliveryPacket(CAdminSocket *pSocket, const unsigned char *packet)
{
	UINT charId = 0;
	INT32 itemId = 0, itemAmount = 0, enchant = 0;
	WCHAR wName[25];
	bool succeed = false;
	Disassemble(packet, "ddddS", &charId, &itemId, &itemAmount, &enchant, sizeof(wName), wName);
	if(charId > 0 && itemId > 0 && itemAmount > 0 && enchant > -1 && enchant < 51)
	{
		CQuery query(L"INSERT INTO [ItemDelivery] ([char_id], [item_id], [item_amount], [enchant]) VALUES (?, ?, ?, ?)");
		query.AddParam(charId);
		query.AddParam(itemId);
		query.AddParam(itemAmount);
		query.AddParam(enchant);
		DBConn *pCon = new DBConn;
		if(pCon->Execute(query.Get()))
		{
			succeed = true;
			g_Logger.Add(L"Item Delivery Packet user[%d] item[%d][%d][%d] by[%s]", charId, itemId, itemAmount, enchant, wName);
		}
		delete pCon;
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid params [%d][%d][%d][%d]", __FUNCTION__, charId, itemId, itemAmount, enchant);
	}

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 114, succeed);
	pSocket->Send(len, buff);


	return false;
}

bool __cdecl CAdminPacketHandler::SetAdditionalBox(CAdminSocket *pSocket, const unsigned char* packet)
{
	UINT accountId = 0, timestamp = 0;
	Disassemble(packet, "dd", &accountId, &timestamp);
	bool succeed = false;
	if(accountId > 0)
	{
		CQuery query(L"EXEC lin_ProcessAdditionalBox ?, ?");
		query.AddParam(accountId);
		query.AddParam(timestamp);
		DBConn *pConn = new DBConn;
		pConn->Execute(query.Get());
		delete pConn;
		g_Logger.Add(L"Admin packet set additional box account[%d] timestamp[%d]", accountId, timestamp);
		succeed = true;
	}

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 113, succeed);
	pSocket->Send(len, buff);

	return false;
}

bool __cdecl CAdminPacketHandler::SetCharacterLock(CAdminSocket *pSocket, const unsigned char* packet)
{
	UINT charId = 0;
	WCHAR wHwid[33];
	memset(wHwid, 0, sizeof(wHwid));
	WCHAR wName[25];
	memset(wName, 0, sizeof(wName));
	Disassemble(packet, "dSS", &charId, sizeof(wHwid), wHwid, sizeof(wName), wName);

	bool succeed = false;
	
	CQuery query(L"EXEC lin_ProcessLockedHardwareId ?, ?, '?', '?'");
	query.AddParam(0);
	query.AddParam(charId);	//charId
	query.AddParam(wHwid);
	query.AddParam(wName);

	DBConn* pDBConn = new DBConn;
	if(pDBConn->Execute(query.Get()))
	{
		succeed = true;
		CSocket *pServer = (*g_ServerSocket);
		if(pServer)
		{
			CHAR buff[8190];
			BYTE hwid[32];
			for(UINT n=0;n<32;n++)
			{
				hwid[n] = static_cast<BYTE>(wHwid[n]);
			}
			UINT len = Assemble(buff, sizeof(buff), "cdddb", 0x22, CO_REPLY_LOAD_HWID_LOCK_DATA, 0, charId, 32, hwid);
			pServer->Send(len, buff);
			succeed = true;
		}
	}

	delete pDBConn;

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 112, succeed);
	pSocket->Send(len, buff);

	g_Logger.Add(L"Admin packet lock character[%d] hwid[%s] gm[%s] succeed[%d]", charId, wHwid, wName, succeed);

	return false;
}

bool __cdecl CAdminPacketHandler::SetAccountLock(CAdminSocket *pSocket, const unsigned char* packet)
{
	UINT accountId = 0;
	WCHAR wHwid[33];
	memset(wHwid, 0, sizeof(wHwid));
	WCHAR wName[25];
	memset(wName, 0, sizeof(wName));
	Disassemble(packet, "dSS", &accountId, sizeof(wHwid), wHwid, sizeof(wName), wName);

	bool succeed = false;
	
	CQuery query(L"EXEC lin_ProcessLockedHardwareId ?, ?, '?', '?'");
	query.AddParam(accountId);
	query.AddParam(0);	//charId
	query.AddParam(wHwid);
	query.AddParam(wName);

	DBConn* pDBConn = new DBConn;
	if(pDBConn->Execute(query.Get()))
	{
		succeed = true;
		CSocket *pServer = (*g_ServerSocket);
		if(pServer)
		{
			CHAR buff[8190];
			BYTE hwid[32];
			for(UINT n=0;n<32;n++)
			{
				hwid[n] = static_cast<BYTE>(wHwid[n]);
			}
			UINT len = Assemble(buff, sizeof(buff), "cdddb", 0x22, CO_REPLY_LOAD_HWID_LOCK_DATA, accountId, 0, 32, hwid);
			pServer->Send(len, buff);
			succeed = true;
		}
	}

	delete pDBConn;

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 111, succeed);
	pSocket->Send(len, buff);

	g_Logger.Add(L"Admin packet lock account[%d] hwid[%s] gm[%s] succeed[%d]", accountId, wHwid, wName, succeed);

	return false;
}

bool __cdecl CAdminPacketHandler::SetHwidBan(CAdminSocket *pSocket, const unsigned char* packet)
{
	WCHAR wHwid[33];
	memset(wHwid, 0, sizeof(wHwid));
	UINT timestamp = 0;
	WCHAR wName[25];
	WCHAR wAccount[25];
	memset(wName, 0, sizeof(wName));
	memset(wAccount, 0, sizeof(wAccount));

	Disassemble(packet, "SdSS", sizeof(wHwid), wHwid, &timestamp, sizeof(wAccount), wAccount, sizeof(wName), wName);

	bool succeed = false;
	if(wcslen(wHwid) == 32)
	{
		g_Logger.Add(L"Admin Packet SetHwidBan - hwid[%s] account[%s] ban - timestamp[%d] by[%s]", wHwid, wAccount, timestamp, wName);
		if(timestamp == 0)
		{
			//unban
			CQuery query(L"EXEC lin_UnBanHardwareId '?'");
			query.AddParam(wHwid);
			DBConn *pCon = new DBConn;
			if(pCon->Execute(query.Get()))
			{
				succeed = true;
			}
			delete pCon;
		}else
		{
			//ban
			CQuery query(L"EXEC lin_BanHardwareId '?', '?', '?', ?");
			query.AddParam(wHwid);
			query.AddParam(wAccount);
			query.AddParam(wName);
			query.AddParam(timestamp);

			DBConn *pCon = new DBConn;
			if(pCon->Execute(query.Get()))
			{
				succeed = true;
			}
			delete pCon;
		}

		if(succeed)
		{
			//succeed, send packet to server
			CSocket *pServer = (*g_ServerSocket);
			if(pServer)
			{
				CHAR buff[8190];
				BYTE hwid[32];
				for(UINT n=0;n<32;n++)
				{
					hwid[n] = static_cast<BYTE>(wHwid[n]);
				}
				UINT len = Assemble(buff, sizeof(buff), "cdbSSd", 0x22, CO_REPLY_LOAD_BANNED_HARDWARE_ID, 32, hwid, wAccount, wName, timestamp);
				pServer->Send(len, buff);
				succeed = true;
			}
		}
	}

	CHAR buff[8190];
	UINT len = Assemble(buff, 8190, "cd", 110, succeed);
	pSocket->Send(len, buff);

	return false;
}
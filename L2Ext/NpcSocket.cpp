#include "StdAfx.h"
#include "MPCC.h"
#include "TerritoryEx.h"
#include "UserCache.h"

/*
 Begin handler table BF4100
 //0x38 opCode = Dummy packet
 End handler table BF44E0
*/
void CNpcSocket::Initialize()
{
	g_HookManager.WriteMemoryDWORD(0xBF4410, (DWORD)NpcCreateSubJob);
	g_HookManager.WriteMemoryDWORD(0xBF4418, (DWORD)NpcChangeSubJob);
	g_HookManager.WriteMemoryDWORD(0xBF4420, (DWORD)NpcCreateSubJob2);
	g_HookManager.WriteMemoryDWORD(0xBF4210, (DWORD)NpcInstantTeleportInMyTerritory);
	g_HookManager.WriteMemoryDWORD(0xBF42C0, (DWORD)Reply);
}

bool CNpcSocket::Reply(CNpcSocket *pSocket, const unsigned char* packet)
{
	DWORD OpCode = 0;
	packet = (PUCHAR)Disassemble(packet, "d", &OpCode);
	
//	g_Log.Add(CLog::Blue, "[%s] OpCode[%d]", __FUNCTION__, OpCode);

	switch(OpCode)
	{
	case TEST_NPC_OPCODE:
		{
			return pSocket->TestPacket(packet);
			break;
		}
	case INITIALIZE_ERA:
		{
			if(g_Config.IsSet(CConfig::ERA_SYSTEM))
				return pSocket->InitializeEra(packet);
			break;
		}
	case PROCESSING_ERA_NPC_DONE:
		{
			if(g_Config.IsSet(CConfig::ERA_SYSTEM))
				return pSocket->ProcessingEraNpcDone(packet);
			break;
		}
	case CHANGE_ERA:
		{
			if(g_Config.IsSet(CConfig::ERA_SYSTEM))
				return pSocket->ChangeEra(packet);
			break;
		}
	case TELEPORT_MPCC:
		{
			return pSocket->TeleportMPCC(packet);
			break;
		}
	case TELEPORT_MPCC_BY_INDEX:
		{
			return pSocket->TeleportMPCCByIndex(packet);
			break;
		}
	case SET_PC_LEVEL:
		{
			return pSocket->SetPCLevel(packet);
			break;
		}
	case ADD_PC_SOCIAL:
		{
			return pSocket->AddPCSocial(packet);
			break;
		}
	case UPDATE_PLEDGE_NAME_VALUE:
		{
			return pSocket->UpdatePledgeNameValue(packet);
			break;
		}
	default:
		return pSocket->NpcDummy(packet);
		break;
	}

	return false;
};

bool CNpcSocket::UpdatePledgeNameValue(const unsigned char* packet)
{
	guard;
	UINT talkerId = 0;
	INT32 value = 0;
	Disassemble(packet, "dd", &talkerId, &value);
	if(talkerId > 0)
	{
		if(User *pUser = User::GetUserBySID(&talkerId))
		{
			if(pUser->pSD->nPledgeID > 0)
			{
				if(CPledge *pPledge = pUser->GetPledge())
				{
					pPledge->UpdatePledgeNameValue(value);
					g_Logger.Add(L"[%s] Talker[%s] pledge[%s][%d] value[%d]", __FUNCTIONW__, pUser->pSD->wszName, pPledge->pSD->wszClanName, pPledge->pSD->nDBID, value);
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] talker[%S] doesn't have a pledge - value[%d]", __FUNCTION__, pUser->pSD->wszName, value);
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find talker[%d]!", __FUNCTION__, talkerId);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] null talker id!", __FUNCTION__);
	}
	unguard;
	return false;
}

bool CNpcSocket::AddPCSocial(const unsigned char *packet)
{
	guard;
	UINT talkerId = 0;
	UINT actionId = 0;
	Disassemble(packet, "dd", &talkerId, &actionId);
	if(talkerId > 0)
	{
		if(User *pUser = User::GetUserBySID(&talkerId))
		{
			if(actionId < 18)
			{
				pUser->SocialAction((SocialActionType)actionId);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid social action[%d] talker[%S]", __FUNCTION__, actionId, pUser->pSD->wszName);
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find talker[%d]!", __FUNCTION__, talkerId);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] null talker id!", __FUNCTION__);
	}

	unguard;
	return false;
}

bool CNpcSocket::SetPCLevel(const unsigned char* packet)
{
	guard;
	UINT talkerId = 0;
	INT32 newLevel = 0;
	Disassemble(packet, "dd", &talkerId, &newLevel);

	if(talkerId > 0)
	{
		if(User *pUser = User::GetUserBySID(&talkerId))
		{
			if(newLevel > 0 && newLevel <= 80)
			{
				pUser->SetLevel(newLevel);
				g_Logger.Add(L"[%S] talker[%s] new level[%d]", __FUNCTION__, pUser->pSD->wszName, newLevel);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid new level[%d] - talker[%S]!", __FUNCTION__, newLevel, pUser->pSD->wszName);
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find talker[%d]!", __FUNCTION__, talkerId);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] null talker id!", __FUNCTION__);
	}
	unguard;
	return false;
}

bool CNpcSocket::TeleportMPCC(const unsigned char* packet)
{
	guard;

	UINT talkerId = 0;
	INT32 x = 0, y = 0, z = 0;
	Disassemble(packet, "dddd", &talkerId, &x, &y, &z);

	if(talkerId > 0)
	{
		User *pUser = User::GetUserBySID(&talkerId);
		if(pUser->ValidUser())
		{
			CMPCC::TeleportMPCC(pUser, x, y, z);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid user[%d]!", __FUNCTION__);
		}
	}
	unguard;
	return false;
}

bool CNpcSocket::TeleportMPCCByIndex(const unsigned char* packet)
{
	guard;

	UINT talkerId = 0;
	UINT index = 0;
	INT32 x = 0, y = 0, z = 0;
	Disassemble(packet, "ddddd", &talkerId, &index, &x, &y, &z);

	if(talkerId > 0)
	{
		User *pUser = User::GetUserBySID(&talkerId);
		if(pUser->ValidUser())
		{
			CMPCC::TeleportMPCCByIndex(pUser, index, x, y, z);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid user[%d]!", __FUNCTION__);
		}
	}
	unguard;
	return false;
}

bool CNpcSocket::NpcDummy(const unsigned char* packet)
{
	typedef bool(*f)(CNpcSocket*, const unsigned char*);
	return f(0x6CF770)(this, packet);
}

bool CNpcSocket::InitializeEra(const unsigned char* packet)
{
	g_EraSystem.Initialize(packet);
	return false;
}

bool CNpcSocket::ProcessingEraNpcDone(const unsigned char* packet)
{
	WCHAR wEra[25]; memset(wEra, 0, sizeof(wEra));
	int nEraIndex = 0, nSpawned = 0;
	Disassemble(packet, "dSd", &nEraIndex, sizeof(wEra), wEra, &nSpawned);
	g_EraSystem.ProcessingFinished(nEraIndex, wEra, nSpawned);
	return false;
}

bool CNpcSocket::ChangeEra(const unsigned char* packet)
{
	WCHAR wEra[25]; memset(wEra, 0, sizeof(wEra));
	DWORD nEra = 0;
	DWORD nLastChanged = 0;
	Disassemble(packet, "dSd", &nEra, sizeof(wEra), wEra, &nLastChanged);
	g_EraSystem.ChangeEra(nEra, wEra, nLastChanged);
	return false;
}

bool CNpcSocket::TestPacket(const unsigned char* packet)
{
	int SubCode = 0;
	Disassemble(packet, "d", &SubCode);
	g_Log.Add(CLog::Blue, "[%s] SubCode[%d]", __FUNCTION__, SubCode);

	return false;
}

bool CNpcSocket::NpcCreateSubJob(CNpcSocket *pSocket, const unsigned char* packet)
{
	_NpcSockFunc NCSJ = (_NpcSockFunc) 0x006E30E0;
	int nNpcSID, nUserSID, nSubjobID;
	Disassemble(packet, "ddd", &nNpcSID, &nUserSID, &nSubjobID);
	CObjectSP UserSP;
	CObjectSP::FindObject(&UserSP, nUserSID);
	User *pUser = UserSP.pObject->GetUser();
	if(pUser->ValidUser())
	{
		DWORD nTime = time(NULL);
		if(pUser->pSD->nSubJobManipulateDelay > nTime)
		{
			pUser->SendSystemMessage(L"You have to wait 30 seconds between changing subclasses.");
			return false;
		}else
		{
			pUser->pSD->nSubJobManipulateDelay = nTime + 30;
		}
	}
	return NCSJ(pSocket, packet);
}
bool CNpcSocket::NpcChangeSubJob(CNpcSocket *pSocket, const unsigned char* packet)
{
	_NpcSockFunc NCSJ = (_NpcSockFunc) 0x006E3600;
	int nNpcSID, nUserSID, nSubjobID;
	Disassemble(packet, "ddd", &nNpcSID, &nUserSID, &nSubjobID);
	CObjectSP UserSP;
	CObjectSP::FindObject(&UserSP, nUserSID);
	User *pUser = UserSP.pObject->GetUser();
	if(pUser->ValidUser())
	{
		DWORD nTime = time(NULL);
		if(pUser->pSD->nSubJobManipulateDelay > nTime)
		{
			pUser->SendSystemMessage(L"You have to wait 30 seconds between changing subclasses.");
			return false;
		}else
		{
			pUser->pSD->nSubJobManipulateDelay = nTime + 30;
		}
	}
	return NCSJ(pSocket, packet);
}
bool CNpcSocket::NpcCreateSubJob2(CNpcSocket *pSocket, const unsigned char* packet)
{
	_NpcSockFunc NCSJ = (_NpcSockFunc) 0x006E3AF0;
	int nNpcSID, nUserSID, nSubjobID;
	Disassemble(packet, "ddd", &nNpcSID, &nUserSID, &nSubjobID);
	CObjectSP UserSP;
	CObjectSP::FindObject(&UserSP, nUserSID);
	User *pUser = UserSP.pObject->GetUser();
	if(pUser->ValidUser())
	{
		DWORD nTime = time(NULL);
		if(pUser->pSD->nSubJobManipulateDelay > nTime)
		{
			pUser->SendSystemMessage(L"You have to wait 30 seconds between changing subclasses.");
			return false;
		}else
		{
			pUser->pSD->nSubJobManipulateDelay = nTime + 30;
		}
	}
	return NCSJ(pSocket, packet);
}

bool CNpcSocket::NpcInstantTeleportInMyTerritory(CNpcSocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT temp;
	UINT npcId = 0;
	INT32 destX = 0, destY = 0, destZ = 0, range = 0;
	const unsigned char* pck = Disassemble(packet, "ddddddddddd", &npcId, &destX, &destY, &destZ, &range, &temp, &temp, &temp, &temp, &temp, &temp);

	UINT territoryCount = 0;
	pck = Disassemble(pck, "d", &territoryCount);
	vector<CTerritoryEx> bannedTerritories;
	//banned territory
	for(UINT territoryCounter = 0; territoryCounter < territoryCount; territoryCounter++)
	{
		UINT pointCount = 0;
		pck = Disassemble(pck, "d", &pointCount);
		CTerritoryEx territory;
		for(UINT pointCounter = 0;pointCounter < pointCount; pointCounter++)
		{
			INT32 x = 0, y = 0, z = 0;
			pck = Disassemble(pck, "ddd", &x, &y, &z);
			FVector pos;
			pos.x = static_cast<double>(x);
			pos.y = static_cast<double>(y);
			pos.z = static_cast<double>(z);
			territory.AddPoint(pos);
		//	g_Log.Add(CLog::Blue, "Point idx: %d loc: %d, %d, %d", pointCounter, x, y, z);
		}
		bannedTerritories.push_back(territory);
	}

	vector<CTerritoryEx> territories;
	//Territory data
	pck = Disassemble(pck, "d", &territoryCount);
	for(UINT territoryCounter = 0; territoryCounter < territoryCount; territoryCounter++)
	{
		UINT pointCount = 0;
		INT32 minZ = 0, maxZ = 0;
		pck = Disassemble(pck, "ddd", &pointCount, &maxZ, &minZ);
		CTerritoryEx territory;
		for(UINT pointCounter = 0;pointCounter < pointCount; pointCounter++)
		{
			INT32 x = 0, y = 0;
			pck = Disassemble(pck, "dd", &x, &y);
			FVector pos;
			pos.x = static_cast<double>(x);
			pos.y = static_cast<double>(y);
			pos.z = static_cast<double>((minZ + maxZ) / 2);
			territory.AddPoint(pos);

		//	g_Log.Add(CLog::Blue, "Point Z Range idx: %d loc: %d, %d MinZ: %d MaxZ: %d", pointCounter, x, y, minZ, maxZ);
		}
		territory.AddZRange(static_cast<double>(minZ), static_cast<double>(maxZ));
		territories.push_back(territory);
	}

	g_UserCache.InstantTeleportInMyTerritory(destX, destY, destZ, range, territories, bannedTerritories);

	bool ret = _NpcSockFunc(0x6EDAA0L)(pSocket, packet);
	unguard;
	return ret;
}
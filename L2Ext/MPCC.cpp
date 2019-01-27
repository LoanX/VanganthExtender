#include "StdAfx.h"
#include "MPCC.h"

void CMPCC::Initialize()
{
	g_HookManager.WriteCall( 0x0068E34F, Identify, 0);
	g_HookManager.WriteCall( 0x0068E21E, IdentifyAssemble, 0);
	g_HookManager.WriteJump( 0x68A273, CMultiPartyCommandChannelManager_WithdrawInternal1, 2);
}

void CMPCC::Identify(CSocket *pSocket, const char *format, ...)
{
	va_list tag;
	va_start(tag, format);
	/*c*/ BYTE OpCode = va_arg(tag, BYTE);
	/*h*/ WORD ExOpCode = va_arg(tag, WORD);
	/*S*/ PWCHAR wsOwner = va_arg(tag, PWCHAR);
	/*d*/ DWORD dMemberCount = va_arg(tag, DWORD);
	/*d*/ DWORD dPartyCount = va_arg(tag, DWORD);
	/*b*/ DWORD BuffSize = va_arg(tag, DWORD); PUCHAR Buff = va_arg(tag, PUCHAR);
	va_end(tag);
	UINT nOwnerId = 1;
	if(User *pUser = g_UserDB.GetUserByName(wsOwner))
	{
		nOwnerId = pUser->nObjectID;
	}
	pSocket->Send("chSdddb", OpCode, ExOpCode, wsOwner, nOwnerId, dMemberCount, dPartyCount, BuffSize, Buff);
}

int CMPCC::IdentifyAssemble(PCHAR buff, int nLen, const char *format, PWCHAR wsName, int nMemberCount)
{
	UINT nLeaderId = 0;
	if(User *pUser = g_UserDB.GetUserByName(wsName))
	{
		nLeaderId = pUser->nObjectID;
	}
	return Assemble(buff, nLen, "Sdd", wsName, nLeaderId, nMemberCount);
}

void CMPCC::TeleportMPCC(User *pMaster, int x, int y, int z)
{
	guard;
	if(CParty *pParty = pMaster->GetParty())
	{
		if(pParty->GetMaster() == pMaster)
		{
			CMultiPartyCommandChannelSP channelSP;
			CMultiPartyCommandChannelRegistry::FindByMasterParty(channelSP, pParty);

			if(CMultiPartyCommandChannel* pChannel = channelSP.get())
			{
				g_Logger.Add(L"Teleporting MPCC to location [%d][%d][%d] - requestor[%s]", x, y, z, pMaster->pSD->wszName);
				pChannel->TeleportMPCCToLocation(pMaster->nObjectID, x, y, z);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find CMultiPartyCommandChannel for user[%S]", __FUNCTION__, pMaster->pSD->wszName);
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] is not party master!", __FUNCTION__, pMaster->pSD->wszName);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] User[%S] has no party!", __FUNCTION__, pMaster->pSD->wszName);
	}
	unguard;
}

void CMPCC::TeleportMPCCByIndex(User *pMaster, UINT index, int x, int y, int z)
{
	guard;
	if(CParty *pParty = pMaster->GetParty())
	{
		if(pParty->GetMaster() == pMaster)
		{
			CMultiPartyCommandChannelSP channelSP;
			CMultiPartyCommandChannelRegistry::FindByMasterParty(channelSP, pParty);

			if(CMultiPartyCommandChannel* pChannel = channelSP.get())
			{
				g_Logger.Add(L"Teleporting MPCC to location by party index[%d] loc [%d][%d][%d] - requestor[%s]", index, x, y, z, pMaster->pSD->wszName);
				pChannel->TeleportMPCCToLocationByIndex(pMaster->nObjectID, index, x, y, z);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find CMultiPartyCommandChannel for user[%S]", __FUNCTION__, pMaster->pSD->wszName);
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] is not party master!", __FUNCTION__, pMaster->pSD->wszName);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] User[%S] has no party!", __FUNCTION__, pMaster->pSD->wszName);
	}
	unguard;
}

bool CMPCC::ShowPartyMemberInfo(User *pUser, UINT leaderId)
{
	guard;
	if(CParty *pParty = pUser->GetParty())
	{
		if(User *pMaster = User::GetUserBySID(&leaderId)->SafeCastUser())
		{
			if(CParty *pMasterParty = pMaster->GetParty())
			{
				UserVector members;
				pMasterParty->GetAllMembersEx(members);
				CHAR buff[8190];
				int len = Assemble(buff, 8190, "chd", 0xFE, 0x4A, pParty->memberCount);
				for(UINT n=0;n<members.size();n++)
				{
					if(User *pMember = members[n]->SafeCastUser())
					{
						len += Assemble(&buff[len], (sizeof(buff) - len), "Sdd", pMember->pSD->wszName, pMember->nObjectID, pMember->pSD->nClass);
					}
				}
				pUser->pSocket->Send("b", len, buff);
			}

		}
	}
	unguard;
	return false;
}
#include "StdAfx.h"
#include "CursedWeaponSystem.h"
#include "UserExPacket.h"
#include "ClanSys.h"
#include "Augmentation.h"
#include "InfinityShot.h"
#include "MPCC.h"
#include "DuelSystem.h"
#include "OlympiadSystem.h"
#include "ItemAutoConsume.h"

extern BYTE g_LastUsedPacketEx[26];
extern DWORD g_ItemPacketReuse;

bool CUserExPacket::Handle(CUserSocket *pSocket, const unsigned char* packet)
{
	
	WORD OpCode;
	const unsigned char* orgPacket = packet;
	packet = Disassemble(packet, "h", &OpCode);

//	g_Log.Add(CLog::Blue,"[%s] opCode[%x]", __FUNCTION__, OpCode);
	
	UINT threadIdx = GetThreadIndex();
	if(threadIdx < 26)
	{
		g_LastUsedPacketEx[threadIdx] = (BYTE)OpCode;
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid thread index[%d]", __FUNCTION__, threadIdx);
	}

	switch(OpCode)
	{
	case 0x05: //RequestAutoSSEx
		{
			if(User *pUser = pSocket->pUser->SafeCastUser())
			{
				if(pUser->IsNowTrade())
				{
					pUser->TradeCancel();
					return false;
				}
				if(pUser->pSD->nAlive == 0)
				{
					return false;
				}

				INT32 itemId = 0;
				UINT enabled = 0;

				Disassemble(packet, "dd", &itemId, &enabled);
				if(enabled)
				{
					if(pUser->olympiadUser.IsInside())
					{
						if(g_OlympiadDB.IsForbiddenItem(itemId))
						{
							pUser->pSocket->SendSystemMessage(1508);
							return false;
						}
					}
				}

				if(g_ItemAutoConsume.IsAutoConsume(itemId))
				{
					if(enabled)
					{
						g_ItemAutoConsume.TurnOn(pUser, itemId);
					}else
					{
						g_ItemAutoConsume.TurnOff(pUser, itemId);
					}
					return false;
				}

				if(g_InfinityShot.IsEnabled())
				{
					if(enabled)
					{
						if(g_InfinityShot.AutoOn(pUser, itemId))
						{
							return false;
						}
					}else
					{
						if(g_InfinityShot.AutoOff(pUser, itemId))
						{
							return false;
						}
					}
				}
				

				return UserSocketCallback(0x0086BE60L)(pSocket, orgPacket);
			}
			return false;
			break;
		}
	case 0x06:
		{
			if(User *pUser = pSocket->GetUser())
			{
				//ExEnchantSkillInfo
				int nSkillID, nLevel;
				Disassemble(packet, "dd", &nSkillID, &nLevel);
				if(nSkillID == 0 || nLevel == 0 || nLevel < 0 || nSkillID < 0)
				{
					return false;
				}

				return UserSocketCallback(0x86BE60L)(pSocket, orgPacket);
			}
			return false;
			break;
		}
	case 0x07:
		{
			if(User *pUser = pSocket->GetUser())
			{
				pSocket->pED->itemActionTimestamp = GetTickCount() + g_ItemPacketReuse;
				return UserSocketCallback(0x86BE60L)(pSocket, orgPacket);
			}
			return false;
			break;
		}
	case 0x14:
		{
			return AskJoinPartyRoom(pSocket, packet);
			break;
		}
	case 0x15:
		{
			return AnswerJoinPartyRoom(pSocket, packet);
			break;
		}
	case 0x16:
		{
			return RequestListPartyMatchingWaitingRoom(pSocket, packet);
			break;
		}
	case 0x17:
		{
		//	g_Log.Add(CLog::Blue, "[Request exit party matching room]");
			//Delete from party matching waiting list send FE 26
			return RequestExitPartyMatchingWaitingRoom(pSocket, packet);
			break;
		}
	case 0x18: //RequestGetBossRecord
		{
			pSocket->Send("chddd", 0xFE, 0x33, 0, 0, 0);
			break;
		}
	case 0x19: //RequestPledgeSetAcademyMaster
		{
			return ClanSys::RequestSetSponsor(pSocket, packet);
			break;
		}
	case 0x1A: //RequestPledgePowerGradeList
		{
			return ClanSys::RequestPledgePowerGradeList(pSocket, packet);
			break;
		}
	case 0x1B:
		{
			return ClanSys::RequestMemberPowerInfo(pSocket, packet);
			break;
		}
	case 0x1C:
		{
			return ClanSys::RequestSetMemberRank(pSocket, packet);
			break;
		}
	case 0x1D:
		{
			return ClanSys::RequestMemberInfo(pSocket, packet);
			break;
		}
	case 0x1E:
		{
			return ClanSys::RequestWarInfo(pSocket, packet);
			break;
		}
	case 0x1F: //RequestExFishRanking
		{
			// No Packet
			break;
		}
	case 0x20: //RequestPCCafeCouponUse
		{
			// No Packet
			break;
		}
	case 0x21: //NotFound
		{
			// No Packet
			break;
		}
	case 0x22: //RequestCursedWeaponList
		{
			return g_CursedWeaponSystem.WeaponList(pSocket);
			break;
		}
	case 0x23: //RequestCursedWeaponLocation
		{
			return g_CursedWeaponSystem.WeaponLocation(pSocket);
			break;
		}
	case 0x24: //RequestPledgeReorganizeMember
		{
			return ClanSys::RequestReorganizeMember(pSocket, packet);
			break;
		}
	case 0x25:
		{
			return false;
			break;
		}
	case 0x26:	//RequestExMPCCShowPartyMembersInfo
		{
			return RequestExShowMPCCPartyMemberInfo(pSocket, packet);
			break;
		}
	case 0x27: //RequestDuelStart
		{
			if(g_DuelSystem.IsEnabled())
			{
				return g_DuelSystem.RequestAskDuel(pSocket, packet);
			}
			return false;
			break;
		}
	case 0x28:
		{
			if(g_DuelSystem.IsEnabled())
			{
				return g_DuelSystem.RequestAnswerDuel(pSocket, packet);
			}
			return false;
			break;
		}
	case 0x29: //RequestConfirmTargetItem - Augmentation
		{
			return g_Augmentation.RequestConfirmTargetItem(pSocket, packet);
			break;
		}
	case 0x2A: //RequestConfirmRefinerItem - Augmentation
		{
			return g_Augmentation.RequestConfirmRefinerItem(pSocket, packet);
			break;
		}
	case 0x2B: //RequestConfirmGemStone
		{
			return g_Augmentation.RequestConfirmGemStone(pSocket, packet);
			break;
		}
	case 0x2C:	//Augment Weapon
		{
			return g_Augmentation.RequestAugmentItem(pSocket, packet);
			break;
		}
	case 0x2D: //RequestConfirmCancelItem
		{
			return g_Augmentation.RequestConfirmCancelItem(pSocket, packet);
			break;
		}
	case 0x2E:
		{
			return g_Augmentation.RequestCancelItem(pSocket, packet);
			break;
		}
	case 0x2F:
		{	
			return RequestTargetGround(pSocket, packet);
			break;
		}
	case 0x30: //RequestDuelSurrender
		{
			if(g_DuelSystem.IsEnabled())
			{
				if(User *pUser = pSocket->GetUser()->SafeCastUser())
				{
					if(pUser->pSD->DuelInfo.DuelID)
					{
						return g_DuelSystem.Withdraw(pUser);
					}
				}
			}
			return false;
			break;
		}
	default:
		{
			if(OpCode <= 0x13)
			{
				return UserSocketCallback(0x0086BE60L)(pSocket, orgPacket);
			}else
			{
				if(User *pUser = pSocket->GetUser())
				{
					g_Log.Add(CLog::Error, "[%s] Not Handled ExPacket[%d] user[%S]", __FUNCTION__, OpCode, pUser->pSD->wszName);
					return false;
				}else
				{
					return true;
				}
			}
			break;
		}
	};
	return false;
}

bool RequestExShowMPCCPartyMemberInfo(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT leaderId = 0;
		Disassemble(packet, "d", &leaderId);
		ret = CMPCC::ShowPartyMemberInfo(pUser, leaderId);
	}
	unguard;
	return ret;
}

bool CUserExPacket::RequestTargetGround(CUserSocket *pSocket, const unsigned char* packet)
{
	if(User *pUser = pSocket->GetUser())
	{
		int x, y, z;
		double x1 = pUser->pSD->Pos.x;
		double y1 = pUser->pSD->Pos.y;
		double z1 = pUser->pSD->Pos.z;
		int nSkillID, nLevel;
		packet = Disassemble(packet,"ddd", &x, &y, &z);
		Disassemble(packet, "d", &nSkillID);
		
		nLevel = pUser->GetAcquiredSkillLevel(nSkillID);
		if(nLevel < 1)
			return false;
		DWORD dReuseEnd = pUser->GetSkillReuse(nSkillID);
		if(dReuseEnd == 0 || GetTickCount() > dReuseEnd)
		{
			//calculating distance
			int nDist = sqrt((pow((x1-x), 2) + pow((y1-y), 2) + pow((z1-z), 2)));
			if(nDist <= 900)
			{
				pUser->pSD->pExData->TargetGroundLoc.nX = x;
				pUser->pSD->pExData->TargetGroundLoc.nY = y;
				pUser->pSD->pExData->TargetGroundLoc.nZ = z;
				return _SockFunc(0x87AEE0L)(pSocket, packet);
			}
			else
			{
				pSocket->SendSystemMessage(L"Your target is too far.");
				return false;
			}
		}else
			pSocket->SendSystemMessage(L"You cannot use that skill at the moment.");
	}
	return false;
}

bool CUserExPacket::AnswerJoinPartyRoom(CUserSocket *pSocket, const unsigned char *packet)
{
	if(User *pUser = pSocket->GetUser())
	{
		int Answer = 0;
		Disassemble(packet, "d", &Answer);
		if(Answer == 1)
		{
			if(pUser->pSD->pExData->requestJoinParyRoom.GetRequestor())
			{
				g_PartyWaitingList.JoinRoom(pUser, pUser->pSD->pExData->requestJoinParyRoom.GetRoomId());
			}
			pUser->pSD->pExData->requestJoinParyRoom.Clear();
		}
	}
	return false;
}

bool CUserExPacket::AskJoinPartyRoom(CUserSocket *pSocket, const unsigned char* packet)
{
	if(User *pUser = pSocket->GetUser())
	{
		WCHAR wTarget[48]; memset(wTarget, 0, sizeof(wTarget));
		Disassemble(packet, "S", 48, wTarget);
		if(true)
		{
			if(wcsnlen(wTarget, 20) > 0)
			{
				g_PartyWaitingList.AskJoinRoom(pUser, wTarget);
			}
		}
	}
	return false;
}

bool CUserExPacket::RequestExitPartyMatchingWaitingRoom(CUserSocket *pSocket, const unsigned char* packet)
{
	if(User *pUser = pSocket->GetUser())
	{
		g_PartyWaitingList.ClearList(pUser);
		g_PartyWaitingList.DelMember(pUser);
	}
	return false;
}

bool CUserExPacket::RequestListPartyMatchingWaitingRoom(CUserSocket *pSocket, const unsigned char* packet)
{
	if(User *pUser = pSocket->GetUser())
	{
		int page, minLevel, maxLevel, inRoom;
		Disassemble(packet, "dddd", &page, &minLevel, &maxLevel, &inRoom);
		g_PartyWaitingList.RequestList(pUser, page, minLevel, maxLevel, inRoom);
	}

	return false;
}
#include "StdAfx.h"
#include "PacketHandler.h"
#include "Geo.h"
#include "VIPSystem.h"
#include "BuildCmdHandler.h"
#include "OfflineShop.h"
#include "SocketDB.h"
#include "VoteReward.h"
#include "ObsceneFilter.h"
#include "TerritoryData.h"
#include "ClanSys.h"
#include "TeleportBypass.h"
#include "TvT.h"
#include "AIO.h"
#include "PrivateStore.h"
#include "PlayerAction.h"
#include "IpList.h"
#include "HTMLCacheManager.h"
#include "MultiSellStat.h"
#include "AccountDB.h"
#include "UserExCommand.h"
#include "Auction.h"
#include "HtmlFilter.h"
#include "OlympiadSystem.h"
#include "ObjectDB.h"
#include "MultiSell.h"
#include "Wedding.h"
#include "CastleSiegeManager.h"
#include "CliExt.h"
#include "LoginDb.h"
#include "CharacterLock.h"
#include "SpawnProtection.h"
#include "SkillMaster.h"
#include "ChatManager.h"
#include "DoorData.h"
#include "PIN.h"

static UserSocketCallback *PHandler;
static UserSocketCallback userPacketHandler[256];
extern UINT g_HeroVoiceDelay;
BYTE g_LastUsedPacket[26];
BYTE g_LastUsedPacketEx[26];

extern DWORD g_ItemPacketReuse;
extern double g_MaxValidateLocationDiff;
extern BOOL g_BlockStatisticPacket;
extern BOOL g_SafeListStatisticPacket;
extern INT32 g_MaxPartyLevelRange;
extern BOOL g_BlockPartyInvite;
extern BOOL g_GlobalChat;
extern BOOL g_PacketLog;
extern BOOL g_PetBlockTrade;
extern vector<INT32> g_PetAllowedItems;
extern INT32 g_ChatShoutLevel;

void PacketHandler::Initialize()
{
	g_HookManager.WriteCall(0x8771F5, PacketHandler::PacketAcceptable, 0);
	//Initialize packet handler
	for(int n=0;n<26;n++)
	{
		g_LastUsedPacket[n] = 0;
		g_LastUsedPacketEx[n] = 0;
	}

	PHandler = (UserSocketCallback*)0x0BF88D0;
	for(int n=0;n<255;n++)
	{
		if(n < 209)
		{
			userPacketHandler[n] = PHandler[n];
			PHandler[n] = HandleUserPacket;
		}
	}

	for(int n=0;n<255;n++)
	{
		if(userPacketHandler[n] == NULL)
		{
			userPacketHandler[n] = DummyPacket;
		}
	}

	InsertHandler(0x00, ProtocolVersion);
	InsertHandler(0x01, MoveToLocationPacket);
	InsertHandler(0x03, EnterWorld);
	InsertHandler(0x04, ActionPacket);
	InsertHandler(0x05, CustomUserPacket);
	InsertHandler(0x08, LoginPacket);
	InsertHandler(0x09, LogoutPacket);
	InsertHandler(0x0A, AttackPacket);
	InsertHandler(0x0B, CharacterCreatePacket);
	InsertHandler(0x0C, CharacterDeletePacket);
	InsertHandler(0x0D, CharSelectPacket);
	InsertHandler(0x12, DropItem);
	InsertHandler(0x13, GetItemPacket);
	InsertHandler(0x14, UseItemPacket);
	InsertHandler(0x15, TradeRequestPacket);
	InsertHandler(0x16, TradeAddItems);
	InsertHandler(0x17, TradeDonePacket);
	InsertHandler(0x1E, SellPacket);
	InsertHandler(0x1F, BuyPacket);
	InsertHandler(0x20, LinkHtml);
	InsertHandler(0x21, BypassToServerCMD);
	InsertHandler(0x24, RequestJoinPledge);
	InsertHandler(0x25, AnswerJoinPledge);
	InsertHandler(0x29, RequestJoinParty);
	InsertHandler(0x2A, AnswerJoinParty);
	InsertHandler(0x2F, MagicSkillUsePacket);
	InsertHandler(0x30, AppearingPacket);
	InsertHandler(0x31, WarehouseDepositPacket);
	InsertHandler(0x32, WarehouseWithdrawPacket);
	InsertHandler(0x38, SayPacket2);
	InsertHandler(0x44, TradeStartPacket);
	InsertHandler(0x45, IconAction);
	InsertHandler(0x46, RestartPacket);
	InsertHandler(0x48, ValidateLocationPacket);
	InsertHandler(0x4D, RequestStartPledgeWar);
	InsertHandler(0x4F, RequestStopPledgeWar);
	InsertHandler(0x59, DestroyItem);
	InsertHandler(0x5B, GMCommand);
	InsertHandler(0x62, RestoreCharacter);
	InsertHandler(0x6B, RequestAcquireSkillInfo);
	InsertHandler(0x6D, RestartPointPacket);
	InsertHandler(0x6C, RequestAcquireSkill);
	InsertHandler(0x6F, ReplyListPartyWaitingPacket);
	InsertHandler(0x74, PrivateStoreListSet);
	InsertHandler(0x79, PrivateStoreBuyListSend);
	InsertHandler(0x7B, TutorialLinkHTML);
	InsertHandler(0x7C, TutorialPassCmdToServer);
	InsertHandler(0x8B, GiveItemToPet);
	InsertHandler(0x8C, GetItemFromPet);
	InsertHandler(0x8F, PetGetItem);
	InsertHandler(0x91, PrivateStoreBuyListSet);
	InsertHandler(0x96, PrivateStoreBuyBuyListSend);
	InsertHandler(0x9E, RequestPackageSendableItemList);
	InsertHandler(0x9F, RequestPackageSend);
	InsertHandler(0xD0, UserExPacket);
	InsertHandler(0xA6, RequestSetCastleSiegeTimePacket);
	InsertHandler(0xA7, MultisellChoose);
	InsertHandler(0xA8, RequestNetPing);
	InsertHandler(0xAA, RequestUserCommand);
	InsertHandler(0xAF, RequestRecipeBookItemMake);
	InsertHandler(0xB2, RequestRecipeShopSetList);
	InsertHandler(0xC0, ManagePledgePower);
	InsertHandler(0xC1, RequestMakeMacro);
	InsertHandler(0xC5, ConfirmDialog);
	InsertHandler(0xCC, L2FriendSayPacket);
}

void PacketHandler::InsertHandler(int nID, UserSocketCallback func)
{
	if(nID < 255)
	{
		userPacketHandler[nID] = func;
	}
}

bool PacketHandler::HandleUserPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(pSocket && pSocket->pED)
	{
		BYTE opCode = packet[-1];
		UINT threadIndex = GetThreadIndex();
		if(threadIndex < 26)
		{
			g_LastUsedPacket[threadIndex] = opCode;
			g_LastUsedPacketEx[threadIndex] = 0;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid thread index[%d]", __FUNCTION__, threadIndex);
		}

		if(opCode < 255)
		{
			bool validCS = true;
			PacketCS& packetCS = pSocket->pED->packetCS;
			if(g_CliExt.IsEnabled() && opCode != 0)
			{
				if(packetCS.current != packetCS.calculated)
				{
					validCS = false;
					if(packetCS.current == packetCS.previous)
					{
						ret = false;	//drop
						g_Log.Add(CLog::Error, "[%s] Duplicated packet[%x] checksum[%u] - Account[%S]", __FUNCTION__, packetCS.current, pSocket->accountName);
					}else
					{
						ret = true;	//kick
						g_Log.Add(CLog::Error, "[%s] Calculated checksum[%u] doesn't match packet[%x] checksum[%u] - Account[%S]", __FUNCTION__, packetCS.calculated, opCode, packetCS.current, pSocket->accountName);
					}
				}
			}
			if(validCS)
			{
				ret = userPacketHandler[opCode](pSocket, packet);
				packetCS.previous = packetCS.current;
			}
			if(g_PacketLog)
			{
				g_Log.Add(CLog::Blue,"[%s] PacketId[%x] len[%d] ret[%d]", __FUNCTION__, opCode, pSocket->inPckSize, ret);
			}
		}

		if(threadIndex < 26)
		{
			g_LastUsedPacket[threadIndex] = 0;
			g_LastUsedPacketEx[threadIndex] = 0;
		}
	}
	unguard;
	return ret;
}

bool PacketHandler::PacketAcceptable(User *pUser, int packetId)
{
	guard;

	typedef bool (*f)(User*, int);
	bool ret = f(0x805FB0L)(pUser, packetId);
#ifdef EXTEND_IOBUFFER
	const unsigned char* packet = (const unsigned char*)&pUser->pSocket->pIncommingPacket->buffEx[2];
#else
	const unsigned char* packet = (const unsigned char*)&pUser->pSocket->pIncommingPacket->buff[2];
#endif
	if(packet)
	{
		if(packet[0] == 0xD0 && packet[1] == 0x06 && packet[2] == 0x00)
		{
			BYTE opCode = 0;
			WORD exOpCode = 0;
			int skillId = 0, level = 0;
			packet = Disassemble(packet, "chdd", &opCode, &exOpCode, &skillId, &level);
			if(skillId < 1 || level < 1)
			{
				ret = false;
			}
		}
	}

	unguard;
	return ret;
}

bool DummyPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	if(User *pUser = pSocket->GetUser())
	{
		g_Log.Add(CLog::Error, "[%s] by user[%S] id[%x]", __FUNCTION__, pUser->pSD->wszName, packet[-1]);
	}
	unguard;
	return true;
}

bool L2FriendSayPacket(CUserSocket *pSocket, const unsigned char *packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser()->SafeCastUser())
	{
		WCHAR wName[25] = { 0 };
		WCHAR wMessage[8190] = { 0 };
		Disassemble(packet, "SS", sizeof(wMessage), wMessage, sizeof(wName), wName);
		
		if(User *pTarget = g_UserDB.GetUserByName(wName))
		{
			if(pUser->IsFriend(pTarget))
			{
				ret = _SockFunc(0x883750L)(pSocket, packet);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] tried friend chat exploit to user [%S]!", __FUNCTION__, pUser->pSD->wszName, pTarget->pSD->wszName);
			}
		}else
		{
			pUser->SendSystemMessage(L"Your friend is offline.");
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool RequestMakeMacro(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->pUser->SafeCastUser())
	{
		UINT macroId = 0;
		WCHAR wName[1024];
		memset(wName, 0, sizeof(wName));
		WCHAR wDescription[1024];
		memset(wDescription, 0, sizeof(wDescription));
		WCHAR wAcronym[1024];
		memset(wAcronym, 0, sizeof(wAcronym));
		BYTE icon = 0, amount = 0;
		const unsigned char* pck = Disassemble(packet, "dSSScc", &macroId, sizeof(wName), wName, sizeof(wDescription), wDescription, sizeof(wAcronym), wAcronym, &icon, &amount); 
		
		if(!Parser::IsValidSqlString(wName))
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] used invalid sql string in macro name[%S]!", __FUNCTION__, pUser->pSD->wszName, wName);
			pUser->SendSystemMessage(L"Invalid macro name!");
			unguard;
			return false;
		}
		if(!Parser::IsValidSqlString(wDescription))
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] used invalid sql string in macro description[%S]!", __FUNCTION__, pUser->pSD->wszName, wDescription);
			pUser->SendSystemMessage(L"Invalid macro description!");
			unguard;
			return false;
		}
		if(!Parser::IsValidSqlString(wAcronym))
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] used invalid sql string in macro acronym[%S]!", __FUNCTION__, pUser->pSD->wszName, wAcronym);
			pUser->SendSystemMessage(L"Invalid macro acronym!");
			unguard;
			return false;
		}
		if(amount < 13)
		{
			for(BYTE n=0;n<amount;n++)
			{
				BYTE entryId = 0, type = 0;
				UINT d1 = 0;
				BYTE d2 = 0;
				WCHAR wCommand[1024];
				memset(wCommand, 0, sizeof(wCommand));
				pck = Disassemble(pck, "ccdcS", &entryId, &type, &d1, &d2, sizeof(wCommand), wCommand);
				if(!Parser::IsValidSqlString(wCommand))
				{
					g_Log.Add(CLog::Error, "[%s] User[%S] used invalid sql string in macro command[%S]!", __FUNCTION__, pUser->pSD->wszName, wCommand);
					pUser->SendSystemMessage(L"Invalid macro command!");
					unguard;
					return false;
				}
			}
			
			ret = _SockFunc(0x869FE0L)(pSocket, packet);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] reached macro command amount limit[%d]!", __FUNCTION__, pUser->pSD->wszName, amount);
		}
	}

	unguard;
	return ret;
}

bool CharacterDeletePacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	
	UINT index = 0;
	Disassemble(packet, "d", &index);
	if(index < 7 )
	{
		if(g_CliExt.IsEnabled())
		{
			HWID hwidC = g_AccountDB.GetLockedCharHwid(pSocket->characterDBID[index]);
			HWID hwidA = g_AccountDB.GetLockedHwid(pSocket->accountId);
			if(hwidC.IsNull() || pSocket->pED->longHWID.IsEqual(hwidC))
			{
				if(hwidA.IsNull() || pSocket->pED->longHWID.IsEqual(hwidA))
				{
					ret = _SockFunc(0x85F720L)(pSocket, packet);
				}else
				{
					ret = false;
					pSocket->lastUsedPacketID = 0x0E;
					pSocket->Send("cdSd", 0xF, 0, g_CliExt.GetLockedAccountHtml(), 0);
				}
			}else
			{
				ret = false;
				pSocket->lastUsedPacketID = 0x0E;
				pSocket->Send("cdSd", 0xF, 0, g_CliExt.GetLockedCharacterHtml(), 0);
			}
		}else
		{
			ret = _SockFunc(0x85F720L)(pSocket, packet);
		}
	}
	unguard;
	return ret;
}


bool CharacterCreatePacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	
	if(g_CliExt.IsEnabled())
	{
		HWID hwidA = g_AccountDB.GetLockedHwid(pSocket->accountId);
		if(!hwidA.IsNull() && !pSocket->pED->longHWID.IsEqual(hwidA))
		{
			pSocket->lastUsedPacketID = 0x0E;
			pSocket->Send("cdSd", 0xF, 0, g_CliExt.GetLockedAccountHtml(), 0);
			unguard;
			return false;
		}
	}

	bool endLine = false;
	for(UINT n=0;n<34;n++)
	{
		if(packet[n] == 0 && packet[n+1] == 0)
		{
			endLine = true;
			break;
		}
	}

	if(endLine)
	{
		ret = _SockFunc(0x85EDC0L)(pSocket, packet);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid name - ip[%d.%d.%d.%d]!", __FUNCTION__, pSocket->addr.S_un.S_un_b.s_b1, pSocket->addr.S_un.S_un_b.s_b2, pSocket->addr.S_un.S_un_b.s_b3, pSocket->addr.S_un.S_un_b.s_b4);
	}

	unguard;
	return ret;
}


bool RestoreCharacter(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	
	UINT slot = 0;
	Disassemble(packet, "d", &slot);

	if(slot < 8)
	{
		ret = _SockFunc(0x8651A0L)(pSocket, packet);
	}

	unguard;
	return ret;
}

bool AppearingPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	
	if(User *pUser = pSocket->pUser->SafeCastUser())
	{
		if(g_SpawnProtection.IsEnabled())
		{
			if(pUser->pED->spawnProtection == 1)
			{
				g_SpawnProtection.ActivateProtection(pUser);
			}
		}
		ret = _SockFunc(0x861690L)(pSocket, packet);
	}

	unguard;
	return ret;
}

bool ValidateLocationPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = false;
	if(User *pUser = pSocket->GetUser())
	{
		INT32 x = 0, y = 0, z = 0, data = 0;
		UINT heading = 0;
		Disassemble(packet, "ddddd", &x, &y, &z, &heading, &data);

		if(g_MaxValidateLocationDiff > 0)
		{
			double _x = static_cast<double>(x) - pUser->pSD->Pos.x;
			double _y = static_cast<double>(y) - pUser->pSD->Pos.y;
			double diff = _x*_x + _y*_y;

			//g_Log.Add(CLog::Blue, "[%s] %d(%f) %d(%f) %d(%f) %d %d diff: %f tp tick: %d current: %d", __FUNCTION__, x ,pUser->pSD->Pos.x, y, pUser->pSD->Pos.y, z, pUser->pSD->Pos.z, heading, data, diff, pUser->pSD->pExData->teleportTick, GetTickCount());

			if(diff > g_MaxValidateLocationDiff)
			{
				DWORD currentTick = GetTickCount();
				//check for teleport tick
				if(pUser->pSD->pExData->teleportTick + 5000 > currentTick)
				{
					ret = _SockFunc(0x8788A0L)(pSocket, packet);
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Dropped - user[%S] %d(%f) %d(%f) %d(%f) %d %d diff: %f tp tick: %d current: %d", __FUNCTION__, pUser->pSD->wszName, x ,pUser->pSD->Pos.x, y, pUser->pSD->Pos.y, z, pUser->pSD->Pos.z, heading, data, diff, pUser->pSD->pExData->teleportTick, GetTickCount());
					ret = false;
				}
			}
		}else
		{
			ret = _SockFunc(0x8788A0L)(pSocket, packet);
		}
	}

	unguard;
	return ret;
}

bool TutorialPassCmdToServer(CUserSocket *pSocket, const unsigned char* packet)
{
	CTL;
	bool ret = true;

	WCHAR wBypass[2048] = { 0 };

	Disassemble(packet, "S", sizeof(wBypass), wBypass);

	//handle pin system bypasses
	if(g_PINSystem.IsEnabled() && pSocket->pUser == 0 && pSocket->pED->selectedCharId)
	{
		wstring wTxt(wBypass);
		if(wTxt.find(L"pin_") == 0)
		{
			ret = g_PINSystem.RequestHandleBypass(pSocket, wTxt);
		
			return ret;
		}
	}

	if(User *pUser = pSocket->GetUser())
	{
		if(g_HtmlFilter.IsValid(pUser, wBypass, false, true))
		{
			ret = _SockFunc(0x892580L)(pSocket, packet);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] sent invalid tutorial bypass [%S]", __FUNCTION__, pUser->pSD->wszName, wBypass);
			ret = false;	//drop the packet
		}
	}

	return ret;
}

bool TutorialLinkHTML(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;

	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		WCHAR wLink[1024];
		Disassemble(packet, "S", sizeof(wLink), wLink);
		if(g_HtmlFilter.IsValid(pUser, wLink, true, true))
		{
			ret = _SockFunc(0x871510L)(pSocket, packet);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] sent invalid tutorial link[%S]", __FUNCTION__, pUser->pSD->wszName, wLink);
			ret = false; //drop the packet
		}
	}

	unguard;
	return ret;
}

bool CustomUserPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(g_CliExt.IsEnabled())
	{
		ret = g_CliExt.HandleCustomPacket(pSocket, packet);
	}
	unguard;
	return ret;
}

bool ReplyListPartyWaitingPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		g_PartyWaitingList.AddMember(pUser);
		ret = _SockFunc(0x876560L)(pSocket, packet);
	}

	unguard;
	return ret;
}

bool RequestNetPing(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = UserSocketCallback(0x8686E0L)(pSocket, packet);
	unguard;
	return ret;
}

bool MoveToLocationPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	CTL;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->pED->isLocked)
		{
			return false;
		}

		if(pUser->pED->spawnProtection)
		{
			if(g_SpawnProtection.IsDeactivateOnMoving() || pUser->pED->spawnProtectionTimeout == 0)
			{
				g_SpawnProtection.DeactivateProtection(pUser);
			}
		}

		INT32 x, y, z, originX, originY, originZ;
		UINT type;
		Disassemble(packet, "ddddddd", &x, &y, &z, &originX, &originY, &originZ, &type);
//		g_Log.Add(CLog::Blue, "[%s] x[%d] y[%d] z[%d] ox[%d] oy[%d] oz[%d] type[%d] current(%d, %d, %d)", __FUNCTION__, x, y, z, originX, originY, originZ, type, (int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z );

		if(g_DoorData.IsEnabled())
		{
			FVector pos(x, y, z);
			if(!g_DoorData.CanSee(pUser->pSD->Pos, pos))
			{
			//	g_Log.Add(CLog::Blue, "[%s] cannot see!", __FUNCTION__);
				pUser->ActionFailed();
				return false;
			}
		}

		ret = UserSocketCallback(0x0085E1D0L)(pSocket, packet);
	}

	return ret;
}

bool ConfirmDialog(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		int nMsgID, nAnswer;
		Disassemble(packet, "dd", &nMsgID, &nAnswer);
		if(nMsgID == 1842)
		{
			//Summon friend
			if(nAnswer == 1)
			{
				if(pUser->pSD->pExData->SummonFriend.nSummonerID)
				{
					if(pUser->olympiadUser.GetState() == 0)
					{
						if(pUser->GetInventory()->HaveItemByClassID(8615, 1))
						{
							pUser->DeleteItemInInventory(8615, 1);
							pUser->StopMove();
							int x = pUser->pSD->pExData->SummonFriend.nX;
							int y = pUser->pSD->pExData->SummonFriend.nY;
							int z = pUser->pSD->pExData->SummonFriend.nZ;
							PlayerAction::OnTeleportToLocation( pUser,  x, y, CGeo::GetInstance()->GetZLoc(x, y, z), false);
							pUser->pSD->pExData->SummonFriend.Clear();
						}else
						{
							pUser->SendSystemMessage(L"Summoning Crystal cannot be found.");
							pUser->pSD->pExData->SummonFriend.Clear();
						}
					}else
					{
						pUser->SendSystemMessage(L"You cannot be summoned while participing in Olympiad Games!");
						pUser->pSD->pExData->SummonFriend.Clear();
					}
				}
				
			}else
			{
				pUser->pSD->pExData->SummonFriend.Clear();
			}
			ret = false;
		}else if( nMsgID == CTeleportBypass::MsgID)
		{
			if(nAnswer == 1)
			{
				g_TeleportBypass.Teleport(pUser, pUser->pSD->pExData->TeleportBypassData.GetName());
				pUser->pSD->pExData->TeleportBypassData.Clear();
			}else
			{
				pUser->pSD->pExData->TeleportBypassData.Clear();
			}
			ret = false;
		}else if( nMsgID == 2461)
		{
			g_Wedding.EngageReply(pUser, nAnswer);
			ret = false;
		}else if( nMsgID == 2462)
		{
			g_Wedding.MarryReply(pUser, nAnswer);
			ret = false;
		}else if( nMsgID == 2463)
		{
			g_Wedding.DivorceReply(pUser, nAnswer);
			ret = false;
		}else
		{
			ret = _SockFunc(0x86B030L)(pSocket, packet);
		}
	}

	unguard;
	return ret;
}

bool RequestAcquireSkillInfo(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		int nSkillID = 0, nLevel = 0, nType = 0;
		Disassemble(packet, "ddd", &nSkillID, &nLevel, &nType);

		if(nSkillID > 0 && nLevel > 0)
		{
			if(nType ==  2)
			{	//Pledge Skill
				ret = ClanSys::AcquireSkillInfo(pSocket, nSkillID, nLevel);
			}else
			{
				if(nType == g_SkillMaster.m_AcquireSkillMode && pUser->pED->skillMasterFlag)
				{
					g_SkillMaster.ShowSkillInfo(pUser, nSkillID, nLevel, pUser->pED->skillMasterFlag);
					ret = false;
				}else
				{
					ret = UserSocketCallback(0x8658D0L)(pSocket, packet);
				}
			}
		}else
		{
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool ActionPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		//format ddddc
		int ObjectID = 0, x = 0, y = 0, z = 0;
		bool WithShift = false;
		Disassemble(packet, "ddddc", &ObjectID, &x, &y, &z, &WithShift);

		if(pUser->pED->isLocked)
		{
			pUser->SendSystemMessage(L"Your character is locked, please enter PIN code:");
			unguard;
			return false;
		}

		CItem *pItem = (CItem*)CObject::GetObjectBySID(ObjectID);
		if(pItem->IsValidItem())
		{
			UINT currentTick = GetTickCount();
			if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
			{
				pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
				ret = _SockFunc(0x860BB0L)(pSocket, packet);
			}else
			{
				pUser->ActionFailed();
				ret = false;
			}
		}else
		{
			if(WithShift)
			{
				if(pUser->pED->spawnProtection)
					g_SpawnProtection.DeactivateProtection(pUser);
			}

			//check if can see
			if(ObjectID && pUser->targetId == ObjectID)
			{
				if(g_DoorData.IsEnabled() && g_DoorData.IsAffectCloseCombat())
				{
					if(CCreature *pCreature = CObject::GetObjectBySID(ObjectID)->SafeCastCreature())
					{
						if(!g_DoorData.CanSee(pUser->pSD->Pos, pCreature->pSD->Pos))
						{
							//Cannot see target
							pUser->pSocket->SendSystemMessage(181);
							pUser->ActionFailed();
						
							return false;
						}
					}
				}
			}

			ret = _SockFunc(0x860BB0L)(pSocket, packet);
		}
	}

	unguard;
	return ret;
}

bool AnswerJoinParty(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->pSD->nCursedOwner == 0  && pUser->olympiadUser.IsInside() == FALSE && pUser->pSD->DuelInfo.DuelID == 0 && pUser->pSD->activateOfflineShopTime == 0 && pUser->pSD->pExData->territoryData.territoryId[NO_PARTY_ZONE] == 0)
		{
			ret =  _SockFunc(0x0088A860L)(pSocket, packet);
		}else
		{
			pSocket->SendSystemMessage(L"You cannot join any party at the moment!");
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool AnswerJoinPledge(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		ret = ClanSys::AnswerJoinPledgePacket(pSocket, packet);
	}

	unguard;
	return ret;
}

bool AttackPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->pED->isLocked)
		{
			unguard;
			return false;
		}

		if(pUser->pED->spawnProtection)
		{
			g_SpawnProtection.DeactivateProtection(pUser);
		}

		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
			ret = false;
		}else
		{
			UINT objectId = 0;
			int x = 0, y = 0, z = 0;
			BYTE type = 0;
			Disassemble(packet, "ddddc", &objectId, &x, &y, &z, &type);
			if(g_DoorData.IsEnabled())
			{
				if(CCreature *pTarget = CObject::GetObjectBySID(objectId)->SafeCastCreature())
				{
					if(!g_DoorData.CanSee(pUser->pSD->Pos, pTarget->pSD->Pos))
					{
						pUser->ActionFailed();
						pUser->pSocket->SendSystemMessage(181);
					
						return false;
					}
				}
			}

			ret = _SockFunc(0x0085FD50L)(pSocket, packet);
		}
	}

	unguard;
	return ret;
}

bool SellPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
		
			if(pUser->pED->isLocked)
			{
				unguard;
				return false;
			}

			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
				pUser->ActionFailed();
				unguard;
				return false;
			}

			if(pUser->pSD->nStoreMode > 0)
			{
				pUser->SendSystemMessage(L"You cannot perform this action while operating a private store!");
				pUser->ActionFailed();
				unguard;
				return false;
			}

			UINT listId = 0;
			UINT entryCount = 0;
			const unsigned char *pck = Disassemble(packet, "dd", &listId, &entryCount);

			if(entryCount == 0 || entryCount > 90)
			{
				pUser->SendSystemMessage(L"You cannot sell more than 90 items at once!");
				pUser->ActionFailed();
				unguard;
				return false;
			}

			vector<INT32> stackableItems;
			vector<UINT> objectIds;
			INT64 totalPrice = 0;

			for(UINT n=0;n<entryCount;n++)
			{
				UINT objectId = 0;
				INT32 itemId = 0, itemAmount = 0;
				pck = Disassemble(pck, "ddd", &objectId, &itemId, &itemAmount);

				if(itemAmount < 1 || itemAmount >= INT_MAX)
				{
					//347	1	a,Incorrect item count.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
					pSocket->SendSystemMessage(347);
					pUser->ActionFailed(); 
					unguard;
					return false;
				}

				//check if user has item in inventory and amount matches
				if(CItem *pItem = pUser->inventory.GetItemBySID(objectId)->SafeCastItem())
				{
					if(pItem->pSID->nItemAmount < itemAmount)
					{
						//347	1	a,Incorrect item count.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
						pSocket->SendSystemMessage(347);
						pUser->ActionFailed(); 
						unguard;
						return false;
					}

					if(pItem->pSID->nConsumeType == ConsumeStackable)
					{
						for(UINT m=0;m < stackableItems.size(); m++)
						{
							if(stackableItems[m] == itemId)
							{
								g_Log.Add(CLog::Error, "[%s] Duplicated stackable item[%d] - User[%S]!", __FUNCTION__, itemId, pUser->pSD->wszName);
								pUser->ActionFailed();
								unguard;
								return false;
							}
						}
						stackableItems.push_back(itemId);
					}
					for(UINT m=0;m<objectIds.size();m++)
					{
						if(objectIds[m] == objectId)
						{
							g_Log.Add(CLog::Error, "[%s] Duplicated objectId item[%d] - User[%S]!", __FUNCTION__, itemId, pUser->pSD->wszName);
							pUser->ActionFailed();
							unguard;
							return false;
						}
					}
					objectIds.push_back(objectId);

					//adena int overflow check
					INT64 price = static_cast<INT64>(pItem->pII->defaultPrice / 2);
					price *= itemAmount;
					totalPrice += price;
					INT64 total = totalPrice + static_cast<INT64>(pUser->inventory.GetAdenaAmount());
					if(total >= INT_MAX)
					{
						g_Log.Add(CLog::Error, "[%s] Price INT Overflow - User[%S]", __FUNCTION__, pUser->pSD->wszName);
						//348	1	a,Incorrect item price.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
						pSocket->SendSystemMessage(348);
						pUser->ActionFailed();
						unguard;
						return false;
					}
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Invalid item serverId[%d] - User[%S]", __FUNCTION__, objectId, pUser->pSD->wszName);
					pUser->ActionFailed();
					unguard;
					return false;
				}
			}

			ret = _SockFunc(0x879F60L)(pSocket, packet);
		}else
		{
			ret = false;	//drop
			pUser->ActionFailed();
		}
	}
	unguard;
	return ret;
}

bool BuyPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;

			if(pUser->pED->isLocked)
			{
				unguard;
				return false;
			}

			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
				pUser->ActionFailed();
				unguard;
				return false;
			}
		
			UINT nListID = 0, nBuyListSize = 0;
			const unsigned char* p = Disassemble(packet, "dd", &nListID, &nBuyListSize);
			if(nBuyListSize > 60 || nBuyListSize == 0)
			{
				pUser->SendSystemMessage(L"You cannot buy more than 60 items at once.");
				pUser->ActionFailed();
				unguard;
				return false;
			}
			INT64 totalPrice = 0;
			vector<INT32> stackableItems;
			for(UINT i=0; i < nBuyListSize; i++)
			{
				int nItemID = 0, nItemAmount = 0;
				p = Disassemble(p, "dd", &nItemID, &nItemAmount);
				if(nItemID < 1 || nItemID > 30000 || nItemID == 57) //Max item class id
				{
					g_Log.Add(CLog::Error, "[%s] Invalid ItemID[%d] Player[%S]", __FUNCTION__, nItemID, pUser->pSD->wszName);
					pUser->ActionFailed();
					unguard;
					return false;
				}
				
				if(nItemAmount < 1)
				{
					g_Log.Add(CLog::Error, "[%s] Invalid Item Amount[%d] Player[%S]", __FUNCTION__, nItemAmount, pUser->pSD->wszName);
					//347	1	a,Incorrect item count.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
					pSocket->SendSystemMessage(347);
					pUser->ActionFailed();
					unguard;
					return false;
				}

				//check for adena overflow
				if(CItem *pItem = g_ObjectDB.GetTemplateObject(nItemID)->SafeCastItem())
				{
					INT64 price = static_cast<INT64>(pItem->pII->defaultPrice);
					price *= nItemAmount;
					totalPrice += price;
					if(totalPrice >= INT_MAX)
					{
						g_Log.Add(CLog::Error, "[%s] INT overflow in price - User[%S]", __FUNCTION__, pUser->pSD->wszName);
						//348	1	a,Incorrect item price.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
						pSocket->SendSystemMessage(348);
						pUser->ActionFailed();
						unguard;
						return false;
					}
					if(pItem->pSID->nConsumeType == ConsumeStackable)
					{
						for(UINT n=0;n<stackableItems.size();n++)
						{
							if(stackableItems[n] == nItemID)
							{
								g_Log.Add(CLog::Error, "[%s] Duplicated stackable item[%d] - User[%S]!", __FUNCTION__, nItemID, pUser->pSD->wszName);
								pUser->ActionFailed();
								unguard;
								return false;
							}
						}
						stackableItems.push_back(nItemID);
					}
				}else
				{
					pUser->ActionFailed();
					unguard;
					return false;
				}
			}

			ret = _SockFunc(0x0087D230L)(pSocket, packet);
		}else
		{
			pUser->ActionFailed();
			ret = false;	//drop packet
		}
	}

	unguard;
	return ret;
}

bool LinkHtml(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		WCHAR wLink[1024];
		memset(wLink, 0, sizeof(wLink));
		Disassemble(packet, "S", sizeof(wLink), wLink);
		if(g_HtmlFilter.IsValid(pUser, wLink, true))
		{
			ret = _SockFunc(0x894370L)(pSocket, packet);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] sent invalid html link[%S]", __FUNCTION__, pUser->pSD->wszName, wLink);
			ret = false; //drop packet
		}
	}

	unguard;
	return ret;
}

bool BypassToServerCMD(CUserSocket *pSocket, const unsigned char* packet)
{
	CTL;
	bool ret = true;

	WCHAR wReply[1024] = { 0 };
	Disassemble(packet, "S", sizeof(wReply), wReply);
	
	//handle pin system bypasses
	if(g_PINSystem.IsEnabled() && pSocket->pUser == 0 && pSocket->pED->selectedCharId)
	{
		wstring wTxt(wReply);
		if(wTxt.find(L"pin_") == 0)
		{
			ret = g_PINSystem.RequestHandleBypass(pSocket, wTxt);
		
			return ret;
		}
	}

	if(User *pUser = pSocket->GetUser())
	{
		if(!pUser->pED->isLocked)
		{
			wstring wTxt(wReply);
			
		//	g_Log.Add(CLog::Blue, "[%s] [%S]", __FUNCTION__, wReply);
			if(g_HtmlFilter.IsValid(pUser, wReply))
			{
		//		g_Log.Add(CLog::Blue, "[%s] [%S] 2", __FUNCTION__, wReply);
				pSocket->pED->itemActionTimestamp = GetTickCount() + g_ItemPacketReuse;

				if(pUser->IsNowTrade())
				{
					pUser->TradeCancel();
					ret = false;
				}else
				{
					if( CBypass::Handled(pUser, wTxt) )
					{
						ret = false;
					}else
					{
						ret = _SockFunc(0x008948A0L)(pSocket, packet);
					}
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] sent invalid html bypass[%S] !", __FUNCTION__, pUser->pSD->wszName, wReply);
				ret = false; //drop packet
			}
		}else
		{
			ret = false;
		}
	}
	return ret;
}

bool CharSelectPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	CTL;
	bool ret = false;

	UINT charIndex = 0;
	Disassemble(packet, "d", &charIndex);
	if(charIndex < 7)
	{
		pSocket->pED->selectedCharId = pSocket->characterDBID[charIndex];
		if(g_PINSystem.IsEnabled() && g_AccountDB.GetLockedHwid(pSocket->accountId).IsNull() && g_AccountDB.GetLockedCharHwid(pSocket->characterDBID[charIndex]).IsNull())
		{
			bool kick = false;
			if(!g_PINSystem.CanEnter(pSocket, kick))
			{
				pSocket->lastUsedPacketID = 0x0E;
			
				return kick;
			}
		}

		if(g_CliExt.IsEnabled())
		{
			if(pSocket->pED->shortHWID > 0)
			{
				//check md5 checksum
				const LPBYTE pMD5Base = g_CliExt.GetMD5CHecksum();
				if(pMD5Base[0] != 0)
				{
					if(pSocket->pED->md5Checksum[0] != 0)
					{
						if(memcmp(pMD5Base, pSocket->pED->md5Checksum, 32))
						{
							pSocket->lastUsedPacketID = 0x0E;
							pSocket->Send("cdSd", 0xF, 0, g_CliExt.GetOutdatedClientHtml(), 0);
							return false;
						}
					}else
					{
						return false;
					}
				}

				if(!g_CliExt.IsAllowed(pSocket->pED->longHWID.hash))
				{
					pSocket->lastUsedPacketID = 0x0E;
					WCHAR html[0x4000] = { 0 };
					g_CliExt.GetAllowedHWIDHtml(pSocket->pED->longHWID, html);
					pSocket->Send("cdSd", 0xF, 0, html, 0);
					return false;
				}

				if(g_CliExt.IsBanned(pSocket->pED->longHWID.hash) == false)
				{
					//check locked acc/char
					HWID hwidA = g_AccountDB.GetLockedHwid(pSocket->accountId);
					if(!hwidA.IsNull() && !pSocket->pED->longHWID.IsEqual(hwidA))
					{
						pSocket->lastUsedPacketID = 0x0E;
						pSocket->Send("cdSd", 0xF, 0, g_CliExt.GetLockedAccountHtml(), 0);
						return false;
					}
					//check locked acc/char
					HWID hwidC = g_AccountDB.GetLockedCharHwid(pSocket->characterDBID[charIndex]);
					if(!hwidC.IsNull() && !pSocket->pED->longHWID.IsEqual(hwidC))
					{
						pSocket->lastUsedPacketID = 0x0E;
						pSocket->Send("cdSd", 0xF, 0, g_CliExt.GetLockedCharacterHtml(), 0);
						return false;
					}
					if(g_CliExt.GetBoxLimit() == 0 || g_AccountDB.IsAdditionalBox(pSocket->accountId) || g_CliExt.GetBoxLimit() >= g_SocketDB.GetHWIDCount(pSocket->pED->longHWID.hash))
					{
						if(g_CharacterLock.IsEnabled())
						{
							if(g_CharacterLock.IsBanned(pSocket->characterDBID[charIndex], pSocket->pED->longHWID))
							{
								pSocket->lastUsedPacketID = 0x0E;
								pSocket->Send("cdSd", 0xF, 0, g_CharacterLock.GetBanHTML(), 0);
								return false;
							}
						}

						ret = _SockFunc(0x85E9B0L)(pSocket, packet);
					}else
					{
						pSocket->lastUsedPacketID = 0x0E;
						pSocket->Send("cdSd", 0xF, 0, g_CliExt.GetBoxLimitHtml(), 0);
						ret = false;
					}
				}else
				{
					pSocket->lastUsedPacketID = 0x0E;
					pSocket->Send("cdSd", 0xF, 0, g_CliExt.GetBannedHtml(), 0);
					ret = false;
				}
			}else
			{
				pSocket->lastUsedPacketID = 0x0E;
				ret = false;
			}
		}else
		{
			if(pSocket->lastUsedPacketID != 0x0D)
			{
				ret = _SockFunc(0x85E9B0L)(pSocket, packet);
			}
		}
	}
	return ret;
}

bool UseItemPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;

		if(pUser->pED->isLocked)
		{
			pUser->SendSystemMessage(L"Your character is locked, please enter PIN code:");
			unguard;
			return false;
		}
		UINT serverId = 0;
		UINT force = 0;

		Disassemble(packet, "dd", &serverId, &force);

		if(pUser->pED->auctionUser.createTimeout > time(0))
		{
			g_Auction.HandleSelectedItem(pUser, serverId);
			unguard;
			return false;
		}
		
		ret = _SockFunc(0x86C8E0L)(pSocket, packet);
	}
	unguard;
	return ret;
}

bool GetItemPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;

			UINT serverId = 0;
			INT32 count = 0;

			Disassemble(packet, "dd", &serverId, &count);
			
			if(count < 1)
			{
				pUser->ActionFailed();
				ret = false;
			}else
			{
				ret = _SockFunc(0x86D5C0L)(pSocket, packet);
			}
		}else
		{
			ret = false;
		}
	}
	unguard;
	return ret;
}


bool DestroyItem(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
			
			if(pUser->pED->isLocked)
			{
				unguard;
				return false;
			}

			int nObjectID, nCount;
			Disassemble(packet, "dd", &nObjectID, &nCount);
			if(nCount > 0)
			{
				CItem *pItem = pUser->inventory.GetItemBySID(nObjectID);
				if(pItem->IsValidItem())
				{
					ret = _SockFunc(0x86FC00L)(pSocket, packet);
				}else
				{
					pSocket->SendSystemMessage(L"Cannot find item in your inventory!");
					ret = false;
				}
			}else
			{
				ret = false;
			}
		}else
		{
			ret = false;
		}
	}
	unguard;
	return ret;
}


bool DropItem(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			
			if(pUser->pED->isLocked)
			{
				pUser->SendSystemMessage(L"Your character is locked, please enter PIN code:");
				unguard;
				return false;
			}

			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
			if(pUser->pED->spawnProtection)
			{
				g_SpawnProtection.DeactivateProtection(pUser);
			}

			int nObjectID = 0, nCount = 0;
			Disassemble(packet, "dd", &nObjectID, &nCount);
			if(nCount > 0)
			{
				CItem *pItem = pUser->inventory.GetItemBySID(nObjectID);
				if(pItem->IsValidItem() && pItem->nAugmentationID)
				{
					if(pItem->nProtectionTimeout > time(0))
					{
						pSocket->SendSystemMessage(L"This item is protected and cannot be dropped!");
					}else
					{
						pSocket->SendSystemMessage(L"You cannot drop this item.");
					}
					ret = false;
				}else
				{
					ret = _SockFunc(0x0086CE20L)(pSocket, packet);
				}
			}
		}else
		{
			ret = false;	//drop packet
		}
	}

	unguard;
	return ret;
}

bool EnterWorld(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = false;

	ret = _SockFunc(0x888510L)(pSocket, packet);

	unguard;
	return ret;
}

bool GetItemFromPet(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;

			if(pUser->pED->isLocked)
			{
				unguard;
				return false;
			}

			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
				ret = false;
			}else if(pUser->pSD->nStoreMode != 0)
			{
				ret = false;
			}else
			{
				UINT nObjectID = 0;
				int nCount = 0;
				Disassemble(packet,"dd", &nObjectID, &nCount);

				if(CPet *pPet = pUser->GetPet())
				{
					if(CItem *pItem = pPet->inventory.GetItemBySID(nObjectID)->SafeCastItem())
					{
						if(pItem->pSID->nItemAmount >= nCount && nCount > 0 && nCount < INT_MAX)
						{
							//check for user item int overflow
							if(pItem->pSID->nConsumeType == 2 || pItem->pSID->nConsumeType == 3)
							{
								if(CItem *pOItem = pUser->inventory.GetFirstItemByClassID(pItem->pSID->nItemID))
								{
									INT64 nTotalAmount = pOItem->pSID->nItemAmount;
									nTotalAmount += nCount;
									if(nTotalAmount >= INT_MAX)
									{
										g_Log.Add(CLog::Error, "[%s] INT Overflow item[%d] requested amount[%d] owner[%S] has[%d]!", __FUNCTION__, pItem->pSID->nItemID, nCount, pUser->pSD->wszName, pOItem->pSID->nItemAmount);
										unguard;
										return false;
									}
								}
							}
							
							ret = _SockFunc(0x008727F0L)(pSocket, packet);
						}else
						{
							ret = false;	//drop packet
						}
					}else
					{
						pSocket->SendSystemMessage(L"Cannot find item in pet's inventory!");
						ret = false;
					}
				}else
				{
					ret = false;
				}
			}
		}else
		{
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool GiveItemToPet(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
			if(pUser->pED->isLocked)
			{
				unguard;
				return false;
			}
			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
				ret = false;
			}else
			{
				UINT nObjectID = 0;
				int nCount = 0;
				Disassemble(packet,"dd", &nObjectID, &nCount);
				if(nCount < INT_MAX && nCount > 0)
				{
					if(CPet* pPet = pUser->GetPet())
					{
						if(CItem *pItem = pUser->inventory.GetItemBySID(nObjectID)->SafeCastItem())
						{
							bool allowed = true;
							if(g_PetAllowedItems.size() > 0)
							{
								allowed = false;
							}
							for(UINT n=0;n< g_PetAllowedItems.size();n++)
							{
								if(pItem->pSID->nItemID == g_PetAllowedItems[n])
								{
									allowed = true;
									break;
								}
							}

							if(pItem->nAugmentationID || pItem->nManaLeft || pItem->nProtectionTimeout > time(0) || !allowed)
							{
								//2502	1	a,You cannot give this item to pet!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pSocket->SendSystemMessage(2502);
								unguard;
								return false;
							}

							if(pItem->pSID->nItemAmount >= nCount)
							{
								//check for int overflow
								if(pItem->pSID->nConsumeType == 2 || pItem->pSID->nConsumeType == 3)
								{
									if(CItem *pOItem = pPet->inventory.GetFirstItemByClassID(pItem->pSID->nItemID))
									{
										INT64 nTotalAmount = pOItem->pSID->nItemAmount;
										nTotalAmount += nCount;
										if(nTotalAmount >= INT_MAX)
										{
											g_Log.Add(CLog::Error, "[%s] INT Overflow item[%d] requested amount[%d] owner[%S] pet has[%d]!", __FUNCTION__, pItem->pSID->nItemID, nCount, pUser->pSD->wszName, pOItem->pSID->nItemAmount);
											unguard;
											return false;
										}
									}
								}
								
								ret = _SockFunc(0x00872380L)(pSocket, packet);
							}else
							{
								ret = false;
							}
						}else
						{
							ret = false;
						}
					}else
					{
						ret = false;
					}
				}else
				{
					pSocket->SendSystemMessage(L"You cannot give as many items to pet!");
					ret = false;
				}
			}
		}else
		{
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool GMCommand(CUserSocket *pSocket, const unsigned char* packet) //SysCMD2Packet
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->pSD->nBuilder)
		{
			WCHAR wsCommand[2000];
			Disassemble(packet, "S", sizeof(wsCommand), wsCommand);

			if(CBuilderCommand::Handle(pUser, wsCommand))
			{
				ret = _SockFunc(0x00864B40L)(pSocket, packet);
			}else
			{
				ret = false;
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Not GM player try to use GM Command- user[%S]", __FUNCTION__, pUser->pSD->wszName);
			ret = true;
		}
	}
	unguard;
	return ret;
}

bool IconAction(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
			ret = false;
		}else
		{
			if(pUser->pED->isLocked)
			{
				pUser->SendSystemMessage(L"Your character is locked, please enter PIN code:");
				unguard;
				return false;
			}
			if(pUser->pED->spawnProtection)
			{
				g_SpawnProtection.DeactivateProtection(pUser);
			}

			int nActionID = 0;
			Disassemble(packet, "d", &nActionID);

			if(nActionID == 19)
			{
				UINT currentTick = GetTickCount();
				if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
				{
					pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
				}else
				{
					unguard;
					return false;
				}

			}

			if(nActionID == 51)	//General Manufacturing
			{
				if(pUser->pSD->DuelInfo.DuelID)
				{
					pUser->SendSystemMessage(L"You cannot use General Manufacture while in duel.");
					unguard;
					return false;
				}
			}

			if(nActionID == 37)	//Dwarven Manufacturing
			{
				if(pUser->pSD->DuelInfo.DuelID)
				{
					pUser->SendSystemMessage(L"You cannot use Dwarven Manufacture while in duel.");
					unguard;
					return false;
				}
			}
			if(nActionID == 3)
			{
				UINT currentTick = GetTickCount();
				if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
				{
					pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
					if(pUser->pSD->pExData->territoryData.territoryId[TRADE_GEODATA_CHECK_ZONE] != 0)
					{
						User *pTarget = User::GetUserBySID(&pUser->targetId);
						if(pTarget->ValidUser())
						{
							if(CGeo::GetInstance()->CanSee(&pUser->pSD->Pos, &pTarget->pSD->Pos))
							{
								ret = _SockFunc(0x00863A50L)(pSocket, packet);
								unguard;
								return ret;
							}else
							{
								pSocket->SendSystemMessage(SystemMessage::CANNOT_SEE_TARGET_);
								unguard;
								return false;
							}
						}else
						{
							pSocket->SendSystemMessage(SystemMessage::INVALID_TARGET_);
							unguard;
							return false;
						}
					}else if(pUser->pSD->pExData->territoryData.territoryId[FORBIDDEN_TRADE_ZONE]!=0)
					{
						pUser->SendSystemMessage(L"You're not allowed to trade here.");
						unguard;
						return false;
					}else
					{
						pSocket->pED->itemActionTimestamp = GetTickCount() + g_ItemPacketReuse;
						ret = _SockFunc(0x00863A50L)(pSocket, packet);
						unguard;
						return ret;
					}
				}
				unguard;
				return false;
			}

			CSummon *pSummon = pUser->GetSummonOrPet();
			if(pSummon->IsSummon() && pSummon->pSD->pExData)
			{
				if(pSummon->pSD->pExData->SkillMod.nBlockIconAction) // == 65824
				{
					unguard;
					return false;
				}
			}

			if(pUser->pSD->pExData && pUser->pSD->pExData->SkillMod.nBlockIconAction) // == 65824
			{
				unguard;
				return false;
			}

			pSocket->pED->itemActionTimestamp = GetTickCount() + g_ItemPacketReuse;
			ret = _SockFunc(0x00863A50L)(pSocket, packet);
		}
	}

	unguard;
	return ret;
}

bool LoginPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	WCHAR accountName[16];
	memset(accountName, 0, sizeof(accountName));

	UINT accountId = 0, oneTimeKey = 0, session1 = 0, session2 = 0, country = 0;
	Disassemble(packet, "Sddddd", 30, accountName, &accountId, &oneTimeKey, &session1, &session2, &country);
	if(accountName[0] != 0 && accountName[15] == 0 && oneTimeKey != 0 && accountId > 0 && session2 > 0)
	{
		UINT error = g_LoginDb.CanLogin(accountId, accountName);
		if(error == 0)
		{
			ret = UserSocketCallback(0x885F20L)(pSocket, packet);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] blocked login to acc[%S][%d] - error[%d]!", __FUNCTION__, accountName, accountId, error);
		}
	}else
	{
		CIPAddress ip(pSocket->addr);
		g_Log.Add(CLog::Error, "[%s] Account[%S][%d] oneTimeKey[%d] s1[%d] s2[%d] country[%d] IP[%s] - hack!", __FUNCTION__, accountName, accountId, oneTimeKey, session1, session2, country, ip.ToString().c_str());
	}
	unguard;
	return ret;
}

bool LogoutPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
			ret = false;
		}else
		{
			g_OfflineShop.AutoActivate(pUser);
			if(pUser->pSD->activateOfflineShopTime)
			{
				pSocket->Send("c", LOGOUT_OK);
				ret = false;
			}else
			{
				ret = _SockFunc(0x0085FFE0L)(pSocket, packet);
			}
		}
	}

	unguard;
	return ret;
}

bool MagicSkillUsePacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->pED->isLocked)
		{
			unguard;
			return false;
		}

		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
			unguard;
			return false;
		}
		int nSkillID = 0, nCtrl = 0;
		BYTE bShift = 0;
		Disassemble(packet, "ddc", &nSkillID, &nCtrl, &bShift);

		if(int TerritoryID = pUser->pSD->nInBlockSkillsZone)
		{
			CBlockSkillsTerritory *pTerritory = g_TerritoryChecker.GetTerritory(TerritoryID)->SafeCastBlockSkills();
			if(pTerritory)
			{
				if(pTerritory->IsBlocked(nSkillID))
				{
					CSystemMessage msg(113);
					msg.AddSkill(nSkillID, 1);
					pSocket->SendSystemMessage(&msg);
					unguard;
					return false;
				}
			}else
			{
				pUser->pSD->nInBlockSkillsZone = 0;
			}
		}

		if(!g_AIOSystem.CanUse(pUser, nSkillID))
		{
			pUser->pSocket->SendSystemMessage(L"You cannot use AllInOne skills outside peace zone.");
			unguard;
			return false;
		}

		if(pUser->pED->tvtUser.status == TvT::UserPreparing || pUser->pED->tvtUser.status == TvT::UserFighting)
		{
			if(g_TvT.IsSkillBlocked(nSkillID))
			{
				pUser->pSocket->SendSystemMessage(L"You cannot use this skill during TvT match.");
				unguard;
				return false;
			}
		}

		if(pUser->pED->spawnProtection)
		{
			bool toggle = false;
			if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(nSkillID, 1))
			{
				if(pSI->operateType == OperateToggle)
				{
					toggle = true;
				}
			}
			if(!toggle)
			{
				g_SpawnProtection.DeactivateProtection(pUser);
			}
		}

		ret = _SockFunc(0x0087AEE0L)(pSocket, packet);
	}

	unguard;
	return ret;
}

bool ManagePledgePower(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		ret = ClanSys::ManagePledgePower(pSocket, packet);
	}

	unguard;
	return ret;
}

bool PetGetItem(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;

	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
			if(pUser->pED->isLocked)
			{
				unguard;
				return false;
			}
			UINT itemServerId = 0;
			Disassemble(packet, "d", &itemServerId);
			
			if(CItem *pItem = CObject::GetObjectBySID(itemServerId)->SafeCastItem())
			{
				bool allowed = true;
				if(g_PetAllowedItems.size() > 0)
				{
					for(UINT n=0;n< g_PetAllowedItems.size();n++)
					{
						if(pItem->pSID->nItemID == g_PetAllowedItems[n])
						{
							allowed = true;
							break;
						}
					}
				}
				if(allowed)
				{
					ret = _SockFunc(0x872A70L)(pSocket, packet);
				}else
				{
					pUser->SendSystemMessage(L"Your pet cannot pick up this item!");
					ret = false;
				}
			}
		}else
		{
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool RequestSetCastleSiegeTimePacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = false;
	if(User *pUser = pSocket->pUser->SafeCastUser())
	{
		if(g_CastleSiegeManager.IsEnabled() == FALSE)
		{
			ret = _SockFunc(0x8745F0L)(pSocket, packet);
		}else
		{
			pUser->SendSystemMessage(L"Castle siege time can be set only by administration!");
		}
	}
	unguard;
	return ret;
}

bool MultisellChoose(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;

	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;

			if(pUser->pED->isLocked)
			{
				unguard;
				return false;
			}

			UINT nListID = 0, nEntryID = 0;
			INT32 nAmount = 0;
			Disassemble(packet, "ddd", &nListID, &nEntryID, &nAmount);
			//g_Log.Add(CLog::Blue, "[%s] List[%d] Entry[%d]", __FUNCTION__, nListID, nEntryID);
			if(nAmount > 0 && nAmount <= 10000)
			{
				INT32 nAdena = pUser->GetInventory()->GetAdenaAmount();
				if(nAdena < 0 || nAdena > 2100000000)
				{
					pSocket->SendSystemMessage(L"Please come back when you get less adena in your inventory.");
					unguard;
					return false;
				}
				if(g_MultiSellDB.Validate(pUser, nListID, nEntryID, nAmount))
				{
					g_MultiSellStat.SetInfo(nListID, nEntryID, nAmount);
					ret = _SockFunc(0x00868480L)(pSocket, packet);
				}else
				{
					ret = false;	//Drop packet
				}
			}else
			{
				pSocket->SendSystemMessage(L"Invalid item count!");
				ret = false;
			}
		}else
		{
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool PrivateStoreBuyBuyListSend(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
			try
			{
				UINT nTraderSID = 0;
				int nItemAmount = 0;
				const unsigned char* pck = Disassemble(packet, "dd", &nTraderSID, &nItemAmount);
				if(nItemAmount > 0 && nItemAmount < 10)
				{
					list<int> lTmp;
					for(int n=0;n<nItemAmount;n++)
					{
						int nStoreIndex;
						int nItemID;
						short sEnchant;
						short sDamaged;
						int nCount;
						int nPrice;
						pck = Disassemble(pck, "ddhhdd", &nStoreIndex, &nItemID, &sEnchant, &sDamaged, &nCount, &nPrice);
						if(nPrice < 1)
						{
							pSocket->SendSystemMessage(L"Invalid item price.");
							unguard;
							return false;
						}
						if(nCount < 1)
						{
							pSocket->SendSystemMessage(L"Invalid item count.");
							unguard;
							return false;
						}
						if(nCount > 1)
						{
							for(list<int>::iterator Iter=lTmp.begin();Iter!=lTmp.end();Iter++)
							{
								if((*Iter) == nItemID)
								{
									g_Log.Add(CLog::Error, "[%s] Buying same item id twice! user[%S] item[%d]", __FUNCTION__, pUser->pSD->wszName, nItemID);
									pUser->SendSystemMessage(L"You cannot do that action!");
									unguard;
									return false;
								}
							}
							lTmp.push_back(nItemID);
						}
					}

					ret = _SockFunc(0x8813f0L)(pSocket, packet);
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
			}
		}
	}

	unguard;
	return ret;
}

bool PrivateStoreBuyListSend(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
				
			_SockFunc psbls = (_SockFunc) 0x00880BF0;

			UINT nTraderSID; int nItemAmount = 0;
			const unsigned char* pck = Disassemble(packet, "dd", &nTraderSID, &nItemAmount);
			if(nItemAmount > 0 && nItemAmount < 12)
			{
				list<UINT> lItems;
				for(int n=0;n<nItemAmount;n++)
				{
					int objectId;
					int itemCount;
					int price;
					pck = Disassemble(pck, "ddd", &objectId, &itemCount, &price);
					if(itemCount < 1 || itemCount > 2000000000)
					{
						g_Log.Add(CLog::Error, "[%s] Invalid item count[%d][%d][%d user[%S]", __FUNCTION__, itemCount, objectId, price, pUser->pSD->wszName);
						unguard;
						return false;
					}
					if(price < 1)
					{
						g_Log.Add(CLog::Error, "[%s] Invalid item price[%d][%d][%d] user[%S]", __FUNCTION__, price, objectId, itemCount, pUser->pSD->wszName);
						unguard;
						return false;
					}
					//Check for dupe
					//g_Log.Add(CLog::Blue, "[%s] ItemID[%d] Count[%d]", __FUNCTION__, nObjectSID, nCount);
					for(list<UINT>::iterator Iter = lItems.begin();Iter!=lItems.end();Iter++)
					{
						if(*Iter == objectId)
						{
							//Cheater
							unguard;
							return false;
						}
					}
					lItems.push_back(objectId);
				}

				bool ret = psbls(pSocket, packet);
				unguard;
				return ret;
			}else
			{
				unguard;
				return false;
			}
		}else
		{
			unguard;
			return false;
		}
	}
	unguard;
	return true;
}

bool PrivateStoreBuyListSet(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	if(User *pUser = pSocket->GetUser())
	{
		if(CSummon *pSummon = pUser->GetSummonOrPet())
		{
			pUser->SendSystemMessage(L"You cannot make a private store while having a summon or pet.");
			pUser->QuitPrivateStoreBuy();
			unguard;
			return false;
		}

		if(pUser->pED->isLocked)
		{
			pUser->QuitPrivateStoreBuy();
			unguard;
			return false;
		}

		if(pUser->pSD->nLevel < g_PrivateStoreSystem.GetMinRequiredLevel())
		{
			pUser->SendSystemMessage(L"You cannot make a private store because your level is too low!");
			pUser->QuitPrivateStoreBuy();
			unguard;
			return false;
		}

		double minDistance = g_PrivateStoreSystem.GetMinDistance();

		if(minDistance > 0.0)
		{
			if(g_UserDB.AnyPrivateStoreInRange(pUser->pSD->Pos, minDistance))
			{
				pUser->SendSystemMessage(L"You cannot make a private store becouse you're too close to another private store.");
				pUser->QuitPrivateStoreBuy();
				unguard;
				return false;
			}
		}

		INT64 totalPrice = 0;

		INT32 itemCount = 0;
		const unsigned char* pck = Disassemble(packet, "d", &itemCount);
		if(itemCount > 0 && itemCount < 20)
		{
			for(int n=0;n<itemCount;n++)
			{
				int itemId = 0, amount = 0, price = 0;
				short enchant = 0, damaged = 0;
				price = 0;
				//dhhdd
				pck = Disassemble(pck, "dhhdd", &itemId, &enchant, &damaged, &amount, &price);
				INT64 tempPrice = amount;
				tempPrice *= price;
				totalPrice += tempPrice;
				UINT error = 0;
				if(!g_PrivateStoreSystem.IsValidStoreItem(itemId, price, amount, error))
				{
					if(error == 1)
					{
						//2516	1	a,Price of $s1 is out of allowed this item price range.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						CSystemMessage msg(2516);
						msg.AddItem(itemId);
						pUser->SendSystemMessage(&msg);
						pUser->QuitPrivateStoreBuy();
						unguard;
						return false;
					}else if(error == 2)
					{
						//2517	1	a,You cannot sell/buy that many $s1 at once.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						CSystemMessage msg(2517);
						msg.AddItem(itemId);
						pUser->SendSystemMessage(&msg);
						pUser->QuitPrivateStoreBuy();
						unguard;
						return false;
					}
				}
			}
		}else
		{
			pUser->SendSystemMessage(L"Invalid item count!");
			pUser->QuitPrivateStoreBuy();
			unguard;
			return false;
		}

		if(totalPrice > pUser->inventory.GetAdenaAmount())
		{
			pUser->SendSystemMessage(L"You don't have enoguh adena in your inventory!");
			pUser->QuitPrivateStoreBuy();
			unguard;
			return false;
		}

		bool bRet = UserSocketCallback(0x873130L)(pSocket, packet);

		if(g_PrivateStoreSystem.StoreInDB())
		{
			if(pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1)
			{
				g_PrivateStoreSystem.OnCreateStore(pUser);
			}
		}
		unguard;
		return bRet;
	}
	unguard;
	return true;
}

bool PrivateStoreListSet(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	if(User *pUser = pSocket->GetUser())
	{
		int nPackageSell, nItemsCount;
		const unsigned char* p = Disassemble(packet, "dd", &nPackageSell, &nItemsCount);
		if(nItemsCount > 0 && nItemsCount < 9)
		{

			if(pUser->pED->isLocked)
			{
				pUser->QuitPrivateStore();
				unguard;
				return false;
			}
			CSummon *pSummon = pUser->GetSummonOrPet();
			
			if(pSummon->ValidCreature())
			{
				pUser->SendSystemMessage(L"You cannot make a private store while having a summon or pet.");
				pUser->QuitPrivateStore();
				unguard;
				return false;
			}

			if(pUser->pSD->nLevel < g_PrivateStoreSystem.GetMinRequiredLevel())
			{
				pUser->SendSystemMessage(L"You cannot make a private store because your level is too low!");
				unguard;
				return false;
			}

			double minDistance = g_PrivateStoreSystem.GetMinDistance();
			if(minDistance > 0.0)
			{
				if(g_UserDB.AnyPrivateStoreInRange(pUser->pSD->Pos, minDistance))
				{
					pUser->SendSystemMessage(L"You cannot make a private store becouse you're too close to another private store.");
					pUser->QuitPrivateStore();
					unguard;
					return false;
				}
			}

			INT64 nFPrice = 0;
			for(int n=0;n<nItemsCount;n++)
			{
				UINT nItemSID = 0;
				INT32 nCount = 0, nPrice = 0;
				p = Disassemble(p, "ddd", &nItemSID, &nCount, &nPrice);
				if(nPrice < 1 || nCount < 1)
				{
					unguard;
					return true;
				}
				
				if(CItem *pItem = CObject::GetObjectBySID(nItemSID)->SafeCastItem())
				{
					if(pItem->nAugmentationID || pItem->nManaLeft || pItem->nProtectionTimeout > time(0))
					{
						pSocket->SendSystemMessage(L"You cannot sell that item.");
						pUser->QuitPrivateStore();
						unguard;
						return false;
					}
					UINT error = 0;
					if(!g_PrivateStoreSystem.IsValidStoreItem(pItem->pSID->nItemID, nPrice, nCount, error))
					{
						if(error == 1)
						{
							//2516	1	a,Price of $s1 is out of allowed this item price range.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							CSystemMessage msg(2516);
							msg.AddItem(pItem->pSID->nItemID);
							pUser->SendSystemMessage(&msg);
							pUser->QuitPrivateStore();
							unguard;
							return false;
						}else if(error == 2)
						{
							//2517	1	a,You cannot sell/buy that many $s1 at once.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							CSystemMessage msg(2517);
							msg.AddItem(pItem->pSID->nItemID);
							pUser->SendSystemMessage(&msg);
							pUser->QuitPrivateStore();
							unguard;
							return false;
						}
					}

					nFPrice += (nPrice*nCount);
				}else
				{
					pUser->SendSystemMessage(L"Invalid item!");
					pUser->QuitPrivateStore();
					unguard;
					return false;
				}
			}
			int nAdenaAmount = pUser->GetInventory()->GetAdenaAmount();
			if((nFPrice+nAdenaAmount) >= INT_MAX)
			{
				pSocket->SendSystemMessage(L"You've got too much adena in your invetory to be able to sell that item with current price.");
				pUser->QuitPrivateStore();
				unguard;
				return false;
			}
			bool bRet = UserSocketCallback(0x870B30L)(pSocket, packet);

			if(g_PrivateStoreSystem.StoreInDB())
			{
				if(pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1)
				{
				//	g_Log.Add(CLog::Blue, "[%s] Created Store", __FUNCTION__);
					g_PrivateStoreSystem.OnCreateStore(pUser);
				}
			}
			unguard;
			return bRet;
		}
	}
	unguard;
	return true;
}

bool RequestAcquireSkill(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		int nSkillID, nLevel, nType;
		Disassemble(packet, "ddd", &nSkillID, &nLevel, &nType);

		if(nSkillID <= 0 || nLevel <= 0)
		{
			unguard;
			return false;
		}

		pSocket->pED->itemActionTimestamp = GetTickCount() + g_ItemPacketReuse;

		if(nType == 2)
		{
			ret = ClanSys::RequestAcquireSkill(pSocket, nSkillID, nLevel);
		}else
		{
			if(nType == g_SkillMaster.m_AcquireSkillMode && pUser->pED->skillMasterFlag)
			{
				g_SkillMaster.AcquireSkill(pUser, nSkillID, nLevel, pUser->pED->skillMasterFlag);
			}else
			{
				ret = _SockFunc(0x00865DA0L)(pSocket, packet);
			}
		}
		ret = false;
	}
	unguard;
	return ret;
}

bool RequestJoinParty(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		WCHAR wTargetName[50];
		Disassemble(packet, "S", sizeof(wTargetName), wTargetName);

		if(pUser->pSD->pExData->territoryData.territoryId[NO_PARTY_ZONE] != 0)
		{
			pSocket->SendSystemMessage(L"You cannot invite to party in thiz zone!");
			unguard;
			return false;
		}

		if(pUser->pSD->nCursedOwner)
		{
			pSocket->SendSystemMessage(L"You cannot make a party when equipping a cursed weapon!");
			unguard;
			return false;
		}

		User *pTarget = g_UserDB.GetUserByName(wTargetName);
		if(pTarget->ValidUser())
		{
			if(pTarget->pSD->nCursedOwner != 0 || pTarget->olympiadUser.IsInside() || pTarget->pSD->DuelInfo.DuelID != 0 || pTarget->pSD->activateOfflineShopTime != 0 || pTarget->pSD->pExData->territoryData.territoryId[NO_PARTY_ZONE] != 0)
			{
				pSocket->SendSystemMessage(L"You cannot invite specified player to party.");
				unguard;
				return false;
			}

			if(g_MaxPartyLevelRange && g_BlockPartyInvite)
			{
				INT32 minLevel = pUser->pSD->nLevel;
				INT32 maxLevel = pUser->pSD->nLevel;
				if(CParty *pParty = pUser->GetParty())
				{
					pParty->GetMinMaxLevel(minLevel, maxLevel);
				}

				INT32 targetLevel = pTarget->pSD->nLevel;
				if( (minLevel + g_MaxPartyLevelRange) < targetLevel )
				{
					pSocket->SendSystemMessage(L"Your target has too high level.");
					unguard;
					return false;
				}

				if( (maxLevel - g_MaxPartyLevelRange) > targetLevel )
				{
					pSocket->SendSystemMessage(L"Your target has too low level.");
					unguard;
					return false;
				}
			}

		}

		if(pUser->pSD->nCursedOwner == 0 && pUser->olympiadUser.IsInside() == FALSE && pUser->pSD->DuelInfo.DuelID == 0 && pUser->pSD->activateOfflineShopTime == 0)
		{
			ret = _SockFunc(0x00889C50L)(pSocket, packet);
		}else
		{
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool RequestJoinPledge(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->pSD->activateOfflineShopTime == 0)
		{
			ret = ClanSys::AskJoinPledgePacket(pSocket, packet);
		}
	}

	unguard;
	return ret;
}

bool RequestPackageSend(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
			int nCharID = 0;
			int nAmount = 0;
			const unsigned char* pck = Disassemble(packet, "dd", &nCharID, &nAmount);
			if(nCharID == pUser->nDBID)
			{
				pSocket->SendSystemMessage(L"You cannot send package to yourself!");
				unguard;
				return false;
			}
			if( nCharID > 0 )
			{
				if(pUser->IsNowTrade())
				{
					pUser->TradeCancel();
					ret = false;
				}else
				{
					CPet *pPet = pUser->GetPet();

					if(nAmount < 88 && nAmount > 0)
					{
						vector<INT32> vStack;
						for(int n=0;n< nAmount; n++)
						{
							int nItemID, nCount;
							pck = Disassemble(pck, "dd", &nItemID, &nCount);
							
							for(int idx = 0; idx < vStack.size(); idx++)
							{
								if(vStack[idx] == nItemID)
								{
									pUser->pSocket->SendSystemMessage(L"You cannot deposit same item twice!");
									unguard;
									return false;
								}
							}
							vStack.push_back(nItemID);

							CItem *pItem = pUser->GetInventory()->GetItemByDBID(nItemID);
							if(pItem->IsValidItem())
							{
								if(pItem->nAugmentationID || pItem->nManaLeft || pItem->nProtectionTimeout > time(0) || pItem->nLifeTime > 0)
								{
									//This item cannot be moved to warehouse
									pSocket->SendSystemMessage(L"You cannot deposit this items.");
									unguard;
									return false;
								}
								if(pItem->pSID->nItemID == 8190 || pItem->pSID->nItemID == 8689)
								{
									pSocket->SendSystemMessage(L"You cannot deposit cursed weapon.");
									unguard;
									return false;
								}

								if(pPet->IsPet())
								{
									if(pPet->pSD->nPetDBID == pItem->pSID->nDBID)
									{
										pSocket->SendSystemMessage(L"You cannot deposit collar when pet is summoned.");
										unguard;
										return false;
									}
								}
							}
						}

						ret = _SockFunc(0x00878F80L)(pSocket, packet);
					}else
					{
						ret = false;
					}
				}
			}
		}else
		{
			ret = false;
		}
	}
	unguard;
	return ret;
}

bool RequestPackageSendableItemList(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		int nCharID = 0;
		Disassemble(packet, "d", &nCharID);

		if( nCharID > 0 )
		{
			ret =  _SockFunc(0x00867BF0L)(pSocket, packet);
		}
	}
	unguard;
	return ret;
}

bool RequestRecipeBookItemMake(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->pSD->DuelInfo.DuelID)
		{
			pUser->SendSystemMessage(L"You cannot create items while fighting in duel.");
			ret = false;
		}else
		{
			ret = UserSocketCallback(0x868FD0L)( pSocket, packet);
		}
	}
	unguard;
	return ret;
}

bool RequestRecipeShopSetList(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;

	if(User *pUser = pSocket->GetUser())
	{
		CSummon *pSummon = pUser->GetSummonOrPet();
		if(pSummon->ValidCreature())
		{
			pUser->SendSystemMessage(L"You cannot make a recipe store while having a summon or pet.");
			pUser->QuitRecipeStore();
			unguard;
			return false;
		}

		if(pUser->pSD->nLevel < g_PrivateStoreSystem.GetMinRequiredLevel())
		{
			pUser->SendSystemMessage(L"You cannot make a recipe store because your level is too low!");
			pUser->QuitRecipeStore();
			unguard;
			return false;
		}

		if(pUser->pED->isLocked)
		{
			pUser->QuitRecipeStore();
			unguard;
			return false;
		}

		double minDistance = g_PrivateStoreSystem.GetMinDistance();
		if(minDistance > 0.0)
		{
			if(g_UserDB.AnyPrivateStoreInRange(pUser->pSD->Pos, minDistance))
			{
				pUser->SendSystemMessage(L"You cannot make a recipe store becouse you're too close to another private store.");
				pUser->QuitRecipeStore();
				unguard;
				return false;
			}
		}

		if(pUser->pSD->DuelInfo.DuelID)
		{
			pUser->SendSystemMessage(L"You cannot setup a recipe store while fighting in duel.");
			pUser->QuitRecipeStore();
			unguard;
			return false;
		}

		UINT recipeCount = 0;
		const unsigned char* pck = Disassemble(packet, "d", &recipeCount);
		if(recipeCount < 64)
		{
			INT32 recipeId = 0, price = 0;
			for(UINT n=0;n<recipeCount;n++)
			{
				pck = Disassemble(pck, "dd", &recipeId, &price);
				if(!g_PrivateStoreSystem.IsValidStoreRecipe(recipeId, price))
				{
					//2518	1	a,Price of $s1 is out of allowed price range for this recipe.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					if(CRecipeInfo *pRI = pUser->GetRecipeInfo(recipeId))
					{
						CSystemMessage msg(2518);
						msg.AddItem(pRI->ItemID);

						pUser->SendSystemMessage(&msg);
					}
					pUser->QuitRecipeStore();
					unguard;
					return false;
				}
			}
		}else
		{
			pUser->SendSystemMessage(L"You've reached limit of recipes in recipe store!");
			pUser->QuitRecipeStore();
			unguard;
			return false;
		}

		ret = UserSocketCallback(0x875230L)( pSocket, packet);
		if(g_PrivateStoreSystem.StoreInDB())
		{
			if(pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1)
			{
				//g_Log.Add(CLog::Blue, "[%s] Created Store", __FUNCTION__);
				g_PrivateStoreSystem.OnCreateStore(pUser);
			}
		}
	}
	unguard;
	return ret;
}

bool RequestStartPledgeWar(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		if(CPledge *pPledge = pUser->GetPledge())
		{
			WCHAR wEnemy[50];
			Disassemble(packet, "S", sizeof(wEnemy), wEnemy);
			CPledge *pEnemy = g_PledgeDB.GetPledgeByName(wEnemy);
			if(pEnemy && pPledge->pSD && pEnemy->pSD)
			{
				int nEnemyID = pEnemy->pSD->nDBID;
				if(!pPledge->pSD->pPledgeEx->WarDelay.HasDelay(nEnemyID))
				{
					pPledge->pSD->pPledgeEx->WarDelay.StartDelay(nEnemyID);
					ret = UserSocketCallback(0x884A30L)( pSocket, packet);
				}else
				{
					pSocket->SendSystemMessage(L"The War with this clan is not available at this time: being prepared for reuse.");
					ret = false;
				}
			}else
			{
				pSocket->SendSystemMessage(L"Clan cannot be found!");
				ret = false;
			}
		}
	}

	unguard;
	return ret;
}

bool RequestStopPledgeWar(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		if(CPledge *pPledge = pUser->GetPledge())
		{
			WCHAR wEnemy[50];
			Disassemble(packet, "S", sizeof(wEnemy), wEnemy);
			CPledge *pEnemy = pPledge->GetEnemyPledgeByName(wEnemy);
			if(pEnemy)
			{
				int nEnemyID = pEnemy->pSD->nDBID;
				if(pPledge->IsInWarWithEx(pEnemy->nObjectID))
				{
					if(!pPledge->pSD->pPledgeEx->WarDelay.HasDelay(nEnemyID))
					{
						pPledge->pSD->pPledgeEx->WarDelay.StartDelay(nEnemyID);
						ret = UserSocketCallback(0x885270L)( pSocket, packet);
					}else
					{
						pSocket->SendSystemMessage(L"Cancelling The War with this clan is not available at this time: being prepared for reuse.");
						ret = false;
					}
				}else
				{
					pPledge->pSD->pPledgeEx->WarDelay.DelDelay(nEnemyID);
					ret = UserSocketCallback(0x885270L)( pSocket, packet);
				}
			}else
			{
				pSocket->SendSystemMessage(L"Enemy clan cannot be found!");
				ret = false;
			}
		}
	}

	unguard;
	return ret;
}

bool RequestUserCommand(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
			int nCmdID = 0;
			Disassemble(packet, "d", &nCmdID);
			
			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
				ret = false;
			}else
			{
				if(nCmdID == 115)
				{
					//show menu
					UserExCommandMenu(pUser, L"");
					ret = false;
				}else if(nCmdID == COfflineShop::OFFLINE_SHOP_USER_COMMAND)
				{
					g_OfflineShop.RequestActivateOfflineShop(pUser);
					ret = false;
				}else
				{
					ret = _SockFunc(0x0088EF50L)(pSocket, packet);
				}
			}
		}else
		{
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool RestartPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;

			g_OfflineShop.AutoActivate(pUser);
			if(pUser->pSD->activateOfflineShopTime)
			{
				pSocket->Send("c", LOGOUT_OK);
				ret = false;
			}else
			{
				ret = _SockFunc(0x0088B7B0L)(pSocket, packet);
			}
		}else
		{
			ret = false; //drop
		}
	}

	unguard;
	return ret;
}

bool RestartPointPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->pUser->SafeCastUser())
	{
		DWORD restartPointType = 0;
		Disassemble(packet, "d", &restartPointType);
	//	g_Log.Add(CLog::Blue, "[%s] [%d]", __FUNCTION__, restartPointType);

		if(pUser->olympiadUser.IsInside())
		{
			pUser->SendSystemMessage(L"You cannot do that while fighting in Olympiad Match!");
			unguard;
			return false;
		}
		if(pUser->pED->tvtUser.status == TvT::UserFighting)
		{
			pUser->SendSystemMessage(L"You cannot do that while fighting in TvT!");
			unguard;
			return false;
		}

		if(pUser->pSD->nAlive == 0)
		{
			//clearing pvp flag
			if(pUser->pSD->bGuilty && pUser->pSD->nKarma == 0)
			{
				pUser->pSD->bGuilty = false;
				pUser->pSD->nPvPAttackTimeStamp = 0;
			}

			if(pUser->pSD->pExData->territoryData.territoryId[RESPAWN_ZONE] != 0)
			{
				if(CRespawnTerritory* pT = g_TerritoryChecker.GetTerritory(pUser->pSD->pExData->territoryData.territoryId[RESPAWN_ZONE])->SafeCastRespawn())
				{
					IVector pos = pT->GetRandomRespawn();
					if(pos.x != 0 || pos.y != 0 || pos.z != 0)
					{
						pUser->Revive();
						PlayerAction::OnTeleportToLocation(pUser, pos.x, pos.y, pos.z, 0);
						
						bool updateStatus = false;
						int hpRatio = pT->GetHpRatio();
						if(hpRatio != 0)
						{
							double hp = pUser->pSD->MaxHP;
							hp *= hpRatio;
							hp /= 100.0;
							if(hp > pUser->pSD->fHP)
							{
								pUser->pSD->fHP = hp;
								updateStatus = true;
							}
						}
						int cpRatio = pT->GetCpRatio();
						if(cpRatio != 0)
						{
							double cp = pUser->pSD->MaxCP;
							cp *= cpRatio;
							cp /= 100.0;
							if(cp > pUser->pSD->fCP)
							{
								pUser->pSD->fCP = cp;
								updateStatus = true;
							}
						}
						int mpRatio = pT->GetMpRatio();
						if(mpRatio != 0)
						{
							double mp = pUser->pSD->MaxMP;
							mp *= mpRatio;
							mp /= 100.0;
							if(mp > pUser->pSD->fMP)
							{
								pUser->pSD->fMP = mp;
								updateStatus = true;
							}
						}
						if(updateStatus)
						{
							pUser->UpdateStatus(User::UPDATE_CP);
							pUser->UpdateStatus(User::UPDATE_MP);
						}
						unguard;
						return false;
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Invalid pos in RESPAWN_ZONE (%d/%d/%d) - User[%S] !", __FUNCTION__, pos.x, pos.y, pos.z, pUser->pSD->wszName);
					}
				}
			}
		}

		pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::VALIDATE_STATS, 1);
		ret = Callback(0x866750L)(pSocket, packet);
	}

	unguard;
	return ret;
}

bool SayPacket2(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	if(User *pUser = pSocket->GetUser())
	{
		try
		{
			//check if string is valid
			int cnt = 0;
			bool valid = false;
			while(cnt < 2000)
			{
				if(packet[cnt] == 0 && packet[cnt+1] == 0)
				{
					valid = true;
					break;
				}
				cnt++;
			}

			if(!valid)
			{
				unguard;
				return false;
			}

			//check for \n start
			WCHAR *wStr = (PWCHAR)packet;
			WCHAR *wEndLine = wcsstr(wStr, L"\\n");
			int k = 0;
			while( wEndLine  && k < 2048)
			{
				k++;
				wEndLine[0] = L' ';
				wEndLine[1] = L' ';
				wEndLine = wcsstr(wStr, L"\\n");
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Invalid string - User[%S]", __FUNCTION__, pUser->pSD->wszName);
			unguard;
			return false;
		}

		WCHAR* wStr = (WCHAR*)packet;
		bool drop = false;
		bool kick = false;
		
		g_ObsceneFilter.CheckString(pUser, wStr, drop, kick);

		if(kick)
		{
			unguard;
			return true;
		}
		if(drop)
		{
			unguard;
			return false;
		}

		WCHAR wTxt[4048] = { 0 };
		int nType;
		Disassemble(packet, "Sd", sizeof(wTxt), wTxt, &nType);

		if(pUser->pED->isLocked)
		{
			g_CharacterLock.OnSay(pUser, wTxt);
			unguard;
			return false;
		}

		try
		{
			if(pUser->pSD->vipLevel && wTxt[0] == L'^')
			{
				if(pUser->stopSayTick == 0)
				{
					VIPInfo vipInfo = g_VIPSystem.GetInfo(pUser->pSD->vipLevel);
					if(vipInfo.specialChat && pUser->pED->vipChatTimestamp < GetTickCount())
					{
						pUser->pED->vipChatTimestamp = GetTickCount() + vipInfo.chatDelay;
						WCHAR wMsg[4096] = { 0 };
						if(swprintf(wMsg, 4090, L"%s: %s", pUser->pSD->wszName, &wTxt[1]) > 0)
						{
							char buff[8190];
							int len = Assemble(buff, 8190, "cddSS", 0x4A, 0, 18, L"VIP", wMsg);
							L2Server::BroadcastToAllUser(len, buff);
						}
						unguard;
						return false;
					}else
					{
						unguard;
						return false;
					}
				}else
				{
					pSocket->SendSystemMessage(147);	//chatting is prohibited
					unguard;
					return false;
				}
			}

			if(g_UserExCommand.IsUserExCommand(wTxt))
			{
				pSocket->pED->itemActionTimestamp = GetTickCount() + 500;

				if(pUser->IsNowTrade())
				{
					pUser->TradeCancel();
					unguard;
					return false;
				}

				bool ret = g_UserExCommand.Handle(pUser, wTxt);
				unguard;
				return ret;
			}

		}catch(...)
		{
			EXCEPTION_LOG;
		}

		CBlockChatTerritory *pTerritory = 0;
		if(pUser->pED->blockChatZoneId)
		{
			pTerritory = g_TerritoryChecker.GetTerritory(pUser->pED->blockChatZoneId)->SafeCastBlockChat();
		}

		if(pUser->pSD->nBuilder == 0)
		{
			switch(nType)
			{
			case SAY_ALL:
				{
					if(pTerritory && pTerritory->IsSet(CBlockChatTerritory::ALL))
					{
						unguard;
						return false;
					}

					break;
				}
			case SAY_SHOUT:
				{
					if(g_ChatShoutLevel > 0 && g_ChatShoutLevel > pUser->pSD->nLevel)
					{
						pUser->SendSystemMessage(L"Your level is too low to use shout chat!");
						unguard;
						return false;
					}
					if(pTerritory && pTerritory->IsSet(CBlockChatTerritory::SHOUT))
					{
						unguard;
						return false;
					}

					break;
				}
			case SAY_TELL:
				{
					if(pTerritory && pTerritory->IsSet(CBlockChatTerritory::TELL))
					{
						unguard;
						return false;
					}

					break;
				}
			case SAY_PARTY:
				{
					if(pTerritory && pTerritory->IsSet(CBlockChatTerritory::PARTY))
					{
						unguard;
						return false;
					}

					break;
				}
			case SAY_CLAN:
				{
					if(pTerritory && pTerritory->IsSet(CBlockChatTerritory::CLAN))
					{
						unguard;
						return false;
					}

					break;
				}
			case SAY_GM:
				{
					break;
				}
			case SAY_PETITION_PLAYER:
				{
					break;
				}
			case SAY_PETITION_GM:
				{
					break;
				}
			case SAY_TRADE:
				{
					if(pTerritory && pTerritory->IsSet(CBlockChatTerritory::TRADE))
					{
						unguard;
						return false;
					}

					break;
				}
			case SAY_ALLIANCE:
				{
					if(pTerritory && pTerritory->IsSet(CBlockChatTerritory::ALLIANCE))
					{
						unguard;
						return false;
					}

					CPledge *pPledge = pUser->GetPledge();
					if(pPledge && pPledge->pSD && pPledge->pSD->nLevel > 4 && pUser->IsAcademyMember())
					{
						unguard;
						return false;
					}
					break;
				}
			case SAY_ANNOUNCEMENT:
				{
					break;
				}
			case SAY_PARTYROOM_ALL:
				{
					break;
				}
			case SAY_PARTYROOM_COMMANDER:
				{
					break;
				}
			case SAY_HERO_VOICE:
				{
					if(pTerritory && pTerritory->IsSet(CBlockChatTerritory::HERO))
					{
						unguard;
						return false;
					}

					DWORD Time = time(NULL);
					if(pUser->pSD->dHeroChatDelay > Time)
					{
						unguard;
						return false;
					}else
					{
						pUser->pSD->dHeroChatDelay = Time + g_HeroVoiceDelay;
					}
					break;
				};
			}
		}

		if(pUser->pED->spawnProtection)
		{
			g_SpawnProtection.DeactivateProtection(pUser);
		}

		if(SAY_SHOUT == nType && g_GlobalChat)
		{
			if(pUser->stopSayTick == 0)
			{
				CHAR buff[8190];
				int len = Assemble(buff, 8190, "cddSS", 0x4A, pUser->nObjectID, SAY_SHOUT, pUser->pSD->wszName, wTxt);
				L2Server::BroadcastToAllUser(len, buff);
			}else
			{
				pSocket->SendSystemMessage(147);	//chatting is prohibited
			}
			unguard;
			return false;
		};

		bool ret = false;

		if(g_ChatManager.IsEnabled())
		{
			ret = g_ChatManager.GetInstance()->Handle(pSocket, packet, nType);
		}else
		{
			ret = Callback(0x00886870L)(pSocket, packet);
		}
		unguard;
		return ret;
	}

	unguard;
	return true;
}

bool TradeAddItems(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;

			if(pUser->IsNowTrade())
			{
				int itemCount = 0;
				UINT nObjectID = 0;
				int nCount = 0;
				Disassemble(packet, "ddd", &itemCount, &nObjectID, &nCount);
				if(nCount > 0 && itemCount == 1 && nCount < INT_MAX)
				{
					if(CItem *pItem = pUser->inventory.GetItemBySID(nObjectID)->SafeCastItem())
					{
						if(CPet *pPet = pUser->GetPet())
						{
							if(pItem->pSID->nDBID == pPet->pSD->nDBID)
							{
								pSocket->SendSystemMessage(L"You cannot trade pet collar when pet is summoned!");
								unguard;
								return false;
							}
						}
						if(pItem->pSID->nItemAmount < nCount)
						{
							g_Log.Add(CLog::Error, "[%s] User[%S] try to trade more items than he have itemId[%d] [%d] vs [%d]", __FUNCTION__, pUser->pSD->wszName, pItem->pSID->nItemID, nCount, pItem->pSID->nItemAmount);
							unguard;
							return false;
						}

						if(pItem->nAugmentationID != 0 || pItem->nManaLeft != 0 || pItem->nLifeTime != 0 || pItem->nProtectionTimeout != 0 )
						{
							pUser->SendSystemMessage(L"You cannot trade this item!");
							unguard;
							return false;
						}

						if(pItem->pSID->nConsumeType == ConsumeAsset || pItem->pSID->nConsumeType == ConsumeStackable)
						{
							//check for same item type
							//new check for duplicated item id 
							CTradeSP tradeSp;
							g_TradeManager.GetTradeSP(&tradeSp, pUser->tradeId);
							if(CTrade *pTrade = tradeSp.get())
							{
								if(!pTrade->CanAdd(pItem->pSID->nItemID, pUser->nObjectID))
								{
									pSocket->SendSystemMessage(L"You cannot exchange same stackable item(s).");
									unguard;
									return false;
								}
							}else
							{
								g_Log.Add(CLog::Error, "[%s] User[%S] tried to trade add items without active trade!", __FUNCTION__, pUser->pSD->wszName);
								pSocket->SendSystemMessage(L"Invalid operation!");
								unguard;
								return false;
							}
						}

						bool ret = UserSocketCallback(0x00860780L)(pSocket, packet);
						unguard;
						return ret;
					}else
					{
						g_Log.Add(CLog::Error, "[%s] User[%S] request trade add invalid item[%d] count[%d] - can be packet flood!", __FUNCTION__, pUser->pSD->wszName, nObjectID, nCount);
						unguard;
						return false;
					}
				}
			}
		}
		unguard;
		return false;
	}
	unguard;
	return true;
}

bool TradeDonePacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->IsNowTrade())
		{
			ret = Callback(0x00860940L)(pSocket, packet);
		}else
		{
			ret = false;	//drop
		}
	}

	unguard;
	return ret;
}

bool TradeRequestPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;

			if(pUser->pED->isLocked)
			{
				unguard;
				return false;
			}

			if(pUser->pSD->pExData->territoryData.territoryId[FORBIDDEN_TRADE_ZONE]!=0)
			{
				pUser->SendSystemMessage(L"You're not allowed to trade here.");
				unguard;
				return false;
			}

			if(pUser->IsNowTrade() || pUser->pSD->nStoreMode != 0 || pUser->olympiadUser.IsInside())
			{
				pUser->SendSystemMessage(L"You cannot do that action in current state.");
				pUser->ActionFailed();
				unguard;
				return false;
			}

			if(g_PetBlockTrade && pUser->GetPet())
			{
				//2500	1	a,You cannot use trade while your pet is summoned!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pSocket->SendSystemMessage(2500);
				unguard;
				return false;
			}

			UINT targetId = 0;
			Disassemble(packet, "d", &targetId);

			if(User *pTarget = g_UserDB.GetUserByServerId(targetId)->SafeCastUser())
			{
				if(pTarget->pSD->nStoreMode != 0)
				{
					pUser->SendSystemMessage(L"You cannot trade with player who has a private store!");
					unguard;
					return false;
				}

				if(pUser->pSD->pExData->territoryData.territoryId[TRADE_GEODATA_CHECK_ZONE] != 0)
				{
					if(!CGeo::GetInstance()->CanSee(&pUser->pSD->Pos, &pTarget->pSD->Pos))
					{
						pSocket->SendSystemMessage(181);	//Cannot see target!
						unguard;
						return false;
					}
				}

				if(g_PetBlockTrade && pTarget->GetPet())
				{
					//2501	1	a,You cannot trade with character who got a summoned pet!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pSocket->SendSystemMessage(2501);
					unguard;
					return false;
				}

				if(!pUser->IsNowTrade() && !pTarget->IsNowTrade())
				{
					pUser->pCreatureController->AsyncDoNothing();

					ret = UserSocketCallback(0x87C930L)( pSocket, packet);
				}else
				{
					ret = false;
				}
			}else
			{
				pSocket->SendSystemMessage(109);	//invalid target
				unguard;
				return false;
			}
		}else
		{
			ret = false;
		}
	}

	unguard;
	return ret;
}

bool TradeStartPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->pSD->pExData->territoryData.territoryId[FORBIDDEN_TRADE_ZONE]!=0)
		{
			pUser->SendSystemMessage(L"You're not allowed to trade here.");
			ret = false;
		}else
		{
			if(g_PetBlockTrade && pUser->GetPet())
			{
				//2500	1	a,You cannot use trade while your pet is summoned!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pSocket->SendSystemMessage(2500);
				unguard;
				return false;
			}

			if(pUser->IsNowTrade() || pUser->pSD->nStoreMode != 0 || pUser->olympiadUser.IsInside())
			{
				pUser->SendSystemMessage(L"You cannot do that action in current state.");
				pUser->ActionFailed();
				unguard;
				return false;
			}
			
			if(pUser->pED->isLocked)
			{
				unguard;
				return false;
			}

			pUser->pCreatureController->AsyncDoNothing();

			ret = UserSocketCallback(0x8605A0L)( pSocket, packet);
		}
	}

	unguard;
	return ret;
}

bool UserExPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	
	bool ret = true;
	if(User *pUser = pSocket->GetUser())
	{
		ret =  CUserExPacket::Handle(pSocket, packet);
	}

	unguard;
	return ret;
}

bool WarehouseDepositPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;
			int nAmount = 0;
			const unsigned char* pck = Disassemble(packet, "d", &nAmount);
			
			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
				return false;
				unguard;
			}

			CWareHouseDeposit *pWareHouse = pUser->GetWareHouseDeposit();
			if(pWareHouse->IsValid())
			{
				if(nAmount < 88 && nAmount > 0)
				{
					struct ItemPckData
					{
						INT32 dbId;
						INT32 count;
					};

					//check for duplicated dbid
					ItemPckData* data = (ItemPckData*)pck;
					CPet *pPet = pUser->GetPet();

					for(int n=0;n<nAmount;n++)
					{
						if(data[n].dbId != 0 && data[n].count > 0 && data[n].count < INT_MAX)
						{
							CItem *pItem = pUser->inventory.GetItemByDBID(data[n].dbId);
							if(pItem->IsValidItem() && data[n].count <= pItem->pSID->nItemAmount)
							{
								if(pItem->pSID->nItemID == 8190 || pItem->pSID->nItemID == 8689)
								{
									pSocket->SendSystemMessage(L"You cannot deposit cursed weapon.");
									unguard;
									return false;
								}
								if(pPet->IsPet())
								{
									if(pPet->pSD->nPetDBID == pItem->pSID->nDBID)
									{
										pSocket->SendSystemMessage(L"You cannot deposit collar when pet is summoned.");
										unguard;
										return false;
									}
								}

								if(pItem->nManaLeft || pItem->nLifeTime)
								{
									pUser->SendSystemMessage(L"You cannot deposit temporary item in warehouse.");
									unguard;
									return false;
								}
								for(int m=(n+1);m<nAmount;m++)
								{
									if(data[n].dbId == data[m].dbId)
									{
										g_Log.Add(CLog::Error, "[%s] Exploiter[%S] - Trying wh dupe exploit! Ban this user!", __FUNCTION__, pUser->pSD->wszName);
										unguard;
										return true;
									}
								}
							}else
							{
								pUser->SetItemListChanged();
								unguard;
								return false; //invalid item
							}
						}else
						{
							pUser->SetItemListChanged();
							unguard;
							return false;	//invalid dbid or count
						}
					}

					bool ret = _SockFunc(0x00861F50L)(pSocket, packet);
					
					if(!ret)
					{
						pUser->SetItemListChanged();
					}
					unguard;
					return ret;
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid warehouse deposit!", __FUNCTION__);
			}
		}
	}
	unguard;
	return false;
}

bool WarehouseWithdrawPacket(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	if(User *pUser = pSocket->GetUser())
	{
		UINT currentTick = GetTickCount();
		if(pSocket->pED->itemActionTimestamp < currentTick || ((pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pSocket->pED->itemActionTimestamp = currentTick + g_ItemPacketReuse;

			try
			{
				if(pUser->IsNowTrade())
				{
					pUser->TradeCancel();
					unguard;
					return false;
				}
				int nAmount = 0;
				const unsigned char* pck = Disassemble(packet, "d", &nAmount);

				CWareHouseWithdraw *pWareHouse = pUser->GetWareHouseWithdraw();
				if(pWareHouse->IsValid())
				{
					if(nAmount > 0 && nAmount < 200)
					{
						struct ItemPckData
						{
							INT32 dbId;
							INT32 count;
						};

						//check for duplicated dbid
						ItemPckData* data = (ItemPckData*)pck;
						for(int n=0;n<nAmount;n++)
						{
							if(data[n].dbId != 0 && data[n].count > 0 && data[n].count < INT_MAX)
							{
								for(int m=(n+1);m<nAmount;m++)
								{
									if(data[n].dbId == data[m].dbId)
									{
										g_Log.Add(CLog::Error, "[%s] Exploiter[%S] - Trying wh dupe exploit! Ban this user!", __FUNCTION__, pUser->pSD->wszName);
										unguard;
										return true;
									}
								}
							}else
							{
								unguard;
								return false;	//invalid dbid or count
							}
						}


						bool ret = Callback(0x008622C0L)(pSocket, packet);
						unguard;
						return ret;
					}
					unguard;
					return false;
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] Exception Handled", __FUNCTION__);
				unguard;
				return false;
			}
		}
		unguard;
		return false;
	}
	unguard;
	return true;
}

bool ProtocolVersion(CUserSocket *pSocket, const unsigned char* packet)
{
	guard;
	INT32 revision = 0;
	Disassemble(packet, "d", &revision);

	if(revision == -3 && g_BlockStatisticPacket)
	{
		unguard;
		return false;
	}else if(revision == -3 && g_SafeListStatisticPacket)
	{
		CIPAddress ip(pSocket->addr);
		if(!g_SafeIpList.Contain(ip))
		{
			unguard;
			return false;
		}
	}else if(revision == -4)
	{
		CIPAddress ip(pSocket->addr);
		if(g_SafeIpList.Contain(ip))
		{
			DWORD gameTime = g_L2Time.GetTime();
			BOOL isNight = g_L2Time.IsNight();
			pSocket->Send("cdd", 0x00, isNight, gameTime);
			unguard;
			return false;
		}
		unguard;
		return true;
	}
/*
 ToDo:
	if(g_Config.IsSet(CConfig::CLIENT_PROTECTION))
	{
		if(g_Config.clientProtectionInfo.IsCheckMac())
		{
			g_SocketDB.SetMacAddress(pSocket, packet);
		}
		in_addr Addr; 
		pSocket->GetAddress(&Addr);
		CIPAddress ip(Addr);
		CMacAddress mac = g_SocketDB.GetMac(pSocket);
		g_SocketDB.SetStatus(pSocket, CSocketData::ENTERING_WORLD);
		int nClients = g_SocketDB.GetIpMacCount(ip, mac);
		if( nClients > g_Config.clientProtectionInfo.GetMaxClients() )
		{
			if(!g_Config.clientProtectionInfo.CanDualBox(ip) && !g_Config.clientProtectionInfo.CanDualBox(mac))
			{
				g_Log.Add(CLog::Error, "[%s] ClientCount[%s][%d] > than limit [%d]", __FUNCTION__, ip.ToString().c_str(), nClients, g_Config.clientProtectionInfo.GetMaxClients());
				g_SocketDB.PrintAccountData(ip, mac);
				return true;
			}
		}
	}
*/
	bool ret = _SockFunc(0x8782D0)(pSocket, packet);
	unguard;
	return ret;
}
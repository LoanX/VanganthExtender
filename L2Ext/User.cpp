#include "StdAfx.h"
#include "DB.h"
#include "SocketDB.h"
#include "SkillAcquireDB.h"
#include "TerritoryData.h"
#include "ClanSys.h"
#include "MultiPartyCommandChannelRegistry.h"
#include "SchemeBuffer.h"
#include "PlayerAction.h"
#include "CreatureAction.h"
#include "HtmlFilter.h"
#include "Packets.h"
#include "License.h"


using namespace License;

extern map<INT32, ItemExSkillList> g_ItemExSkillList;
extern BOOL g_ValidateAcquiredSkills;
extern BOOL g_SubStackServer;
extern BOOL g_DeleteInvalidStack;
extern INT32 g_MaxPartyLevelRange;
extern BOOL g_CustomAbnormalInfoPacket;
extern WCHAR g_LastSentHtml[32][260];

void User::ExtInit()
{
	//.text:000000000058A916 mov     edx, 2D78h
	BYTE pMem1[4] = { 0x78, 0x2D, 0x00, 0x00 };
	g_HookManager.WriteMemoryDWORD(0x58A917, sizeof(User), pMem1);
	//.text:000000000058A950 mov     dword ptr [r14+rcx*4+6C52320h], 2D78h
	g_HookManager.WriteMemoryDWORD(0x58A958, sizeof(User), pMem1);
	//.text:000000000058A95E mov     r8d, 2D78h
	g_HookManager.WriteMemoryDWORD(0x58A960, sizeof(User), pMem1);
	
	g_HookManager.WriteMemoryDWORD(0xA87490, (DWORD)User::OnCreate);
	g_HookManager.WriteMemoryDWORD(0xA871D0, (DWORD)User::OnDestruct);
	g_HookManager.WriteCall(0x850380, User::OnDelete, 0);
}

void User::OnCreate(User *pUser)
{
	guard;

	typedef void (*f)(User*);
	f(0x7FD640L)(pUser);

	if(pUser)
	{
		pUser->pED = new UserExtData;
		if(pUser->pED)
		{
			pUser->pED->autoLoot = false;
			pUser->pED->userInfoChanged = FALSE;
			pUser->pED->charInfoChanged = FALSE;
			pUser->pED->itemListChanged = FALSE;
			pUser->pED->abnormalStatusChanged = FALSE;
			pUser->pED->validateParametersRequest = FALSE;
			pUser->pED->spawnProtection = 0;
			pUser->pED->spawnProtectionTimeout = 0;
			pUser->pED->spawnProtectionTimeSent = 0;
			pUser->pED->cursedWeaponId = 0;
			pUser->pED->cursedWeaponLevel = 0;
			pUser->pED->miningAreaId = 0;
			pUser->pED->charmOfCourageLevel = 0;
			pUser->pED->nicknameColor = 0;
			pUser->pED->titleColor = 0;
			pUser->pED->antiPkLevelRange = 0;
			pUser->pED->itemDeliveryTimestamp = 0;
			pUser->pED->invalidTick = 0;
			pUser->pED->aioId = 0;
			pUser->pED->aioTimestamp = 0;
			pUser->pED->aioNicknameColor = 0;
			pUser->pED->auctionUser.createTimeout = 0;
			pUser->pED->auctionUser.itemClassId = 0;
			pUser->pED->auctionUser.itemServerId = 0;
			pUser->pED->auctionUser.pendingAuction = FALSE;
			pUser->pED->auctionUser.paymentAskTimestamp = 0;
			pUser->pED->bonusCon = 0;
			pUser->pED->bonusDex = 0;
			pUser->pED->bonusInt = 0;
			pUser->pED->bonusMen = 0;
			pUser->pED->bonusStr = 0;
			pUser->pED->bonusWit = 0;
			memset(pUser->pED->autoInfinitySoulshot, 0, sizeof(pUser->pED->autoInfinitySoulshot));
			memset(pUser->pED->autoInfinitySpiritshot, 0, sizeof(pUser->pED->autoInfinitySpiritshot));
			pUser->pED->multiSellGroupId = 0;
			pUser->pED->voteRewardMsgRequest = 0;
			pUser->pED->voteRewardPoints = 0;
			pUser->pED->voteRewardTick = 0;
			pUser->pED->leftWorld = FALSE;
			pUser->pED->nameColorEx = 0;
			pUser->pED->titleColorEx = 0;
			pUser->pED->vipChatTimestamp = 0;
			pUser->pED->broadcastCombatModeTimestamp = 0;
			ZeroMemory(pUser->pED->lockHash, 32);
			ZeroMemory(pUser->pED->tempLockHash, 32);
			pUser->pED->isLocked = false;
			pUser->pED->lockAttempts = 0;
			pUser->pED->dailyPvPAmount = 0;
			pUser->pED->dailyPvPTimestamp = 0;
			pUser->pED->requestSaveAllSkills = false;
			memset(pUser->pED->chatTimestamp, 0, sizeof(pUser->pED->chatTimestamp));
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot allocate user ext data!", __FUNCTION__);
		}
	}

	unguard;
}

User *User::OnDestruct(User *pUser, DWORD param)
{
	typedef User*(*f)(User*, DWORD);
	
	if(pUser->pED)
	{
		delete pUser->pED;
		pUser->pED = 0;
	}

	return f(0x84F260L)(pUser, param);
}

void User::OnDelete(LPVOID lpInstance, UINT userId, const char *file, int line)
{
	typedef void(*f)(LPVOID, UINT, const char*, int);
	f(0x8404B0L)(lpInstance, userId, file, line);

	g_UserDB.DelUser(userId);
}

CPledge * User::GetPledge()
{
	if(this)
	{
		typedef CPledge *(*f) (User *);
		return f(0x00410C00L)(this);
	}
	return NULL;
}

CUserSocket * User::GetSocket()
{
	if(this)
	{
		try
		{
			return pSocket;
		}
		catch(...)
		{
			return NULL;
		}
	}
	return NULL;
}

CWareHouseDeposit *User::GetWareHouseDeposit()
{
	if(this)
	{
		try
		{
			return (CWareHouseDeposit*)*(PUINT64)((UINT64)this + 0x2910);
		}
		catch(...)
		{
			return NULL;
		}
	}
	return NULL;
}
CWareHouseWithdraw *User::GetWareHouseWithdraw()
{
	if(this)
	{
		try
		{
			return (CWareHouseWithdraw*)*(PUINT64)((UINT64)this + 0x2910);
		}
		catch(...)
		{
			return NULL;
		}
	}
	return NULL;
}

void User::ActionFailed()
{
	typedef void(*f)(User*);
	f(0x800D00L)(this);
}

CParty * User::GetParty()
{
	if(this)
	{
		typedef CParty *(__fastcall * __GetParty)(User*);
		__GetParty _GetParty = (__GetParty) 0x0046E4D0;
		return _GetParty(this);
	}
	return NULL;
}

INT32 User::GetMPCCPartyAmount()
{
	guard;
	INT32 amount = 0;
	if(CParty *pParty = GetParty())
	{
		if(pParty->GetMaster() == this)
		{
			CMultiPartyCommandChannelSP channelSP;
			CMultiPartyCommandChannelRegistry::FindByMasterParty(channelSP, pParty);

			if(CMultiPartyCommandChannel* pChannel = channelSP.get())
			{
				amount = static_cast<INT32>(pChannel->parties.size());
			}
		}
	}
	unguard;
	return amount;
}

INT32 User::GetMPCCMemberDistance()
{
	guard;
	INT32 distance = 0;
	try
	{
		if(CParty *pParty = GetParty())
		{
			if(pParty->GetMaster() == this)
			{
				CMultiPartyCommandChannelSP channelSP;
				CMultiPartyCommandChannelRegistry::FindByMasterParty(channelSP, pParty);
				if(CMultiPartyCommandChannel* pChannel = channelSP.get())
				{
					double x1 = pSD->Pos.x;
					double y1 = pSD->Pos.y;

					for(UINT n=0;n<pChannel->parties.size();n++)
					{
						if(CParty *pParty2 = CObject::GetObjectBySID(pChannel->parties[n])->SafeCastParty())
						{
							UserVector members;
							pParty2->GetAllMembersEx(members);
							for(UINT m=0;m<members.size();m++)
							{
								if(User *pMember = members[m]->SafeCastUser())
								{
									double x2 = pMember->pSD->Pos.x;
									double y2 = pMember->pSD->Pos.y;
									int dist = (int)sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
									if(dist > distance)
									{
										distance = dist;
									}
								}
							}
						}
					}
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	unguard;
	return distance;
}

bool User::IsNowTrade()
{
	if(this)
	{
		typedef bool (*f)(User*);
		return f(0x0080F840L)(this);
	}
	return false;
}

void User::SendSystemMessage(PWCHAR Talker, PWCHAR Message)
{
	if(this)
	{
		typedef void (*f)(User*, PWCHAR, PWCHAR);
		f(0x007FEC10)(this, Talker, Message);
	}
}

void User::SendSystemMessage(const wchar_t* Talker, const wchar_t* Message)
{
	if(this)
	{
		typedef void (*f)(User*, const wchar_t*, const wchar_t*);
		f(0x007FEC10)(this, Talker, Message);
	}
}

void User::SendUserInfo(CUserSocket* bGM)
{
	if(ValidUser())
	{
		typedef void (* f) (User*, CUserSocket*);
		f(0x855740L)(this, bGM);
	}
}

void User::SendCharInfo(CUserSocket* pGMSocket)
{
	if(ValidUser())
	{
		CPacketFix::CharInfoSetUser(this, pGMSocket);
	}
}

bool User::SendCoolTime()
{
	typedef bool(__fastcall *_Reset)(User*);
	_Reset Reset = (_Reset) 0x0082AA70;
	return Reset(this);
}

void User::SendItemList(bool show)
{
	typedef void(*sil)(User*, BYTE, WORD, INT64);
	sil _sil = (sil) 0x00819C40;
	_sil(this, 0, show, 0);
}

void User::SendRelationChanged(CUserSocket *pSocket)
{
	typedef void(*f)(User*, CUserSocket*);
	f(0x852B90L)(this, pSocket);
}

void User::SendAcquireSkillToDB(int nSkillID, int nLevel, bool bMayDown)
{
	typedef void (*f) (User*, int, int, bool);
	f(0x00801CD0L)(this, nSkillID, nLevel, bMayDown);
}

void User::SendDeleteSkillToDB(int nSkillID)
{
	typedef void (*f) (User*, int nSkillID);
	f(0x00801E40L)(this, nSkillID);
}

void User::SendHpInfo()
{
	typedef void (__fastcall * __SendHpInfo) (User*);
	__SendHpInfo _SendHpInfo = (__SendHpInfo) 0x008464A0;
	_SendHpInfo(this);
}

void User::SendMpInfo()
{
	typedef void (__fastcall * __SendMpInfo) (User*);
	__SendMpInfo _SendMpInfo = (__SendMpInfo) 0x0080ECE0;
	_SendMpInfo(this);
}

void User::SetKarma(int nKarma)
{
	typedef void (__fastcall * __SetKarma)(User*, int nKarma);
	__SetKarma _SetKarma = (__SetKarma) 0x00812B10;
	_SetKarma(this, nKarma);
}
bool User::PacketAcceptable(int nPacketNo)
{
	typedef bool(__fastcall *__pa)(User*, int nPacketNo);
	__pa _pa = (__pa) 0x00805FB0;
	return _pa(this, nPacketNo);
}

void User::SendQuestList()
{
	typedef void (__fastcall *__sql)(User*, DWORD dNull);
	__sql _sql = (__sql) 0x0081EB20;
	_sql(this, 0);
}
bool User::TeleportToLocation(int x, int y, int z, bool bNotFadeScreen)
{
	typedef bool (__fastcall *__ttl)(User*, int, int, int, bool);
	__ttl _ttl = (__ttl) 0x008151E0;
	return _ttl(this, x, y, z, bNotFadeScreen);
}

void User::TradeCancel()
{
	typedef void(*f)(User*);
	f(0x0080F370L)(this);
}

void User::ChangeTarget(CCreature *pObject, bool ByUser)
{
	typedef void(__fastcall *__ct)(CCreature*, CCreature *, bool ByUser);
	__ct _ct = (__ct) 0x00800E30;
	_ct(this, pObject, ByUser);
}

void User::SendSystemMessage(int msgId)
{
	if(this)
	{
		pSocket->SendSystemMessage(msgId);
	}
}


void User::SendSystemMessage(PWCHAR wszMessage)
{
	if(this)
	{
		pSocket->SendSystemMessage(wszMessage);
	}
}

void User::SendSystemMessage(const WCHAR* wszMessage)
{
	if(this)
	{
		pSocket->SendSystemMessage(wszMessage);
	}
}

void User::SendSystemMessage(CSystemMessage *pMsg)
{
	if(this)
	{
		if(pSocket)
		{
			pSocket->Send("b", pMsg->GetSize(), pMsg->GetBuff());
		}
	}
}
CSummon *User::GetSummonOrPet()
{
	typedef CSummon *(__fastcall *__gp)(User*);
	__gp _gp = (__gp) 0x00812680;
	return _gp(this);
}

CSummon *User::GetSummon()
{
	typedef CSummon* (*f)(User*);
	return f(0x8122F0L)(this);
}

void User::SendETCBuffStatus()
{
	typedef void (*f) (User*);
	f(0x007FFEB0L)(this);
}

void User::SendAbnormalStatusInfo()
{
	guard;

	if(g_CustomAbnormalInfoPacket)
	{
		CHAR buffBuff[8190];
		int buffLen = 0;
		CHAR debuffBuff[8190];
		int debuffLen = 0;
		CHAR songDanceBuff[8190];
		int songDanceLen = 0;
		CHAR toggleBuff[8190];
		int toggleLen = 0;
		INT32 abnormalCount = 0;
		__time32_t currentTime = _time32(0);
		abnormalStatusLock.Lock(__FILEW__, __LINE__);
		//add buffs
		for(INT32 n=0;n<vAbnormalStatus.size();n++)
		{
			CSkillInfo *pSI  = vAbnormalStatus[n].pSI;
			AbnormalData& ad = vAbnormalStatus[n];
			if(pSI)
			{
				
				__time32_t timeLeft = currentTime - ad.expireTime;
				timeLeft = pSI->nAbnormalTime - timeLeft;
				if(timeLeft < 0)
				{
					timeLeft = 0;
				}
				if(pSI->nDebuff != 1)
				{
					if(pSI->IsSongOrDance())
					{
						songDanceLen += Assemble(&songDanceBuff[songDanceLen], (8190 - songDanceLen), "dhd", pSI->nSkillID, pSI->nLevel, timeLeft);
						abnormalCount++;
					}else
					{
						buffLen += Assemble(&buffBuff[buffLen], (8190 - buffLen), "dhd", pSI->nSkillID, pSI->nLevel, timeLeft);
						abnormalCount++;
					}
				}else
				{
					debuffLen += Assemble(&debuffBuff[debuffLen], (8190 - debuffLen), "dhd", pSI->nSkillID, pSI->nLevel, timeLeft);
					abnormalCount++;
				}
			}
		}

		abnormalStatusLock.Unlock();

		ToggleSkillLock.Lock(__FILEW__, __LINE__);

		for(ToggleSkillMap::iterator it = mToggleSkill.begin(); it != mToggleSkill.end(); it++)
		{
			if(it->second.InUse)
			{
				toggleLen += Assemble(&toggleBuff[toggleLen], (8190 - toggleLen), "dhd", it->second.pSI->nSkillID, it->second.pSI->nLevel, -1);
				abnormalCount++;
			}
		}

		ToggleSkillLock.Unlock();

		pSocket->Send("chbbbb", 0x7F, abnormalCount, buffLen, buffBuff, songDanceLen, songDanceBuff, debuffLen, debuffBuff, toggleLen, toggleBuff);
	}else
	{
		typedef void (*f) (User*);
		f(0x827C20L)(this);
	}
	unguard;
}

void User::SendSpelledInfoChanged()
{
	typedef void(*f)(User*);
	f(0x853CA0L)(this);
}

void User::SendSkillList(bool bNothing, CSocket *pGMSocket)
{
	typedef void (*f)(User*, bool, CSocket*);
	f(0x00828500L)(this, bNothing, pGMSocket);
}

void User::SaveInfectedSkills()
{
	typedef void (__fastcall * __SIS)(User*);
	__SIS _SIS = (__SIS) 0x00842F60;
	if(this)
		_SIS(this);
}

void User::DespawnSummon()
{
	typedef void(__fastcall *_dI)(User*);
	_dI dI = (_dI) 0x00812510;
	return dI(this);
}

bool User::InCombat()
{
	if(ValidUser())
		return inCombatMode;
	else
		return false;
}

DWORD User::CombatTime()
{
	if(ValidUser())
	{
		return combatModeTimeStamp;
	}
	return NULL;
}

void User::IncreaseDuelCount()
{
	typedef void(*f)(User*);
	f(0x814A10L)(this);
}

bool User::InCombatZone()
{
	if(ValidUser())
	{
		return inPvpZone;
	}else 
		return false;
}

bool User::CanEquip(CItem *pItem)
{
	typedef bool (*f)(User*, CItem*);
	return f(0x815900L)(this, pItem);
}

bool User::CanGetPartyBonus(CCreature *pCreature)
{
	guard;

	try
	{
		if(this && pCreature)
		{
			if(CParty *pParty = GetParty())
			{
				UserVector vMembers;
				pParty->GetAllMembersEx(vMembers, true);
				for(INT32 n=0;n<vMembers.size();n++)
				{
					if(User *pMember = vMembers[n]->GetUser())
					{
						if(pMember->nObjectID != nObjectID )
						{
							int levelRange = abs(pMember->pSD->nLevel - pSD->nLevel);
							if( levelRange <= g_MaxPartyLevelRange && pMember->IsInRange(pCreature, 1500))
							{
								unguard;
								return true;
							}
						}
					}
				}
			}
		}
	}catch(...)
	{
	}

	unguard;
	return false;
}

bool User::UseItem(CItem *pItem, int nForce)
{
	typedef bool (__fastcall * UseItemS) (User*, CItem*, int nForce);
	UseItemS _UseItem = (UseItemS) 0x0080E160;
	if(pItem)
		return _UseItem(this, pItem, nForce);
	else return false;
}

bool User::IsItemUsable()
{
	typedef bool (*f)(User*);
	return f(0x811EB0L)(this);
}

bool User::DropItem(CItem *pItem, int nAmount)
{
	typedef bool (__fastcall * __DropItem)(User*, CItem*, int nAmount, FVector*);
	__DropItem _DropItem = (__DropItem) 0x00800350;
	if(pItem)
		return _DropItem(this, pItem, nAmount, &pSD->Pos);
	else return false;
}
bool User::UnEquipItem(int nSlotType)
{
	typedef bool(__fastcall *_EquipI)(User*, int nSlotType);
	_EquipI EquipI = (_EquipI) 0x00819960;
	return EquipI(this, nSlotType);
}
int User::ExpDown(int nExpDec)
{
	typedef int (*f)(User*, int);
	return f(0x8040F0L)(this, nExpDec);
}

void User::SetExpDown(int nExpDec)
{
	try
	{
		expDrop = nExpDec;
	}
	catch(...)
	{
		return;
	}
}

void User::IncreaseKarma(bool bWithPk)
{
	typedef void (__fastcall *__IK)(User*, bool bWithPkCount);
	__IK _IK = (__IK) 0x008146A0;
	_IK(this, bWithPk);
}

CSharedNoblessInfo *User::GetNoblessInfo()
{
	typedef CSharedNoblessInfo*(__fastcall *__GetInfo)(User*);
	__GetInfo _GetInfo = (__GetInfo) 0x008177D0;
	return _GetInfo(this);
}

void User::ResetJoinPledgePenalty()
{
	typedef void(__fastcall *_Reset)(User*);
	_Reset Reset = (_Reset) 0x00803FB0;
	Reset(this);
}

bool User::ChangeClass(int nClass)
{
	typedef bool(*f)(User*, int);
	return f(0x0080E970L)(this, nClass);
}

bool User::CanJoinPledge()
{
	typedef bool (*cjp)(User*);
	cjp _cjp = (cjp) 0x008042C0;
	return _cjp(this);
}

void User::DespawnPet(bool byLeaveWorld)
{
	typedef void (*cjp)(User*, bool);
	cjp _cjp = (cjp) 0x00816C50;
	_cjp(this, byLeaveWorld);
}

void User::PlaySound(PWCHAR wszName, int nCount, int nObjectID, int x, int y, int z)
{
	if(ValidUser())
	{
		if(CSocket *pSocket = GetSocket())
		{
			pSocket->Send("cdSdddddd", 0x98, 0, wszName, nCount, nObjectID, x, y, z, 0);
		}
	}
}

void User::Ride(CCreature *pCreature)
{
	typedef void(__fastcall *__ODDI)(User*, CCreature*);
	__ODDI _ODDI = (__ODDI) 0x00834AD0;
	_ODDI(this, pCreature);
}

void User::RideWyvern(int wyvernClassId, int level)
{
	typedef void (*f)(User*, int, int);
	f(0x833E40L)(this, wyvernClassId, level);
}

int User::GetAugmentationID()
{
	if(ValidUser())
	{
		try
		{
			CSharedItemData *pData = this->GetEquipedWeapon();
			if(pData)
			{
				CWeapon *pWeapon = inventory.GetItemByIndex(pData->nItemIndex)->GetWeapon();
				if(pWeapon)
				{
					if(g_Config.AugmentationInfo.GetBlockedGlowCount() > 0)
					{
						int nItemID = pData->nItemID;
						if(g_Config.AugmentationInfo.IsBlockedGlow(nItemID))
							return NULL;
					}
					return pWeapon->nAugmentationID;
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception!",__FUNCTION__);
		}
	}
	return NULL;
}

int User::GetAugmentationID(int nItemIndex)
{	
	if(this)
	{
		try
		{
			CItem *pItem = inventory.GetItemByIndex(nItemIndex);
			if(pItem)
				return pItem->nAugmentationID;
		}catch(...)
		{
			return NULL;
		}
	}
	return NULL;
}

int User::GetPledgeType()
{
	if(ValidUser())
	{
		return pSD->nPledgeType;
	}
	return NULL;
}

int User::GetPledgeReputation()
{
	if(ValidUser())
	{
		if(CPledge* pPledge = GetPledge())
		{
			if(pPledge->pSD)
			{
				return pPledge->pSD->nReputation;
			}
		}
	}
	return 0;
}

UINT User::GetPledgeClass()
{	
	UINT nRank = VAGABOND;
	if( ValidUser() && pSD )
	{
		CPledge *pPledge = GetPledge();
		if(pPledge && pPledge->pSD)
		{
			int nClanLevel = pPledge->pSD->nLevel;
			nRank = VASSAL;
			switch(nClanLevel)
			{
			case 4:
				{
					if(pSD->nIsPledgeLeader > 0)
						nRank = KNIGHT;
					break;
				}
			case 5:
				{
					if(pSD->nIsPledgeLeader > 0)
						nRank = ELDER;
					else
						nRank = HEIR;
					break;
				}
			case 6:
				{
					switch(pSD->nPledgeType)
					{
					case CPledge::ACADEMY:
						{
							nRank = VASSAL;
							break;
						}
					case CPledge::ROYAL_GUARD_I:
					case CPledge::ROYAL_GUARD_II:
						{
							nRank = HEIR;
							break;
						}
					default:
						{
							if(pSD->nIsPledgeLeader > 0)
								nRank = BARON;
							else
								nRank = KNIGHT;
							break;
						}
					}
					int nSubUnit = pPledge->IsCaptainOfSubpledge(pSD->wszName);
					if(nSubUnit == CPledge::ROYAL_GUARD_I || nSubUnit == CPledge::ROYAL_GUARD_II)
					{
						nRank = ELDER;
					}
					break;
				}
			case 7:
				{
					switch(pSD->nPledgeType)
					{
					case CPledge::ACADEMY:
						{
							nRank = VASSAL;
							break;
						}
					case CPledge::ROYAL_GUARD_I:
					case CPledge::ROYAL_GUARD_II:
						{
							nRank = KNIGHT;
							break;
						}
					case CPledge::ORDER_OF_KNIGHTS_I:
					case CPledge::ORDER_OF_KNIGHTS_II:
					case CPledge::ORDER_OF_KNIGHTS_III:
					case CPledge::ORDER_OF_KNIGHTS_IV:
						{
							nRank = HEIR;
							break;
						}
					default:
						{
							if(pSD->nIsPledgeLeader > 0)
								nRank = COUNT;
							else
								nRank = ELDER;
							break;
						}
					}
							
					int nSubUnit = pPledge->IsCaptainOfSubpledge(pSD->wszName);
					if(nSubUnit == CPledge::ROYAL_GUARD_I || nSubUnit == CPledge::ROYAL_GUARD_II)
					{
						nRank = VISCOUNT;
					}else if( nSubUnit >= CPledge::ORDER_OF_KNIGHTS_I && nSubUnit <= CPledge::ORDER_OF_KNIGHTS_IV)
					{
						nRank = BARON;
					}
					break;
				}
			case 8:
				{
					switch(pSD->nPledgeType)
					{
					case CPledge::ACADEMY:
						{
							nRank = VASSAL;
							break;
						}
					case CPledge::ROYAL_GUARD_I:
					case CPledge::ROYAL_GUARD_II:
						{
							nRank = ELDER;
							break;
						}
					case CPledge::ORDER_OF_KNIGHTS_I:
					case CPledge::ORDER_OF_KNIGHTS_II:
					case CPledge::ORDER_OF_KNIGHTS_III:
					case CPledge::ORDER_OF_KNIGHTS_IV:
						{
							nRank = KNIGHT;
							break;
						}
					default:
						{
							if(pSD->nIsPledgeLeader > 0)
								nRank = MARQUIS;
							else
								nRank = BARON; //Direct Member
							break;
						}
					}
					int nSubUnit = pPledge->IsCaptainOfSubpledge(pSD->wszName);
					if(nSubUnit == CPledge::ROYAL_GUARD_I || nSubUnit == CPledge::ROYAL_GUARD_II)
					{
						nRank = COUNT;
					}else if( nSubUnit >= CPledge::ORDER_OF_KNIGHTS_I && nSubUnit <= CPledge::ORDER_OF_KNIGHTS_IV)
					{
						nRank = VISCOUNT;
					}
					break;
				}
			default:
				{
					nRank = VASSAL;
					break;
				}
			}
		}
		if(nRank < 5)
			if(pSD->nNobless)
				nRank = BARON;
		if(pSD->nHero == 2)
			nRank = MARQUIS;
	}
	return nRank;
}

void User::OnDieDropItem(bool bDieOnBattleField)
{
	typedef void(__fastcall *__ODDI)(User*, bool bDieOnBattlefield);
	__ODDI _ODDI = (__ODDI) 0x0081C310;
	_ODDI(this, bDieOnBattleField);
}
PWCHAR User::GetTitle()
{
	return wTitle;
}
User *User::GetUserBySID(UINT *pSID)
{
	typedef User*(*f)(UINT*);
	return f(0x007F04E0L)(pSID);
}

int User::GetEquipedItemSlot(int nItemIndex)
{
	int nSlot = CItem::SlotType_None;
	if(this)
	{
		if(pSD)
		{
			for(int n=0;n<18;n++)
			{
				if( pSD->nEquipment[n] == nItemIndex)
				{
					return n;
				}
			}
		}
	}
	return nSlot;
}

void User::QuitPrivateStoreBuy()
{
	if(ValidUser())
	{
		typedef void (*f)(User*);
		f(0x8056B0L)(this);
	}
}

void User::QuitPrivateStore()
{
	if(ValidUser())
	{
		typedef void (*f)(User*);
		f(0x804580L)(this);
	}
}

void User::QuitRecipeStore()
{
	typedef void(*f)(User*);
	f(0x806DB0L)(this);
}

bool User::IsAcademyMember()
{
	bool bAcademyMember = false;
	if(this)
	{
		CPledge *pPledge = GetPledge();
		if(pPledge)
		{
			try
			{
				MemberData *pData = pPledge->GetMemberDataByDBID(this->nDBID);
				if(pData)
				{
					if(pData->nPledgeType == CPledge::ACADEMY)
						bAcademyMember = true;
				}
			}catch(...)
			{
			}
		}
	}
	return bAcademyMember;
}

void User::GetOff(bool bForce)
{
	typedef void(*_getoff)(User*, bool);
	_getoff getoff = (_getoff) 0x008354A0;
	getoff(this, bForce);
}

void User::ShowHTML(const WCHAR* filename, const WCHAR* buffer, int classID)
{
	PlayerAction::ShowHTML(this, filename, buffer, classID);
}

void User::SetSpelledInfoChanged()
{
	typedef void(*f)(User*);
	f(0x84C6B0L)(this);
}

void User::SetUserInfoChanged()
{
	InterlockedExchange(&pED->userInfoChanged, TRUE);
}

void User::SetCharInfoChanged()
{
	InterlockedExchange(&pED->charInfoChanged, TRUE);
}

void User::SetItemListChanged()
{
	InterlockedExchange(&pED->itemListChanged, TRUE);
}

void User::SetAbnormalStatusChanged()
{
	InterlockedExchange(&pED->abnormalStatusChanged, TRUE);
}
void User::SetLevelByExp(INT32 nExp)
{
	typedef void (*_f)(User*, INT32);
	_f f = (_f) 0x004D8AB0;
	f(this, nExp);
}

void User::SetLevel(int nLevel)
{
	if(nLevel > 0 && nLevel < 81)
	{
		INT32 nExp = g_UserDB.GetExp(nLevel);
		this->WriteLock(__FILEW__, __LINE__);
		this->SetLevelByExp(nExp);
		this->WriteUnlock();
	}
}
bool User::DoExtraValidation()
{
	typedef bool (__fastcall * __SIS)(User*);
	__SIS _SIS = (__SIS) 0x007FCDB0;
	return _SIS(this);
}
bool User::HaveMemo(int nQuestID)
{
	if(this->ValidUser())
	{
		for(int n=0;n<26;n++)
		{
			if(this->pSD->Quests[n].questId == nQuestID)
				return true;
		}
	}
	return false;
}
int User::GetMemoState(int nQuestID)
{
	if(this->ValidUser())
	{
		for(int n=0;n<26;n++)
		{
			if(this->pSD->Quests[n].questId == nQuestID)
				return this->pSD->Quests[n].questState;
		}
	}
	return NULL;
}

void User::SetMemoState(int nQuestID, int nState)
{
	if(this->ValidUser())
	{
		if(this->HaveMemo(nQuestID))
		{
			for(int n=0;n<26;n++)
			{
				if(this->pSD->Quests[n].questId == nQuestID)
				{
					this->pSD->Quests[n].questState = nState;
					return;
				}
			}
		}
	}
}
PrivateStoreItemMap* User::GetPrivateStoreSell()
{
	return (PrivateStoreItemMap*)((UINT64)this + 0x26B8);
}
PrivateStoreItemMap* User::GetPrivateStoreBuy()
{
	return (PrivateStoreItemMap*)((UINT64)this + 0x26D0);
}
PrivateStoreRecipeMap* User::GetPrivateStoreRecipe()
{
	return (PrivateStoreRecipeMap*)((UINT64)this + 0x26A0);
}
RecipeInfoMap* User::GetRecipeInfo()
{
	return (RecipeInfoMap*)((UINT64)this + 0x2688);
}

CRecipeInfo* User::GetRecipeInfo(int recipeId)
{
	typedef CRecipeInfo* (*f)(User*, int);
	return f(0x82B3E0L)(this, recipeId);
}

void User::DeleteItem(CItem *pItem)
{
	if(ValidUser() && pItem->IsValidItem())
	{
		if(IsNowTrade())
		{
			TradeCancel();
		}
		g_DB.RequestDestroyItem(pItem, 1, this);
	}
}

bool User::IsInCombatMode()
{
	if(this)
	{
		if(inCombatMode && ((combatModeTimeStamp + 30) > time(0)) )
		{
			return true;
		}
	}
	return false;
}

CPet* User::GetPet()
{
	typedef CPet* (*_f)(User*);
	_f f = (_f) 0x008120B0;
	return f(this);
}

void User::SaveAcquiredSkills()
{
	guard;
	acquiredSkillLock.Lock(__FUNCTIONW__, __LINE__);
	for(map<int, int>::iterator Iter = mAcquiredSkill.begin(); Iter!=mAcquiredSkill.end();Iter++)
	{
		if(g_SkillAcquireDB.IsInDatabase(Iter->first))
		{
			if(pED->subStackInfo.classId[pSD->nSubClassIndex][0] != UINT_MAX)
			{
				//player has a sub stack
				if(g_SkillAcquireDB.CanAcquire(pED->subStackInfo.classId[pSD->nSubClassIndex][0], pSD->nLevel, Iter->first, Iter->second) || g_SkillAcquireDB.CanAcquire(pED->subStackInfo.classId[pSD->nSubClassIndex][1], pSD->nLevel, Iter->first, Iter->second))
				{
					//save the skill in db
					SendAcquireSkillToDB(Iter->first, Iter->second, false);
				}

			}else if(g_SkillAcquireDB.CanAcquire(pSD->nClass, pSD->nLevel, Iter->first, Iter->second))
			{
				SendAcquireSkillToDB(Iter->first, Iter->second, false);
			}
		}
	}
	acquiredSkillLock.Unlock();
	unguard;
}

void User::ValidateAcquiredSkills(bool log)
{
	guard;

	list<int> lToDelete;
	int nClassID = pSD->nClass;
	int nLevel = pSD->nLevel;
	if(g_SubStackServer)
	{
		nLevel = 80;
		
		acquiredSkillLock.Lock(__FUNCTIONW__, __LINE__);
		for(map<int, int>::iterator Iter = mAcquiredSkill.begin(); Iter!=mAcquiredSkill.end();Iter++)
		{
			if(g_SkillAcquireDB.IsInDatabase(Iter->first))
			{
				if(!g_SkillAcquireDB.CanAcquire(pSD->nSubJobClass[0], nLevel, Iter->first, Iter->second) &&
					!g_SkillAcquireDB.CanAcquire(pSD->nSubJobClass[1], nLevel, Iter->first, Iter->second) &&
					!g_SkillAcquireDB.CanAcquire(pSD->nSubJobClass[2], nLevel, Iter->first, Iter->second) &&
					!g_SkillAcquireDB.CanAcquire(pSD->nSubJobClass[3], nLevel, Iter->first, Iter->second))
				{
					lToDelete.push_back(Iter->first);
					if(log)
					{
						g_Log.Add(CLog::Error, "[%s] User[%S] has invalid skill[%d][%d]!", __FUNCTION__, pSD->wszName, Iter->first, Iter->second);
					}
				}
			}
		}
		acquiredSkillLock.Unlock();

	}else
	{
		acquiredSkillLock.Lock(__FUNCTIONW__, __LINE__);
		for(map<int, int>::iterator Iter = mAcquiredSkill.begin(); Iter!=mAcquiredSkill.end();Iter++)
		{
			if(g_SkillAcquireDB.IsInDatabase(Iter->first))
			{
				if(pED->subStackInfo.classId[pSD->nSubClassIndex][0] != UINT_MAX)
				{
					//player has a sub stack
					if(!g_SkillAcquireDB.CanAcquire(pED->subStackInfo.classId[pSD->nSubClassIndex][0], nLevel, Iter->first, Iter->second) && !g_SkillAcquireDB.CanAcquire(pED->subStackInfo.classId[pSD->nSubClassIndex][1], nLevel, Iter->first, Iter->second))
					{
						lToDelete.push_back(Iter->first);
						if(log)
						{
							g_Log.Add(CLog::Error, "[%s] User[%S] has invalid skill[%d][%d] maxNotEnchanted[%d]!", __FUNCTION__, pSD->wszName, Iter->first, Iter->second, g_SkillAcquireDB.GetMaxNotEnchantedSkillLevel(Iter->first));
						}
					}

				}else if(!g_SkillAcquireDB.CanAcquire(nClassID, nLevel, Iter->first, Iter->second))
				{
					lToDelete.push_back(Iter->first);
					if(log)
					{
						g_Log.Add(CLog::Error, "[%s] User[%S] has invalid skill[%d][%d] maxNotEnchanted[%d]!", __FUNCTION__, pSD->wszName, Iter->first, Iter->second, g_SkillAcquireDB.GetMaxNotEnchantedSkillLevel(Iter->first));
					}
				}
			}
		}
		acquiredSkillLock.Unlock();
	}
	
	for(list<int>::iterator Iter = lToDelete.begin(); Iter!=lToDelete.end();Iter++)
	{
		int skillId = (*Iter);
		int skillLevel = g_SkillAcquireDB.GetMaxAcquirableLevel(nClassID, nLevel, skillId);
		if(g_SubStackServer)
		{
			for(int n=0;n<4;n++)
			{
				int tmpLevel = g_SkillAcquireDB.GetMaxAcquirableLevel(pSD->nSubJobClass[n], nLevel, skillId);
				if(tmpLevel > skillLevel)
					skillLevel = tmpLevel;
			}
		}
		if(skillLevel > 0)
		{
			AcquireSkill(skillId, skillLevel, TRUE);
			SendAcquireSkillToDB(skillId, skillLevel, true);
		}else
		{
			DeleteSkill(*Iter);
			SendDeleteSkillToDB(*Iter);
		}
	}
	ValidateSkillList();
	SetValidateParametersRequest();
	unguard;
}

int User::GetTradeSID()
{
	if(ValidUser())
	{
		return tradeId;
	}
	return NULL;
}

int User::GetAccountID() 
{
	if(ValidUser())
	{
		return (*(LPINT)((UINT64)this + (UINT64)0x27F8));
	}
	return NULL;
}

void User::SetCombatMode()
{
	if(ValidUser())
	{
		inCombatMode = true;
		CIOObject::SetTimeStamp((long*)&combatModeTimeStamp);
		BroadcastCombatModeStart();
		SendUserInfo(false);
		SendCharInfo(false);
	}
}

void User::SetGuilty(bool c)
{
	typedef void (*f)(User*, bool);
	f(0x815030L)(this, c);
}

void User::SetPvPFlag()
{
	if(ValidUser())
	{
		CIOObject::SetTimeStamp(&pSD->nPvPAttackTimeStamp);
		SetGuilty(true);
	}
}

void User::Kick()
{
	if(pSocket)
	{
		StopMove();
		pSocket->Close();
	}
}

void User::SetStopSay(DWORD timeInMs, bool saveInDB)
{
	typedef void(*f)(User*, DWORD, bool);
	return f(0x8031F0L)(this, timeInMs, saveInDB);
}

void User::KillMyself()
{
	if( ValidUser() )
	{
		(*(LPDWORD)((UINT64)this+0x2958)) = 5;
		typedef bool (*f)(User*, CCreature*);
		pSD->nCursedVictim = 1;
		f(0x81DF00)(this, NULL); //User::Die(CCreature *pEnemy)
	}
}

void User::SendRuneInfo()
{
	double itemRateBonus = 1.0;
	double adenaBonus = 1.0;
	double raidBossBonus = 1.0;
	double raidBossOnlyChanceBonus = 1.0;
	double epicBossBonus = 1.0;
	double onlyChanceBonus = 1.0;

	GetDropBonuses(itemRateBonus, adenaBonus, raidBossBonus, raidBossOnlyChanceBonus, epicBossBonus, onlyChanceBonus);
	wstringstream msg;
	msg << L"========== Rune Status ==========" << endl;
	
	if(pSD->pExData->SkillMod.bonusExp != 1.0)
		msg << L"Exp: " << (INT32)( (pSD->pExData->SkillMod.bonusExp * 100) - 100.0 ) << L" %\n";
	if(pSD->pExData->SkillMod.bonusSp != 1.0)
		msg << L"SP: " << (INT32)( (pSD->pExData->SkillMod.bonusSp * 100) - 100.0) << L" %\n";
	if(itemRateBonus != 1.0)
		msg << L"Drop (Chance&Amount): " << (INT32)((itemRateBonus * 100) - 100.0) << L" %\n";
	if(onlyChanceBonus != 1.0)
		msg << L"Drop (Chance): " << (INT32)((onlyChanceBonus * 100) - 100.0) << L" %\n";
	if(adenaBonus != 1.0)
		msg << L"Adena (Amount): " << (INT32)((adenaBonus * 100) - 100) << L" %\n";
	if(raidBossBonus != 1.0)
		msg << L"Raid Boss (Chance&Amount): " << (INT32)((raidBossBonus * 100) - 100) << L" %\n";
	if(raidBossOnlyChanceBonus != 1.0)
		msg << L"Raid Boss (Chance): " << (INT32)((raidBossOnlyChanceBonus * 100) - 100) << L" %\n";
	if(epicBossBonus != 1.0)
		msg << L"Epic RB Rate (Chance): " << (INT32)((epicBossBonus * 100) - 100) << L" %\n";
	if(pSD->pExData->SkillMod.bonusSpoil != 1.0)
		msg << L"Spoil (Chance&Amount): " << (INT32)((pSD->pExData->SkillMod.bonusSpoil * 100) - 100) << L" %\n";
	if(pSD->pExData->SkillMod.bonusHarvesting != 1.0)
		msg << L"Harvesting Manor (Chance&Amount): " << (INT32)((pSD->pExData->SkillMod.bonusHarvesting * 100) - 100) << L" %\n";

	SendSystemMessage(msg.str().c_str());

	//2453	1	a,$s1 : $s2 %\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
	map<INT32, double> itemBonus;
	pED->itemBonus.GetAll(itemBonus);
	for(map<INT32, double>::iterator it = itemBonus.begin(); it!=itemBonus.end();it++)
	{	
		CSystemMessage msgPck(2453);
		msgPck.AddItem(it->first);
		msgPck.AddNumber((INT32)((it->second * 100.0) - 100.0));
		pSocket->SendSystemMessage(&msgPck);
	}

	pSocket->SendSystemMessage(L"=============================");
}

void User::SendLifeTimeItemsInfo()
{
	guard;

	CSharedItemData* lpSharedItemDataArray = (*(CSharedItemData**)0x0E412138);
	INT32 itemCount = 0;
	
	vector<pair<INT32, UINT>> messages;
	time_t currentTime = time(0);
	inventory.ReadLock(__FILEW__, __LINE__);
	INT32 index = inventory.GetFirstItemIndex();
	try
	{
		while(index > -1 && index < 900000 && itemCount < 200)
		{
			CSharedItemData *lpSID = &lpSharedItemDataArray[index];
			if(lpSID)
			{
				if(CItem *pItem = inventory.GetItemByIndex(index))
				{
					if(pItem->nLifeTime > 0)
					{
						messages.push_back(pair<INT32, UINT>(pItem->pSID->nItemID, pItem->nLifeTime));
					}
				}
			}
			index = lpSID->nNextIndex;
			itemCount++;
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}

	inventory.ReadUnlock();
	
	if(messages.size() > 0)
	{
		//2554	1	a,Fixed time:.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
		pSocket->SendSystemMessage(2554);

		for(UINT n=0;n<messages.size();n++)
		{
			pair<INT32, UINT>& data = messages[n];
			INT32 diff = data.second - currentTime;
			if(diff > 60)
			{
				INT32 seconds = diff % 60;
				diff /= 60;
				INT32 minutes = diff % 60;
				diff /= 60;
				INT32 hours = diff;

				//2553	1	a,$s1 will disappear in $s2 hour(s) and $s3 minute(s).\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				
				CSystemMessage msg(2553);
				msg.AddItem(data.first);
				msg.AddNumber(hours);
				msg.AddNumber(minutes);
				SendSystemMessage(&msg);
			}else
			{
				//2555	1	a,$s1 will disappear in less than a minute.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				CSystemMessage msg(2555);
				msg.AddItem(data.first);
				SendSystemMessage(&msg);
			}
		}
	}
	unguard;
}

void User::ValidateLifeTimeItems()
{
	guard;
	CSharedItemData* lpSharedItemDataArray = (*(CSharedItemData**)0x0E412138);
	INT32 itemCount = 0;
	vector<CItem*> outdatedItems;
	vector<INT32> message5Minutes;
	vector<INT32> message1Minute;
	time_t currentTime = time(0);
	inventory.ReadLock(__FILEW__, __LINE__);
	INT32 index = inventory.GetFirstItemIndex();
	try
	{
		while(index > -1 && index < 900000 && itemCount < 200)
		{
			CSharedItemData *lpSID = &lpSharedItemDataArray[index];
			if(lpSID)
			{
				if(CItem *pItem = inventory.GetItemByIndex(index))
				{
					if(pItem->nLifeTime)
					{
						INT32 timeDiff = pItem->nLifeTime - currentTime;
						if(timeDiff >= 290 && timeDiff < 310)
						{
							message5Minutes.push_back(pItem->pSID->nItemID);
						}else if(timeDiff >= 50 && timeDiff < 70)
						{
							message1Minute.push_back(pItem->pSID->nItemID);
						}
						if(timeDiff <= 0)
						{
							outdatedItems.push_back(pItem);
						}
					}
				}
			}
			index = lpSID->nNextIndex;
			itemCount++;
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}

	inventory.ReadUnlock();
	//2575	1	a,$s1 will disappear in $s2 minute(s).\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0


	for(UINT n=0;n<message5Minutes.size();n++)
	{
		CSystemMessage msg(2575);
		msg.AddItem(message5Minutes[n]);
		msg.AddNumber(5);
		SendSystemMessage(&msg);
	}

	for(UINT n=0;n<message1Minute.size();n++)
	{
		CSystemMessage msg(2575);
		msg.AddItem(message1Minute[n]);
		msg.AddNumber(1);
		SendSystemMessage(&msg);
	}

	//2576	1	a,$s1 life time expired.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
	for(UINT n=0;n<outdatedItems.size();n++)
	{
		CItem *pItem = outdatedItems[n];
		CSystemMessage msg(2576);
		msg.AddItem(pItem->pSID->nItemID);
		SendSystemMessage(&msg);

		g_DB.RequestDestroyItem(pItem, pItem->pSID->nItemAmount, this);
	}
	unguard;
}

void User::ValidateItemExSkill()
{
	guard;
	if(ValidUser())
	{
		vector<INT32> stacked;
		map<INT32, INT32> skillsToAcquire;
		for(map<INT32, ItemExSkillList>::iterator it = g_ItemExSkillList.begin(); it != g_ItemExSkillList.end(); it++)
		{
			INT32 itemCount = 0;
			CItem *pItem = inventory.GetFirstItemByClassID(it->first);
			while(pItem->IsValidItem())
			{
				itemCount += pItem->pSID->nItemAmount;
				pItem = inventory.GetNextItemByClassID(pItem->pSID->nItemIndex);
			}
			if(itemCount > 0)
			{
				ItemExSkillList& skillList = it->second;
				int stackType = skillList.m_stackType;

				bool canAcquire = true;
				//check if it's already in stack
				if(stackType > 0)
				{
					for(UINT n=0;n<stacked.size();n++)
					{
						if(stacked[n] == stackType)
						{
							canAcquire = false;
							break;
						}
					}
					if(canAcquire)
					{
						stacked.push_back(stackType);
					}
				}

				if(canAcquire)
				{
					for(UINT n=0;n<skillList.m_Skills.size();n++)
					{
						int skillId = skillList.m_Skills[n].skill_id;
						int skillLevel = skillList.m_Skills[n].level;
						if(skillList.m_maxStackableLevel > 1)
						{
							skillLevel = skillLevel + itemCount - 1;
							if(skillLevel > skillList.m_maxStackableLevel)
							{
								skillLevel = skillList.m_maxStackableLevel;
							}
						}
						skillsToAcquire.insert(pair<INT32, INT32>(skillId, skillLevel));
					}
				}
			}
		}

		bool validateSkills = false;
		vector<INT32> toDelete;
		pSD->pExData->Lock();	
		if(skillsToAcquire != pED->itemSkills)
		{
			validateSkills = true;
			for(map<INT32, INT32>::iterator it = pED->itemSkills.begin(); it!= pED->itemSkills.end();it++)
			{
				if(skillsToAcquire.find(it->first) == skillsToAcquire.end())
				{
					toDelete.push_back(it->first);
				}
			}
			pED->itemSkills = skillsToAcquire;
		}
		pSD->pExData->Unlock();

		if(validateSkills)
		{
			//delete skills
			for(UINT n=0;n<toDelete.size();n++)
			{
				DeleteSkill(toDelete[n]);
			}
			//acquire new skills
			for(map<INT32, INT32>::iterator it=skillsToAcquire.begin();it!=skillsToAcquire.end();it++)
			{
				AcquireSkill(it->first, it->second, 1);
			}

			ValidateSkillList();
			ValidateParameters();
		}
	}
	unguard;
}

bool User::HaveItemInPrivateStore(int ItemID)
{
	if(ValidUser())
	{
		PrivateStoreItemMap* pMap = NULL;
		if(pSD->nStoreMode == StoreSell)
		{
			pMap = GetPrivateStoreSell();
		}else if(pSD->nStoreMode == StoreBuy)
		{
			pMap = GetPrivateStoreBuy();
		}
		if(pMap)
		{
			for(PrivateStoreItemMap::iterator Iter = pMap->begin();Iter!=pMap->end();Iter++)
			{
				if(Iter->second->ClassID == ItemID)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool User::Revive()
{
	if(ValidUser())
	{
		if(pSD->nAlive == 0)
		{
			typedef bool (*f)(CCreature*, CCreature*, int);
			f(0x518070L)(this, 0, pSD->nRessurectExp);
			pSD->nRessurectExp = 0;
			pSD->nRessurectTickCount = GetTickCount();
		}
	}
	return false;
}

void User::ValidateTerritory()
{
	guard;
	if(ValidUser())
	{
		TerritoryData& td = pSD->pExData->territoryData;
		if(td.territoryId[FORBIDDEN_RECALL_ZONE] != 0)
		{
			if(HasAbnormal(179))
			{
				DispelByAbnormal(179, true);
				pSocket->SendSystemMessage(L"You cannot use clan gate in this zone!");
			}
		}

		if(td.territoryId[NOBLESS_ZONE] != 0)
		{
			if(!pSD->nNobless)
			{
				//2574	1	a,Only noble characters are allowed in this zone.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				SendSystemMessage(2574);
				if(CNoblessTerritory *pT = g_TerritoryChecker.GetTerritory(td.territoryId[NOBLESS_ZONE])->SafeCastNobless())
				{
					IVector kickOut = pT->GetKickOutPos();
					PlayerAction::OnTeleportToLocation(this, kickOut.x, kickOut.y, kickOut.z, 0);
				}
			}
		}

		//No Part Zone
		if(td.territoryId[NO_PARTY_ZONE] != 0)
		{
			if(CParty *pParty = GetParty())
			{
				pParty->LeaveParty(this);
				SendSystemMessage(L"Party is not allowed in this zone!");
			}
		}

		//Mining zone
		if(pED->miningAreaId)
		{
			if(td.territoryId[MINING_ZONE] == 0)
			{
				pED->miningAreaId = 0;
				//2566	1	a,You have left a Mining Zone.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pSocket->SendSystemMessage(2566);
			}
		}else
		{
			if(td.territoryId[MINING_ZONE] != 0)
			{
				pED->miningAreaId = td.territoryId[MINING_ZONE];
				//2565	1	a,You have entered a Mining Zone.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pSocket->SendSystemMessage(2565);
			}
		}

		//NickNameChange Zone
		if(pSD->nInNickNameChangeZone)
		{
			if(td.territoryId[NICKNAME_CHANGE_ZONE] == 0)
			{
				pSD->nInNickNameChangeZone = NULL;
				SetUserInfoChanged();
				SetCharInfoChanged();
			}
		}else
		{
			if(td.territoryId[NICKNAME_CHANGE_ZONE])
			{
				pSD->nInNickNameChangeZone = td.territoryId[NICKNAME_CHANGE_ZONE];
				SetUserInfoChanged();
				SetCharInfoChanged();
			}
		}
		//Block Scheme Buffer zone
		if(pED->schemeBufferUser.inBlockZone)
		{
			if(td.territoryId[BLOCK_SCHEME_BUFFER_ZONE] == 0)
			{
				pED->schemeBufferUser.inBlockZone = false;
			}
		}else
		{
			if(td.territoryId[BLOCK_SCHEME_BUFFER_ZONE] != 0)
			{
				pED->schemeBufferUser.inBlockZone = true;
			}
		}

		//respawn zone
		if(pED->preserveAbnormalTerritory)
		{
			if(td.territoryId[RESPAWN_ZONE] == 0 && td.territoryId[PRESERVE_ABNORMAL_ZONE] == 0)
			{
				pED->preserveAbnormalTerritory = false;
				SetValidateParametersRequest();
			}
		}else
		{
			if(td.territoryId[RESPAWN_ZONE] != 0)
			{
				if(CRespawnTerritory *pT = g_TerritoryChecker.GetTerritory(td.territoryId[RESPAWN_ZONE])->SafeCastRespawn())
				{
					if(pT->ProtectBuffs())
					{
						pED->preserveAbnormalTerritory = true;
						SetValidateParametersRequest();
					}
				}
			}
			if(td.territoryId[PRESERVE_ABNORMAL_ZONE] != 0)
			{
				pED->preserveAbnormalTerritory = true;
				SetValidateParametersRequest();
			}
		}

		//BlockSkills zone
		if(pSD->nInBlockSkillsZone)
		{
			if(td.territoryId[BLOCK_SKILLS_ZONE] == 0)
			{
				pSD->nInBlockSkillsZone = 0;
			}
		}else
		{
			if(td.territoryId[BLOCK_SKILLS_ZONE])
			{
				pSD->nInBlockSkillsZone = td.territoryId[BLOCK_SKILLS_ZONE];
			}
		}

		//BlockItems zone
		if(pSD->nInBlockItemsZone)
		{
			if(td.territoryId[BLOCK_ITEMS_ZONE] == 0)
			{
				pSD->nInBlockItemsZone = 0;
			}
		}else
		{
			if(td.territoryId[BLOCK_ITEMS_ZONE])
			{
				pSD->nInBlockItemsZone = td.territoryId[BLOCK_ITEMS_ZONE];
				//check items
				CBlockItemsTerritory *pTerritory = g_TerritoryChecker.GetTerritory(td.territoryId[BLOCK_ITEMS_ZONE])->SafeCastBlockItems();
				if(pTerritory)
				{
					for(int n=0;n<18;n++)
					{
						int nItemIndex = pSD->nEquipment[n];
						if(nItemIndex > 0)
						{
							CSharedItemData *pSID = CSharedItemData::FromIndex(nItemIndex);
							if(pSID)
							{
								if(pTerritory->IsBlocked(pSID->nItemID))
								{
									UnEquipItem(n);
								}
							}
						}
					}
				}
			}
		}
		
		// Party zone
		if(td.territoryId[PARTY_ZONE] != 0)
		{
			if(pSD->nActiveUser)
			{
				if(CPartyTerritory *pTerritory = g_TerritoryChecker.GetTerritory(td.territoryId[PARTY_ZONE])->SafeCastParty())
				{
					UINT members = 0;
					if(CParty *pParty = GetParty())
					{
						members = pParty->memberCount;
					}
					if(!pTerritory->CanEnter(members))
					{
						IVector ipos(pTerritory->GetKickOutPos());
						SendSystemMessage(L"You're not allowed to be here!");
						PlayerAction::OnTeleportToLocation(this, ipos.x, ipos.y, ipos.z, false);
					}
				}
			}
		}

		// Level restriction zone
		if(td.territoryId[LEVEL_RESTRICTION_ZONE] != 0)
		{
			if(pSD->nActiveUser)
			{
				if(CLevelRestrictionTerritory *pTerritory = g_TerritoryChecker.GetTerritory(td.territoryId[LEVEL_RESTRICTION_ZONE])->SafeCastLevelRestriction())
				{
					if(!pTerritory->CanEnter(pSD->nLevel))
					{
						IVector ipos(pTerritory->GetKickOutPos());
						SendSystemMessage(L"You're not allowed to be here!");
						PlayerAction::OnTeleportToLocation(this, ipos.x, ipos.y, ipos.z, false);
					}
				}
			}
		}

		// Required Item zone
		if(td.territoryId[REQUIRED_ITEM_ZONE] != 0)
		{
			if(CRequiredItemTerritory* pTerritory = g_TerritoryChecker.GetTerritory(td.territoryId[REQUIRED_ITEM_ZONE])->SafeCastRequiredItem())
			{
				if(!pTerritory->HasItem(this))
				{
					IVector kickPos = pTerritory->GetKickOutPos();
					SendSystemMessage(L"You're not allowed to be in this territory!");
					PlayerAction::OnTeleportToLocation(this, kickPos.x, kickPos.y, kickPos.z, 0);
				}
			}
		}

		//instance zone
		UINT oldInstanceId = pED->currentInstanceId;
		pED->currentInstanceId = 0;
		if(td.territoryId[INSTANCE_ZONE] != 0)
		{
			VIRTUALIZER_TIGER_RED_START;
			static bool canUseInstance = false;
			if(canUseInstance || CLicense::GetInstance().CanUse(LicenseInstanceZone))
			{
				canUseInstance = true;
				if(CInstanceTerritory *pTerritory = g_TerritoryChecker.GetTerritory(td.territoryId[INSTANCE_ZONE])->SafeCastInstance())
				{
					pED->currentInstanceId = pTerritory->GetInstanceId();
					bool kick = false;
					SaveInstanceTime(pTerritory->GetInstanceId(), oldInstanceId, pTerritory->GetAllowedTime(), pTerritory->GetReuseTime(), kick, pTerritory->IsByHWID() ? true : false);
					if(kick)
					{
						IVector kickPos = pTerritory->GetKickOutPos();
						//2537	1	a,You have no time left in this instance zone.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pSocket->SendSystemMessage(2537);
						PlayerAction::OnTeleportToLocation(this, kickPos.x, kickPos.y, kickPos.z, 0);
					}
				}
			}
			VIRTUALIZER_TIGER_RED_END;
		}


		//Race Change Zone
		if(pED->raceChangeZoneId)
		{
			if(td.territoryId[RACE_CHANGE_ZONE] == 0)
			{
				pED->raceChangeZoneId = 0;
				pED->fakeRace = UINT_MAX;
				pED->fakeSex = UINT_MAX;
				PlayerAction::OnTeleportToLocation(this, (int)pSD->Pos.x, (int)pSD->Pos.y, (int)pSD->Pos.z, false);
			}
		}else
		{
			if(td.territoryId[RACE_CHANGE_ZONE])
			{
				pED->raceChangeZoneId = td.territoryId[RACE_CHANGE_ZONE];
				if(CRaceChangeTerritory *pTerritory = g_TerritoryChecker.GetTerritory(pED->raceChangeZoneId)->SafeCastRaceChange())
				{
					pED->fakeRace = pTerritory->GetRace();
					pED->fakeSex = pTerritory->GetSex();
				}
				PlayerAction::OnTeleportToLocation(this, (int)pSD->Pos.x, (int)pSD->Pos.y, (int)pSD->Pos.z, false);
			}
		}
		
		//block chat zone
		if(pED->blockChatZoneId)
		{
			if(td.territoryId[BLOCK_CHAT_ZONE] == 0)
			{
				pED->blockChatZoneId = 0;
				SendETCBuffStatus();
			}
		}else
		{
			if(td.territoryId[BLOCK_CHAT_ZONE])
			{
				pED->blockChatZoneId = td.territoryId[BLOCK_CHAT_ZONE];
				SendETCBuffStatus();
			}
		}
	}
	unguard;
}

void User::RequestUpdateSpelledInfo()
{
	if(this)
	{
		typedef void(*f)(User*);
		f(0x84C6B0L)(this);
	}
}

bool User::ValidateStackableItems()
{
	guard;
	CSharedItemData* lpSharedItemDataArray = (*(CSharedItemData**)0x0E412138);
	inventory.ReadLock(__FILEW__, __LINE__);
	INT32 index = inventory.GetFirstItemIndex();
	INT32 itemCount = 0;
	map<INT32, list<UINT>> mStack;
	try
	{
		while(index > -1 && index < 900000 && itemCount < 200)
		{
			CSharedItemData *lpSID = &lpSharedItemDataArray[index];
			if(lpSID)
			{
				if(lpSID->nConsumeType == ConsumeAsset || lpSID->nConsumeType == ConsumeStackable)
				{
					map<INT32, list<UINT>>::iterator it = mStack.find(lpSID->nItemID);
					if(it!=mStack.end())
					{
						it->second.push_back(lpSID->nDBID);
						g_Log.Add(CLog::Error, "[%s] Multiple stack [%d][%d] User[%S]", __FUNCTION__, lpSID->nItemID, lpSID->nDBID, pSD->wszName);
						if(!g_DeleteInvalidStack)
						{
							pSocket->SendSystemMessage(L"System detected invalid items in your inventory! Please contact with administration!");
							inventory.ReadUnlock();
							unguard;
							return false;
						}
					}else
					{
						list<UINT> lDbId;
						lDbId.push_back(lpSID->nDBID);
						mStack.insert(pair<INT32, list<UINT>>(lpSID->nItemID, lDbId));
					}
				}
			}
			index = lpSID->nNextIndex;
			itemCount++;
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}

	inventory.ReadUnlock();

	if(g_DeleteInvalidStack)
	{
		bool tradeCancelled = false;
		for(map<INT32, list<UINT>>::iterator it = mStack.begin();it!=mStack.end();it++)
		{
			list<UINT>& lDbId = it->second;
			if(lDbId.size() > 1)
			{
				lDbId.sort();
				bool toDelete = false;
				for(list<UINT>::iterator itItem = lDbId.begin();itItem != lDbId.end();itItem++)
				{
					if(!toDelete)
					{
						toDelete = true;
					}else
					{
						if(!tradeCancelled)
						{
							tradeCancelled = true;
							if(IsNowTrade())
							{
								TradeCancel();
							}
						}
						CItem *pItem = inventory.GetItemByDBID((*itItem));
						if(pItem)
						{
							g_Logger.Add(L"Deleting duplicated stack [%d][%d][%d] for user[%s]", pItem->pSID->nDBID, pItem->pSID->nItemID, pItem->pSID->nItemAmount, pSD->wszName);
							g_DB.RequestDestroyItem(pItem, pItem->pSID->nItemAmount, this);
						}
					}
				}
			}
		}
	}
	unguard;
	return true;
}

bool User::GetDropBonuses(double& itemDrop, double& adena, double& raidBoss, double& raidBossOnlyChance, double& epicBoss, double &itemOnlyChance)
{
	guard;
	itemDrop = 0.0;
	adena = 0.0;
	raidBoss = 0.0;
	raidBossOnlyChance = 0.0;
	epicBoss = 0.0;
	itemOnlyChance = 0.0;
	if(CParty *pParty = GetParty())
	{
		INT32 memberCount = 0;
		UserVector vMembers;
		pParty->GetAllMembersEx(vMembers);
		for(UserVector::iterator it = vMembers.begin(); it!= vMembers.end(); it++)
		{
			User *pMember = (*it);
			if(pMember->ValidUser())
			{
				if(g_MaxPartyLevelRange)
				{
					if((pSD->nLevel + g_MaxPartyLevelRange) < pMember->pSD->nLevel)
					{
						continue;
					}
					if((pSD->nLevel - g_MaxPartyLevelRange) > pMember->pSD->nLevel)
					{
						continue;
					}
				}

				if(IsInRange(pMember, 3000))
				{
					memberCount++;
					itemDrop += pMember->pSD->pExData->SkillMod.bonusItemDrop;
					adena += pMember->pSD->pExData->SkillMod.bonusAdena;
					raidBoss += pMember->pSD->pExData->SkillMod.bonusRb;
					epicBoss += pMember->pSD->pExData->SkillMod.bonusEpicRb;
					itemOnlyChance += pMember->pSD->pExData->SkillMod.bonusItemDropOnlyChance;
					raidBossOnlyChance += pMember->pSD->pExData->SkillMod.bonusRbOnlyChance;
				}
			}
		}
		if(memberCount > 0)
		{
			itemDrop /= static_cast<double>(memberCount);
			adena /= static_cast<double>(memberCount);
			raidBoss /= static_cast<double>(memberCount);
			epicBoss /= static_cast<double>(memberCount);
			itemOnlyChance /= static_cast<double>(memberCount);
			raidBossOnlyChance /= static_cast<double>(memberCount);
		}
	}else
	{
		itemDrop = pSD->pExData->SkillMod.bonusItemDrop;
		adena = pSD->pExData->SkillMod.bonusAdena;
		itemOnlyChance = pSD->pExData->SkillMod.bonusItemDropOnlyChance;
		raidBoss = pSD->pExData->SkillMod.bonusRb;
		epicBoss = pSD->pExData->SkillMod.bonusEpicRb;
		raidBossOnlyChance = pSD->pExData->SkillMod.bonusRbOnlyChance;
	}
	unguard;
	return true;
}

double User::GetAdenaBonus()
{
	double bonus = 0.0;
	if(CParty *pParty = GetParty())
	{
		INT32 memberCount = 0;
		UserVector vMembers;
		pParty->GetAllMembersEx(vMembers);
		for(UserVector::iterator it = vMembers.begin(); it!= vMembers.end(); it++)
		{
			User *pMember = (*it);
			if(pMember->ValidUser())
			{
				if(g_MaxPartyLevelRange)
				{
					if((pSD->nLevel + g_MaxPartyLevelRange) < pMember->pSD->nLevel)
					{
						continue;
					}
					if((pSD->nLevel - g_MaxPartyLevelRange) > pMember->pSD->nLevel)
					{
						continue;
					}
				}
				if(IsInRange(pMember, 3000))
				{
					memberCount++;
					bonus += pMember->pSD->pExData->SkillMod.bonusAdena;	
				}
			}
		}
		if(memberCount > 0)
		{
			bonus /= static_cast<double>(memberCount);
		}
	}else
	{
		bonus = pSD->pExData->SkillMod.bonusAdena;
	}
	return bonus;
}

void User::ShowBlockableItems(User *pGM)
{
	guard;
	CSharedItemData* lpSharedItemDataArray = (*(CSharedItemData**)0x0E412138);
	INT32 itemCount = 0;
	vector<BlockItemInfo> items;
	inventory.ReadLock(__FILEW__, __LINE__);
	INT32 index = inventory.GetFirstItemIndex();
	try
	{
		while(index > -1 && index < 900000 && itemCount < 200)
		{
			CSharedItemData *lpSID = &lpSharedItemDataArray[index];
			if(lpSID)
			{
				if(lpSID->nConsumeType == ConsumeNormal && lpSID->nItemType < 3 )
				{
					BlockItemInfo info;
					info.databaseId = lpSID->nDBID;
					info.enchant = lpSID->nEnchantLevel;
					info.itemId = lpSID->nItemID;
					items.push_back(info);
				}
			}
			index = lpSID->nNextIndex;
			itemCount++;
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}

	inventory.ReadUnlock();

	//generate htmls
	wstringstream htmlStream;
	htmlStream << L"<html><body>";
	htmlStream << L"NickName: " << pSD->wszName << L" Items: " << items.size() << L"<br>";
	for(INT32 n=0;n<items.size();n++)
	{	
		htmlStream << L"<a action=\"bypass -h block_item?id=" << items[n].databaseId << L"&userId=" << nDBID << L"\">" << (n+1)<< L". " << g_ItemDBEx.GetItemName(items[n].itemId).c_str() << L" Enchant: " << items[n].enchant << L"</a><br>";
	}
	htmlStream << L"</body></html>";

	if(pGM->ValidUser())
	{
		PlayerAction::ShowHTML(pGM, L"block_item.htm", htmlStream.str().c_str(), 0);
	}else
	{
		PlayerAction::ShowHTML(this, L"block_item.htm", htmlStream.str().c_str(), 0);
	}
	unguard;
}

LPBYTE User::GetHWID()
{
	if(ValidUser())
	{
		return pSocket->pED->longHWID.hash;
	}
	return 0;
}

bool User::QueueBuffList(bool summon, bool remote)
{
	guard;
	SchemeBufferUser& sbu = pED->schemeBufferUser;
	UINT delay = g_SchemeBuffer.GetBuffDelay();
	sbu.lock.Enter();
	if(sbu.buffQueue.size() < 96)
	{
		vector<SkillPchId>& buffList = sbu.GetList(summon, remote);
		for(UINT n=0;n<buffList.size();n++)
		{
			SchemeBuff& sb = g_SchemeBuffer.Get(buffList[n].pchId, remote);
			if(sb.pSI)
			{
				//check if it's not already in queue
				bool alreadyIn = false;
				for(UINT m=0;m<sbu.buffQueue.size();m++)
				{
					if(sbu.buffQueue[m].pSI == sb.pSI && sbu.buffQueue[m].summon == summon)
					{
						alreadyIn = true;
						break;
					}
				}

				if(alreadyIn)
				{
					continue;
				}

				QueuedBuff qb;
				qb.pSI = sb.pSI;
				qb.abnormalTime = sb.abnormalTime;
				qb.summon = summon;
				if(delay > 0)
				{
					if(delay < 100)
					{
						delay = 100;
					}
					sbu.buffQueue.push_back(qb);
					if(!sbu.timerInUse)
					{
						sbu.timerInUse = TRUE;
						typedef void(*f)(CCreature*, UINT, UINT);
						f(0x6261D0L)(this, delay, 1986);
					}
				}else
				{
					ActivateQueuedBuff(qb);
				}
			}
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] User[%S] reached buff queue limit!", __FUNCTION__, pSD->wszName);
	}
	sbu.lock.Leave();
	unguard;
	return false;
}

bool User::QueueBuff(CSkillInfo *pSI, bool summon, int abnormalTime)
{
	guard;
	QueuedBuff qb;
	qb.pSI = pSI;
	qb.abnormalTime = abnormalTime;
	qb.summon = summon;

	UINT delay = g_SchemeBuffer.GetBuffDelay();
	if(delay > 0)
	{
		if(delay < 100)
		{
			delay = 100;
		}
		SchemeBufferUser& sbu = pED->schemeBufferUser;
		sbu.lock.Enter();
		if(sbu.buffQueue.size() < 40)
		{
			//check if it's not already in queue
			bool alreadyIn = false;
			for(UINT m=0;m<sbu.buffQueue.size();m++)
			{
				if(sbu.buffQueue[m].pSI == pSI && sbu.buffQueue[m].summon == summon)
				{
					alreadyIn = true;
					break;
				}
			}

			if(!alreadyIn)
			{
				sbu.buffQueue.push_back(qb);
				if(!sbu.timerInUse)
				{
					sbu.timerInUse = TRUE;
					typedef void(*f)(CCreature*, UINT, UINT);
					f(0x6261D0L)(this, delay, 1986);
				}
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] reached buff queue limit!", __FUNCTION__, pSD->wszName);
		}
		sbu.lock.Leave();

	}else
	{
		ActivateQueuedBuff(qb);
	}
	
	unguard;
	return false;
};

void User::ActivateQueuedBuff(QueuedBuff& qb)
{
	guard;

	CCreature *pTarget = this;
	if(qb.summon)
	{
		pTarget = GetSummonOrPet();
	}
	if(pTarget->ValidCreature() && qb.pSI)
	{
		if(pTarget->pSD->nIsPC)
		{
			if(User *pUser = pTarget->SafeCastUser())
			{
				if(pUser->olympiadUser.IsInside() && g_SchemeBuffer.IsAllowInOlympiad() == FALSE)
				{
					unguard;
					return;
				}
			}
		}
		qb.pSI->ActivateSkill(this, pTarget);
		//change abnormal time if required
		if(qb.abnormalTime > 0)
		{
			UINT customAbnormalTime = time(0) + qb.abnormalTime - qb.pSI->nAbnormalTime;

			pTarget->abnormalStatusLock.Lock(__FILEW__, __LINE__);
			for(AbnormalDataVector::iterator Iter = pTarget->vAbnormalStatus.begin(); Iter != pTarget->vAbnormalStatus.end(); Iter++)
			{
				if( Iter->pSI )
				{
					if( Iter->pSI->nSkillID == qb.pSI->nSkillID && Iter->pSI->nLevel == qb.pSI->nLevel )
					{
						Iter->expireTime = customAbnormalTime;
						break;
					}
				}
			}
			pTarget->abnormalStatusLock.Unlock();
			if( pTarget == this )
			{
				SetAbnormalStatusChanged();
				SetSpelledInfoChanged();
			}
		}
	}
	unguard;
}

void User::DispatchQueuedBuff()
{
	guard;
	SchemeBufferUser& sbu = pED->schemeBufferUser;
	sbu.lock.Enter();
	vector<QueuedBuff>::iterator it = sbu.buffQueue.begin();
	if(it!=sbu.buffQueue.end())
	{
		QueuedBuff qb;
		qb.abnormalTime = it->abnormalTime;
		qb.pSI = it->pSI;
		qb.summon = it->summon;
		sbu.buffQueue.erase(it);
		if(sbu.buffQueue.size() > 0)
		{
			UINT delay = g_SchemeBuffer.GetBuffDelay();
			if(delay < 100)
			{
				delay = 100;
			}
			typedef void(*f)(CCreature*, UINT, UINT);
			f(0x6261D0L)(this, delay, 1986);
		}else
		{
			sbu.timerInUse = FALSE;
		}
		sbu.lock.Leave();

		ActivateQueuedBuff(qb);
	}else
	{
		sbu.timerInUse = FALSE;
		sbu.lock.Leave();
	}
	unguard;
}

void User::ClearQueuedBuffs()
{
	guard;
	SchemeBufferUser& sbu = pED->schemeBufferUser;
	sbu.lock.Enter();
	if(sbu.buffQueue.size() > 0)
	{
		sbu.buffQueue.clear();
	}
	sbu.lock.Leave();
	unguard;
}

void User::HideBoard()
{
	pSocket->Send("ccSSSSSSSSS", 0x6E, 0x00, L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"<HTML><HEAD></HEAD><BODY></BODY></HTML>");
}

void User::ShowBoard(const WCHAR *wHtml)
{
	if(g_HtmlFilter.IsEnabled())
	{
		wstringstream dataStream;
		dataStream << L"bypass _auction_none" << endl << wHtml << endl;
		g_HtmlFilter.OnSendHtml(this, L"community_board.htm", dataStream.str().c_str(), 0);
	}
	pSocket->Send("ccSSSSSSSSS", 0x6E, 0x01, L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", L"bypass _auction_none", wHtml);
}

double User::GetSkillDmgMultipler(int skillId, int level)
{
	guard;
	double multipler = 1.0;
	pED->lock.Enter();
	for(UINT n=0;n<pED->skillDmgMultiplers.size();n++)
	{
		SkillDmgMultipler& sdm = pED->skillDmgMultiplers[n];
		if(sdm.skillId == 0 || sdm.skillId == skillId)
		{
			if(sdm.level == 0 || sdm.level == level)
			{
				multipler *= sdm.multipler;
			}
		}
	}
	pED->lock.Leave();

	unguard;
	return multipler;
}

bool User::TiggerAutoCastSkill(AutoCastSkill::Type type, CCreature *pOther)
{
	guard;

	vector<AutoCastSkill> autoSkills;
	pED->lock.Enter();
	for(UINT n=0;n<pED->autoCastSkills.size();n++)
	{
		pED->autoCastSkills[n];
		if(pED->autoCastSkills[n].type == type)
		{
			autoSkills.push_back(pED->autoCastSkills[n]);
		}
	}
	pED->lock.Leave();

	for(UINT n=0;n<autoSkills.size();n++)
	{
		AutoCastSkill& acs = autoSkills[n];
		if(acs.skillId > 0)
		{
			if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(acs.skillId, acs.level))
			{
				if(User *pTarget = pOther->SafeCastUser())
				{
					if(pSI->nEffectPoint < 0 && pTarget->pSD->nIsPC)
					{
						if(pTarget->inPvpZone == false && pTarget->pSD->bGuilty == false && pTarget->pSD->nKarma == 0)
						{
							continue;
						}
					}
				}

				if(acs.chance > g_Random.RandDouble(100.0))
				{
					//check reuse
					if(pSI->dReuseDelay > 0.0)
					{
						LPDWORD lpTimestamp = CreatureAction::OnGetSkillUsedTime(this, acs.skillId, acs.level);
						if(lpTimestamp)
						{
							if((*lpTimestamp) > GetTickCount())
							{
								//48	1	a,$s1 is not available at this time: being prepared for reuse.\0	0	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,none\0
								CSystemMessage msg(48);
								msg.AddSkill(acs.skillId, acs.level);
								SendSystemMessage(&msg);
								continue;
							}else
							{
								CreatureAction::OnSetSkillUsableTime(this, acs.skillId, acs.level, lpTimestamp, static_cast<DWORD>(pSI->dReuseDelay));
							}
						}else
						{
							CreatureAction::OnSetSkillUsableTime(this, acs.skillId, acs.level, lpTimestamp, static_cast<DWORD>(pSI->dReuseDelay));
						}
					}
					int nTargetID = 0;
					if(pSI->targetType == CSkillInfo::TargetSelf)
					{
						nTargetID = nObjectID;
						pSI->ActivateSkill(this, this, 0, 0);
					}else
					{
						if(pOther->ValidCreature())
						{
							nTargetID = pOther->nObjectID;
							pSI->ActivateSkill(this, pOther, 0, 0);
							if(acs.skillId == 5144)
							{
								CSkillFx::BroadcastAutoCastSkill(nObjectID, nTargetID, acs.skillId, acs.level, &pSD->Pos);
							}
						}
					}
				}
			}
		}
	}

	unguard;
	return false;
}

void User::StopSummon()
{
	if(CSummon *pSummon = GetSummonOrPet())
	{
		typedef void(*f)(User*, int, bool, bool);
		f(0x837850L)(this, 23, true, false);
	}
}

bool User::IsFriend(User *pUser)
{
	guard;
	if(pFriends)
	{
		CHAR buff[8190] = { 0 };
		int len = 0;
		int friendAmount = pFriends->GetFriendList(8190, buff, len);
		const unsigned char* pck = (const unsigned char*)buff;
		for(int n = 0; n < friendAmount; n++)
		{
			UINT userId = 0;
			WCHAR wName[25] = { 0 };
			UINT online, unkn;
			//dSdd loop
			pck = Disassemble(pck, "dSdd", &userId, sizeof(wName), wName, &online, &unkn);
			if(userId == pUser->nDBID)
			{
				unguard;
				return true;
			}
		}
	}
	unguard;
	return false;
}

void User::ShowPCCafePoints(INT32 totalPoints, INT32 newPoints, INT32 timeInHours, bool adding)
{
	guard;

	UINT pointType = 0;
	UINT periodType = 0;

	if(adding)
	{
		periodType = 1;
		pointType = 1;
	}else
	{
		periodType = 2;
		pointType = 2;
	}

	pSocket->Send("chddcdc", 0xFE, 0x31, totalPoints, newPoints, periodType, timeInHours, pointType);

	unguard;
}

void User::ValidateSubjobParameters()
{
	typedef void(*f)(User*);
	f(0x851C70L)(this);
}

void User::HideAbnormal(AbnormalData& abnormal, UINT expireTime)
{
	guard;

	HiddenAbnormalData had(abnormal);
	had.hideExpireTimestamp = time(0) + expireTime;
	

	pED->lock.Enter();

	if(pED->hiddenAbnormals.size() < 40)
	{
		pED->hiddenAbnormals.push_back(had);
	}

	pED->lock.Leave();

	unguard;
}

void User::ClearHiddenAbnormals()
{
	guard;

	pED->lock.Enter();
	pED->hiddenAbnormals.clear();
	pED->lock.Leave();

	unguard;
}

void User::TimerExpiredForHiddenAbnormals(bool allExpired)
{
	guard;

	time_t currentTime = time(0);
	vector<HiddenAbnormalData> toRestore;
	pED->lock.Enter();

	for(UINT n=0;n<pED->hiddenAbnormals.size();n++)
	{
		if(pED->hiddenAbnormals[n].hideExpireTimestamp <= currentTime || allExpired)
		{
			toRestore.push_back(pED->hiddenAbnormals[n]);
		}
	}

	//remove restored from container
	if(allExpired)
	{
		pED->hiddenAbnormals.clear();
	}else
	{
		for(UINT n=0;n<toRestore.size();n++)
		{
			for(vector<HiddenAbnormalData>::iterator it = pED->hiddenAbnormals.begin(); it != pED->hiddenAbnormals.end(); it++)
			{
				if(toRestore[n].pSI == it->pSI && toRestore[n].expireTime == it->expireTime)
				{
					pED->hiddenAbnormals.erase(it);
					break;
				}
			}
		}
	}
	pED->lock.Leave();

	//restore abnormals
	bool restored = false;
	for(UINT n=0;n<toRestore.size();n++)
	{
		HiddenAbnormalData& had = toRestore[n];
		if(had.pSI)
		{
			if(had.pSI->operateType == OperateA2)
			{
				int currentLevel = HasAbnormal(had.pSI->nAbnormalType);
				if(currentLevel < had.pSI->nAbnormalLevel)
				{
					had.pSI->ActivateSkill(this, this);
					had.restored = true;
					restored = true;
				}
			}
		}
	}

	if(restored)
	{
		abnormalStatusLock.Lock(__FILEW__, __LINE__);
		for(UINT m = 0; m < toRestore.size(); m++)
		{
			HiddenAbnormalData& had = toRestore[m];
			if(had.restored)
			{
				for(UINT n=0;n<vAbnormalStatus.size();n++)
				{
					AbnormalData& ad = vAbnormalStatus[n];
					if(ad.pSI == had.pSI)
					{
						ad.expireTime = had.expireTime;
						ad.partyBuff = had.partyBuff;
						ad.casterSID = had.casterSID;
						ad.selfBuff = had.selfBuff;
					}
				}
			}
		}
		abnormalStatusLock.Unlock();
		lpSharedDataLock->Lock(__FILEW__, __LINE__);
		ValidateAllOnSIM();
		lpSharedDataLock->Unlock();
		broadcastInfoPacket = true;

		CheckAbnormalVisualEffect();
		SetAbnormalStatusChanged();
		RequestUpdateSpelledInfo();
		SetUserInfoChanged();
		SetCharInfoChanged();
	}


	unguard;
}

void User::SaveInstances()
{
	guard;

	for(UINT n=0;n<pED->instances.size();n++)
	{
		InstanceData& id = pED->instances[n];
		if(!id.saved)
		{
			g_DB.RequestSaveInstanceTime(nDBID, id.instanceId, id.timeSpent / 1000, id.reuseTimeout);
		}
	}


	unguard;
}

void User::ValidateEquippedItems()
{
	guard;
	
	for(int n=0;n<18;n++)
	{
		int itemIndex = pSD->nEquipment[n];
		if(itemIndex > 0)
		{
			CItemSP itemSp;
			CItem::FindItemSp(itemSp, itemIndex);

			if(CItem *pItem = itemSp.get())
			{
				if(!CanEquip(pItem))
				{
					UnEquipItem(n);
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find item with index[%d] - User[%S]!", __FUNCTION__, itemIndex, pSD->wszName);
			}
		}
	}

	unguard;
}

void User::SaveInstanceTime(UINT instanceId, UINT previousInstanceId, UINT allowedTime, UINT reuseTime, bool& kick, bool byHwid)
{
	CTL;
	bool done = false;
	if(byHwid)
	{
		UINT timeout = g_UserDB.GetInstanceTimeout(pSocket->pED->longHWID, instanceId);
		if(timeout > time(0))
		{
			kick = true;
			return;
		}
	}
	if(allowedTime > 0 && reuseTime > 0)
	{
		UINT currentTick = GetTickCount();
		//check if already exist
		for(UINT n=0;n<pED->instances.size();n++)
		{
			InstanceData& id = pED->instances[n];
			if(id.instanceId == instanceId)
			{
				//check if player can be in the zone
				if((id.timeSpent / 1000) > allowedTime)
				{
					//reached the limit check if reuse passed
					if(id.reuseTimeout < time(0))
					{
						//reuse time passed he can stay in the zone - reset stats
						id.timeSpent = 0;
						id.incTimeTick = 0;
						id.saved = false;
					}else
					{
						//reuse didn't pass yet - kickout
						if(!id.saved)
						{
							//save
							g_DB.RequestSaveInstanceTime(nDBID, id.instanceId, id.timeSpent / 1000, id.reuseTimeout);
							if(byHwid)
							{
								g_UserDB.SetInstanceTimeout(pSocket->pED->longHWID, instanceId, id.reuseTimeout);
							}
							id.saved = true;
						}
						kick = true;
						done = true;
						break;
					}
				}else
				{
					
					INT32 secInside = id.timeSpent / 1000;
					secInside = allowedTime - secInside;
					secInside--;
					
					//2536	1	a,You have $s1 second(s) left in this instance zone.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					if(secInside == 1800)
					{
						//30 min left
						CSystemMessage msg(2536);
						msg.AddNumber(secInside);
						SendSystemMessage(&msg);
					}else if(secInside == 900)
					{
						CSystemMessage msg(2536);
						msg.AddNumber(secInside);
						SendSystemMessage(&msg);
					}else if(secInside == 600)
					{
						CSystemMessage msg(2536);
						msg.AddNumber(secInside);
						SendSystemMessage(&msg);
					}else if(secInside == 300)
					{
						CSystemMessage msg(2536);
						msg.AddNumber(secInside);
						SendSystemMessage(&msg);
					}else if(secInside == 60)
					{
						CSystemMessage msg(2536);
						msg.AddNumber(secInside);
						SendSystemMessage(&msg);
					}else if(secInside == 30)
					{
						CSystemMessage msg(2536);
						msg.AddNumber(secInside);
						SendSystemMessage(&msg);
					}else if(secInside < 6 && secInside > 0)
					{
						CSystemMessage msg(2536);
						msg.AddNumber(secInside);
						SendSystemMessage(&msg);
					}
				}

				if(previousInstanceId != instanceId)
				{
					id.incTimeTick = 0;
				}

				if(id.incTimeTick != 0)
				{
					id.timeSpent += (currentTick - id.incTimeTick);
				//	g_Log.Add(CLog::Blue, "[%s] User[%S] instanceId[%d] timeSpent[%d]", __FUNCTION__, pSD->wszName, instanceId, id.timeSpent);
				}			
				id.incTimeTick = currentTick;
				id.reuseTimeout = static_cast<UINT>(time(0) + reuseTime);
				id.saved = false;
				done = true;
				break;
			}
		}
		if(!done)
		{
			InstanceData id;
			id.incTimeTick = GetTickCount();
			id.instanceId = instanceId;
			id.reuseTimeout = static_cast<UINT>(time(0) + reuseTime);
			pED->instances.push_back(id);
		}
	}
}
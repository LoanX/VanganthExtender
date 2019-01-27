#pragma once

#include "Handler.h"
#include "UserExPacket.h"

//Start of Function Array = 0x0BF88D0
//End of Function Array = 0x0BF8F58
//Start of ExPackets = 0x0BF8140
//End of ExPackets = 0x0BF81E0


extern "C"
{
	void GodsVoice(PWCHAR wStr);
};

class PacketHandler
{
public:
	static void Initialize();
	static void InsertHandler(int nID, UserSocketCallback func);
	static bool HandleUserPacket(CUserSocket *pSocket, const unsigned char* packet);
	static bool PacketAcceptable(User *pUser, int packetId);
};

static bool DummyPacket(CUserSocket* pSocket, const unsigned char* packet);
static bool CharacterCreatePacket(CUserSocket *pSocket, const unsigned char* packet);
static bool CharacterDeletePacket(CUserSocket *pSocket, const unsigned char* packet);
static bool MoveToLocationPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool ValidateLocationPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool AppearingPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestNetPing(CUserSocket *pSocket, const unsigned char* packet);
static bool CustomUserPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool ConfirmDialog(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestAcquireSkillInfo(CUserSocket *pSocket , const unsigned char* packet);
static bool ActionPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool AnswerJoinParty(CUserSocket *pSocket, const unsigned char* packet);
static bool AnswerJoinPledge(CUserSocket *pSocket, const unsigned char* packet);
static bool AttackPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool BuyPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool BypassToServerCMD(CUserSocket *pSocket, const unsigned char* packet);
static bool CharSelectPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool DropItem(CUserSocket *pSocket, const unsigned char* packet);
static bool GetItemPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool UseItemPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool DestroyItem(CUserSocket *pSocket, const unsigned char* packet);
static bool EnterWorld(CUserSocket *pSocket, const unsigned char* packet);
static bool GetItemFromPet(CUserSocket *pSocket, const unsigned char* packet);
static bool GiveItemToPet(CUserSocket *pSocket, const unsigned char* packet);
static bool GMCommand(CUserSocket *pSocket, const unsigned char* packet);
static bool IconAction(CUserSocket *pSocket, const unsigned char* packet);
static bool L2FriendSayPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool LinkHtml(CUserSocket *pSocket, const unsigned char* packet);
static bool LoginPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool LogoutPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool MagicSkillUsePacket(CUserSocket *pSocket, const unsigned char* packet);
static bool ManagePledgePower(CUserSocket *pSocket, const unsigned char* packet);
static bool MultisellChoose(CUserSocket *pSocket, const unsigned char* packet);
static bool PetGetItem(CUserSocket *pSocket, const unsigned char* packet);
static bool PrivateStoreBuyBuyListSend(CUserSocket *pSocket, const unsigned char* packet);
static bool PrivateStoreBuyListSend(CUserSocket *pSocket, const unsigned char* packet);
static bool PrivateStoreBuyListSet(CUserSocket *pSocket, const unsigned char* packet);
static bool PrivateStoreListSet(CUserSocket *pSocket, const unsigned char* packet);
static bool ProtocolVersion(CUserSocket *pSocket, const unsigned char* packet);
static bool ReplyListPartyWaitingPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool RestoreCharacter(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestAcquireSkill(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestJoinParty(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestJoinPledge(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestMakeMacro(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestPackageSend(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestPackageSendableItemList(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestRecipeShopSetList(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestRecipeBookItemMake(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestStartPledgeWar(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestStopPledgeWar(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestUserCommand(CUserSocket *pSocket, const unsigned char* packet);
static bool RequestSetCastleSiegeTimePacket(CUserSocket *pSocket, const unsigned char* packet);
static bool RestartPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool RestartPointPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool SayPacket2(CUserSocket *pSocket, const unsigned char* packet);
static bool SellPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool TradeAddItems(CUserSocket *pSocket, const unsigned char* packet);
static bool TradeDonePacket(CUserSocket *pSocket, const unsigned char* packet);
static bool TradeRequestPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool TradeStartPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool TutorialLinkHTML(CUserSocket *pSocket, const unsigned char* packet);
static bool TutorialPassCmdToServer(CUserSocket *pSocket, const unsigned char* packet);
static bool UserExPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool WarehouseDepositPacket(CUserSocket *pSocket, const unsigned char* packet);
static bool WarehouseWithdrawPacket(CUserSocket *pSocket, const unsigned char* packet);
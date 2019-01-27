#pragma once

#include "resource.h"
#include <assert.h>
#include "Log.h"
#include "SystemMessage.h"
#include "IOCriticalSection.h"
#include "SPointer.h"
#include "Config.h"
#include "CPacket.h"
#include "UserSocket.h"
#include "CSharedFactory.h"
#include "CSharedRWLock.h"
#include "CCreature.h"
#include "CPledge.h"
#include "CCastle.h"
#include "CWareHouseWithdraw.h"
#include "CWareHouseDeposit.h"
#include "User.h"
#include "CNPC.h"
#include "CSummon.h"
#include "CPet.h"
#include "CreatureController.h"
#include "CWeapon.h"
#include "CEtcItem.h"
#include "CAsset.h"
#include "CArmor.h"
#include "CAccessary.h"
#include "CQuestItem.h"
#include "L2Server.h"
#include "SkillFx.h"
#include "UserDB.h"
#include "ItemDBEx.h"
#include "CInventory.h"
#include "CParty.h"
#include "ServerPacketList.h"
#include "SpiritSystem.h"
#include "NpcServer.h"
#include "ClassExtender.h"
#include "NoblessData.h"
#include "AVESystem.h"
#include "ATSystem.h"
#include "BuffSlots.h"
#include "CSkillAction.h"
#include "L2Time.h"
#include "CObjectMap.h"
#include "CAttackAction.h"
#include "Quests.h"
#include "SkillMod.h"
#include "ShadowItem.h"
#include "PledgeDB.h"
#include "ResidenceDB.h"
#include "NpcSocket.h"
#include "FreightSystem.h"
#include "Parser.h"
#include "CategoryDataDB.h"
#include "EnchantedSet.h"
#include "Manor.h"
#include "SkillDBEx.h"
#include "TeamContainer.h"
#include "SiegeStat.h"
#include "ItemReuseManager.h"
#include "SkillReuseManager.h"
#include "LevelUP.h"
#include "TradeManager.h"
#include "KillDeathStat.h"
#include "Bypass.h"
#include "EraSystem.h"
#include "AdminSocket.h"
//Asm fixes
extern "C"
{
	void ItemDistribution();
}

class L2Ext
{
public:
	static void FixNPCnItemID();
	static void BaseData();
	static void EtcHook();
	static bool ValidateAddress(INT64 nAddress, int nLen);
};
#include "StdAfx.h"
#include "CursedWeaponSystem.h"
#include "Packets.h"
#include "AIO.h"
#include "TerritoryData.h"
#include "VisualArmor.h"
#include "PlayerCustomizer.h"
#include "SubStack.h"

extern UINT g_ClanLeaderTitleColor;
extern UINT g_ClanLeaderNicknameColor;
extern UINT g_GMTitleColor;
extern UINT g_GMNicknameColor;
extern UINT g_DefaultTitleColor;

void CPacketFix::UserInfoFix(CUserSocket *pSocket, const char *format, ...)
{
	guard;

	CPacket Packet;
	if(User *pUser = pSocket->GetUser())
	{

		va_list tag;
		va_start (tag, format); //start reading old packet data and assembling new packet
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//x
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//y
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//z
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//heading
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//object_id

			PWCHAR wName = va_arg(tag, PWCHAR);
			CNickNameChangeTerritory *pTerritory = NULL;
			if(int TerritoryID = pUser->pSD->nInNickNameChangeZone)
			{
				pTerritory = g_TerritoryChecker.GetTerritory(TerritoryID)->SafeCastNickNameChange();
				if(pTerritory)
				{
					PWCHAR wNewName = (PWCHAR)pTerritory->GetNickName();
					if(wNewName)
					{
						wName = wNewName;
					}
				}
			}
			/*S*/ Packet.WriteS(wName); //nickname

			DWORD Race = va_arg(tag, DWORD);
			DWORD Sex = va_arg(tag, DWORD);

	/*d*/ Packet.WriteD(Race);	//race
	/*d*/ Packet.WriteD(Sex);	//sex

			DWORD BaseClass = va_arg(tag, DWORD);
			
			if(g_SubStack.IsEnabled())
			{
				//get char index;
				UINT charIndex = UINT_MAX;
				for(UINT n=0;n<7;n++)
				{
					if(pSocket->characterDBID[n] == pUser->nDBID)
					{
						charIndex = n;
						break;
					}
				}
				if(charIndex != UINT_MAX && charIndex < 7)
				{
					if(pSocket->pED->baseClass[charIndex] != UINT_MAX)
					{
						BaseClass = pSocket->pED->baseClass[charIndex];
					}
				}
			}
	/*d*/ Packet.WriteD(BaseClass);	//class

	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//level
	/*d*/ Packet.WriteQ(va_arg(tag, INT64));	// - Exp will be Q in c5 format in format
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//str
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//dex
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//con
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//int
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//wit
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//men
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//max_hp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//current_hp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//max_mp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//current_mp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//sp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//current_load
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//max_load
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//unknown
	//PAPERDOLL_START objectId
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//dhair
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//rear
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//lear
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//neck
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//rfinger
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//lfinger
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//head
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//rhand
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//lhand
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//gloves
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//chest
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//legs
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//feet
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//back
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//lrhand
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//hair
	/*d*/ Packet.WriteD(0); 					//face - interlude addon
	//PAPERDOLL item class id
		INT32 dhairId = va_arg(tag, INT32);
		INT32 rearId = va_arg(tag, INT32);
		INT32 learId = va_arg(tag, INT32);
		INT32 neckId = va_arg(tag, INT32);
		INT32 rfingerId = va_arg(tag, INT32);
		INT32 lfingerId = va_arg(tag, INT32);
		INT32 headId = va_arg(tag, INT32);
		INT32 rhandId = va_arg(tag, INT32);
		INT32 lhandId = va_arg(tag, INT32);
		INT32 glovesId = va_arg(tag, INT32);
		INT32 chestId = va_arg(tag, INT32);
		INT32 legsId = va_arg(tag, INT32);
		INT32 feetId = va_arg(tag, INT32);
		INT32 backId = va_arg(tag, INT32);
		INT32 lrhandId = va_arg(tag, INT32);
		INT32 hairId = va_arg(tag, INT32);
		INT32 faceId = 0;
		if(g_VisualArmor.IsEnabled())
		{
			VisualArmorUser& vUser = pUser->pED->visualArmorUser;
			if(vUser.armorId > 0)
			{
				if(vUser.armorType == 0 || vUser.armorType == pUser->pSD->nArmorType)
				{
					if(chestId > 0)
					{
						chestId = vUser.chestSlotId;
					}
					if(legsId > 0)
					{
						legsId = vUser.legsSlotId;
					}
					if(glovesId > 0)
					{
						glovesId = vUser.glovesSlotId;
					}
					if(feetId > 0)
					{
						feetId = vUser.feetSlotId;
					}
				}
			}
		}
	/*d*/ Packet.WriteD(dhairId);	//dhair
	/*d*/ Packet.WriteD(rearId);	//rear
	/*d*/ Packet.WriteD(learId);	//lear
	/*d*/ Packet.WriteD(neckId);	//neck
	/*d*/ Packet.WriteD(rfingerId);	//rfinger
	/*d*/ Packet.WriteD(lfingerId);	//lfinger
	/*d*/ Packet.WriteD(headId);	//head
	/*d*/ Packet.WriteD(rhandId);	//rhand
	/*d*/ Packet.WriteD(lhandId);	//lhand
	/*d*/ Packet.WriteD(glovesId);	//gloves
	/*d*/ Packet.WriteD(chestId);	//chest
	/*d*/ Packet.WriteD(legsId);	//legs
	/*d*/ Packet.WriteD(feetId);	//feet
	/*d*/ Packet.WriteD(backId);	//back
	/*d*/ Packet.WriteD(lrhandId);	//lrhand
	/*d*/ Packet.WriteD(hairId);	//hair
	/*d*/ Packet.WriteD(faceId);						//face - interlude addon
	//PAPERDOLL_END

			UINT nAugmentation = pUser->GetAugmentationID();
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteD(nAugmentation);//Augmentation ID lhand
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteD(nAugmentation);//Augmentation ID lrhand
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//p_atk
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//p_atk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//p_def
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//evasion
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//accuracy
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//critical_rate
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//m_atk
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//m_atk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//p_atk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//m_def
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pvp_flag
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//karma
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//run_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//walk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//swim_run_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//swim_walk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fly_run_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fly_walk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fly_run_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fly_walk_speed
	/*f*/ Packet.WriteF(va_arg(tag, double));	//move_multipler
	/*f*/ Packet.WriteF(va_arg(tag, double));	//atk_speed_multipler

			double radius = va_arg(tag, double);
			double height = va_arg(tag, double);

	/*f*/ Packet.WriteF(radius);	//collision_radius
	/*f*/ Packet.WriteF(height);	//collision_height

	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//hair_style
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//hair_color
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//face
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//builder_level

			if(pTerritory && pTerritory->IsHidePledge())
			{
					va_arg(tag, PWCHAR);
			/*S*/ Packet.WriteS(L"");	//title
					va_arg(tag, DWORD);
			/*d*/ Packet.WriteD(0);	//pledge_id
					va_arg(tag, DWORD);
			/*d*/ Packet.WriteD(0);	//pledge_crest_id
					va_arg(tag, DWORD);
			/*d*/ Packet.WriteD(0);	//ally_id
					va_arg(tag, DWORD);
			/*d*/ Packet.WriteD(0);	//ally_crest_id
			}else
			{
			/*S*/ Packet.WriteS(va_arg(tag, PWCHAR));	//title
			/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pledge_id
			/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pledge_crest_id
			/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//ally_id
			/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//ally_crest_id
			}

	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//relation // 0x40 leader rights siege flags: attacker - 0x180 sword over name, defender - 0x80 shield, 0xC0 crown (|leader), 0x1C0 flag (|leader)
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//mount_type
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//private_store_mode
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//dwarven_craft_mode
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pk_count
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pvp_count
	/*h*/ Packet.WriteH(va_arg(tag, WORD));		//cubic_count
	/*BuffSize*/ DWORD CubicBuffSize = va_arg(tag, DWORD);	//cubic_buff_size
	/*b*/ Packet.WriteB(CubicBuffSize, va_arg(tag, PUCHAR)); //cubic_buff
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//find_party_members
	/*d*/ DWORD AbnormalVisualEffect = va_arg(tag, DWORD);	//abnormal_visual_effect
			if(pUser->pSD->pExData->SkillMod.semiInvisible)
			{
				AbnormalVisualEffect |= 0x100000;	//Stealth
			}
			if(pUser->pSD->activateOfflineShopTime)
			{
				AbnormalVisualEffect |= 0x000080; //Sleep
			}
			if(pUser->pED->spawnProtection == 2)
			{
				AbnormalVisualEffect |= 0x200000;
			}
			Packet.WriteD(AbnormalVisualEffect);
	/*c*/ Packet.WriteC(va_arg(tag, DWORD));	//unknown
	/*BuffSize*/ DWORD ClanPrivilegeBuffSize = va_arg(tag, DWORD); //c5fix = 4	//pledge_rights_buff_size
	/*b*/ PUCHAR ClanPrivilege = va_arg(tag, PUCHAR); //ClanPrivileges - 32 BYTES	//pledge_rights_buff
		Packet.WriteD(8388606);	//write static rights
	/*h*/ Packet.WriteH(va_arg(tag, WORD));		//recommendation_remaining
	/*h*/ Packet.WriteH(va_arg(tag, WORD));		//recommendation_received
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//unknown
	/*h*/ Packet.WriteH(va_arg(tag, WORD));		//inventory_limit
	
	UINT classId = va_arg(tag, DWORD);

	/*d*/ Packet.WriteD(classId);	//class_id

	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//unknown
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//max_cp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//current_cp
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//enchant_effect
	/*c*/ BYTE bTeam = va_arg(tag, BYTE);		//team_circle

			if(pUser->pSD->DuelInfo.DuelID && pUser->pSD->DuelInfo.Fighting)
				bTeam = pUser->pSD->DuelInfo.DuelTeam;

			if(pUser->pED->tvtUser.status == TvT::UserFighting || pUser->pED->tvtUser.status == TvT::UserPreparing)
				bTeam = pUser->pED->tvtUser.team;

			Packet.WriteC(bTeam); //1blue 2 red
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pledge_large_crest_id
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//is_nobless
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//is_hero
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//is_fishing
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fishing_x
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fishing_y
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fishing_z
		
			DWORD NickNameColor = va_arg(tag, DWORD);	//nickname_color
			if(pUser->pSD->nIsPledgeLeader && g_ClanLeaderNicknameColor != 0)
			{
				NickNameColor = g_ClanLeaderNicknameColor;
			}
			if(pUser->pSD->nBuilder && g_GMNicknameColor != 0)
			{
				NickNameColor = g_GMNicknameColor;
			}
			if(pUser->pED->nicknameColor != 0)
			{
				NickNameColor = pUser->pED->nicknameColor;
			}
			if(pUser->pED->nameColorEx != 0)
			{
				NickNameColor = pUser->pED->nameColorEx;
			}

			if(pUser->pED->aioId > 0 && pUser->pSD->nSubClassIndex == 0)
			{
				if(g_AIOSystem.IsEnabled())
				{
					NickNameColor = pUser->pED->aioNicknameColor;
				}
			}
			if(pTerritory)
			{
				if(DWORD Color = pTerritory->GetColor())
					NickNameColor = Color;
			}
	/*d*/ Packet.WriteD(NickNameColor);
		va_end(tag);

		//New C5 Things cdddddd
		Packet.WriteC((BYTE)pUser->pSD->nMoveType);
		Packet.WriteD(pUser->GetPledgeClass());
		Packet.WriteD(pUser->GetPledgeType());
		DWORD titleColor = g_DefaultTitleColor;
	
		if(pUser->pSD->nIsPledgeLeader && g_ClanLeaderTitleColor != 0)
		{
			titleColor = g_ClanLeaderTitleColor;
		}
				
		if(g_PlayerCustomizer.IsEnabled())
		{
			titleColor = g_PlayerCustomizer.GetPvpTitleColor(pUser->pSD->nPvpCount);
		}

		if(pUser->pSD->nBuilder && g_GMTitleColor != 0)
		{
			titleColor = g_GMTitleColor;
		}

		if(pUser->pED->titleColor != 0)
		{
			titleColor = pUser->pED->titleColor;
		}
		if(pUser->pED->titleColorEx != 0)
		{
			titleColor = pUser->pED->titleColorEx;
		}

		Packet.WriteD(titleColor);
		Packet.WriteD(pUser->pED->cursedWeaponLevel);
		Packet.WriteD(pUser->GetPledgeReputation());

		//SendNewInfo
		pSocket->Send("b", Packet.GetSize(), Packet.GetBuff());
	}

	unguard;
}

void CPacketFix::GMUserInfoFix(CUserSocket *pSocket, const char *format, ...)
{
	CPacket Packet;
	va_list tag;
	va_start (tag, format); //start reading old packet data and assembling new packet

	/*c*/ Packet.WriteC(va_arg(tag, BYTE));
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//x
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//y
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//z
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//heading
	UINT nObjectId = va_arg(tag, UINT);
	/*d*/ Packet.WriteD(nObjectId);	//object_id
	
	User *pUser = User::GetUserBySID(&nObjectId);
	
	if(pUser->ValidUser())
	{
		PWCHAR wName = va_arg(tag, PWCHAR);
		CNickNameChangeTerritory *pTerritory = NULL;
		if(int TerritoryID = pUser->pSD->nInNickNameChangeZone)
		{
			pTerritory = g_TerritoryChecker.GetTerritory(TerritoryID)->SafeCastNickNameChange();
			if(pTerritory)
			{
				PWCHAR wNewName = (PWCHAR)pTerritory->GetNickName();
				if(wNewName)
				{
					wName = wNewName;
				}
			}
		}
	/*S*/ Packet.WriteS(wName); //nickname

			DWORD Race = va_arg(tag, DWORD);
			DWORD Sex = va_arg(tag, DWORD);

	/*d*/ Packet.WriteD(Race);	//race
	/*d*/ Packet.WriteD(Sex);	//sex


	
	DWORD BaseClass = va_arg(tag, DWORD);
			
	if(g_SubStack.IsEnabled())
	{
		//get char index;
		UINT charIndex = UINT_MAX;
		for(UINT n=0;n<7;n++)
		{
			if(pSocket->characterDBID[n] == pUser->nDBID)
			{
				charIndex = n;
				break;
			}
		}
		if(charIndex != UINT_MAX && charIndex < 7)
		{
			if(pSocket->pED->baseClass[charIndex] != UINT_MAX)
			{
				BaseClass = pSocket->pED->baseClass[charIndex];
			}
		}
	}

	/*d*/ Packet.WriteD(BaseClass);	//class

	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//level
	/*d*/ Packet.WriteQ(va_arg(tag, INT64));	// - Exp will be Q in c5 format in format
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//str
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//dex
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//con
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//int
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//wit
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//men
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//max_hp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//current_hp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//max_mp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//current_mp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//sp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//current_load
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//max_load
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//unknown
	//PAPERDOLL_START objectId
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//dhair
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//rear
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//lear
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//neck
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//rfinger
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//lfinger
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//head
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//rhand
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//lhand
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//gloves
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//chest
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//legs
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//feet
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//back
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//lrhand
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//hair
	/*d*/ Packet.WriteD(0); 					//face - interlude addon
	//PAPERDOLL item class id
		INT32 dhairId = va_arg(tag, INT32);
		INT32 rearId = va_arg(tag, INT32);
		INT32 learId = va_arg(tag, INT32);
		INT32 neckId = va_arg(tag, INT32);
		INT32 rfingerId = va_arg(tag, INT32);
		INT32 lfingerId = va_arg(tag, INT32);
		INT32 headId = va_arg(tag, INT32);
		INT32 rhandId = va_arg(tag, INT32);
		INT32 lhandId = va_arg(tag, INT32);
		INT32 glovesId = va_arg(tag, INT32);
		INT32 chestId = va_arg(tag, INT32);
		INT32 legsId = va_arg(tag, INT32);
		INT32 feetId = va_arg(tag, INT32);
		INT32 backId = va_arg(tag, INT32);
		INT32 lrhandId = va_arg(tag, INT32);
		INT32 hairId = va_arg(tag, INT32);
		INT32 faceId = 0;
		if(g_VisualArmor.IsEnabled())
		{
			VisualArmorUser& vUser = pUser->pED->visualArmorUser;
			if(vUser.armorId > 0)
			{
				if(vUser.armorType == 0 || vUser.armorType == pUser->pSD->nArmorType)
				{
					if(chestId > 0)
					{
						chestId = vUser.chestSlotId;
					}
					if(legsId > 0)
					{
						legsId = vUser.legsSlotId;
					}
					if(glovesId > 0)
					{
						glovesId = vUser.glovesSlotId;
					}
					if(feetId > 0)
					{
						feetId = vUser.feetSlotId;
					}
				}
			}
		}
	/*d*/ Packet.WriteD(dhairId);	//dhair
	/*d*/ Packet.WriteD(rearId);	//rear
	/*d*/ Packet.WriteD(learId);	//lear
	/*d*/ Packet.WriteD(neckId);	//neck
	/*d*/ Packet.WriteD(rfingerId);	//rfinger
	/*d*/ Packet.WriteD(lfingerId);	//lfinger
	/*d*/ Packet.WriteD(headId);	//head
	/*d*/ Packet.WriteD(rhandId);	//rhand
	/*d*/ Packet.WriteD(lhandId);	//lhand
	/*d*/ Packet.WriteD(glovesId);	//gloves
	/*d*/ Packet.WriteD(chestId);	//chest
	/*d*/ Packet.WriteD(legsId);	//legs
	/*d*/ Packet.WriteD(feetId);	//feet
	/*d*/ Packet.WriteD(backId);	//back
	/*d*/ Packet.WriteD(lrhandId);	//lrhand
	/*d*/ Packet.WriteD(hairId);	//hair
	/*d*/ Packet.WriteD(faceId);						//face - interlude addon
	//PAPERDOLL_END

			UINT nAugmentation = pUser->GetAugmentationID();
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteD(nAugmentation);//Augmentation ID lhand
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteD(nAugmentation);//Augmentation ID lrhand
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
			Packet.WriteH(0x00);
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//p_atk
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//p_atk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//p_def
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//evasion
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//accuracy
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//critical_rate
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//m_atk
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//m_atk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//p_atk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//m_def
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pvp_flag
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//karma
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//run_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//walk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//swim_run_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//swim_walk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fly_run_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fly_walk_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fly_run_speed
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fly_walk_speed
	/*f*/ Packet.WriteF(va_arg(tag, double));	//move_multipler
	/*f*/ Packet.WriteF(va_arg(tag, double));	//atk_speed_multipler

			double radius = va_arg(tag, double);
			double height = va_arg(tag, double);

	/*f*/ Packet.WriteF(radius);	//collision_radius
	/*f*/ Packet.WriteF(height);	//collision_height

	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//hair_style
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//hair_color
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//face
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//builder_level

	/*S*/ Packet.WriteS(va_arg(tag, PWCHAR));	//title
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pledge_id
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pledge_crest_id
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//ally_id

	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//mount_type
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//private_store_mode
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//dwarven_craft_mode
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pk_count
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pvp_count
	/*h*/ Packet.WriteH(va_arg(tag, WORD));		//recommendation_remaining
	/*h*/ Packet.WriteH(va_arg(tag, WORD));		//recommendation_received

	UINT classId = va_arg(tag, DWORD);
	/*d*/ Packet.WriteD(classId);	//class_id
	
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//unknown
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//max_cp
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//current_cp
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//enchant_effect
	/*c*/ BYTE bTeam = va_arg(tag, BYTE);		//team_circle

			if(pUser->pSD->DuelInfo.DuelID && pUser->pSD->DuelInfo.Fighting)
				bTeam = pUser->pSD->DuelInfo.DuelTeam;

			if(pUser->pED->tvtUser.status == TvT::UserFighting || pUser->pED->tvtUser.status == TvT::UserPreparing)
				bTeam = pUser->pED->tvtUser.team;

			Packet.WriteC(bTeam); //1blue 2 red
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//pledge_large_crest_id
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//is_nobless
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//is_hero
	/*c*/ Packet.WriteC(va_arg(tag, BYTE));		//is_fishing
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fishing_x
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fishing_y
	/*d*/ Packet.WriteD(va_arg(tag, DWORD));	//fishing_z
		
			DWORD NickNameColor = va_arg(tag, DWORD);	//nickname_color
			if(pUser->pED->nicknameColor != 0)
			{
				NickNameColor = pUser->pED->nicknameColor;
			}
			if(pUser->pED->nameColorEx != 0)
			{
				NickNameColor = pUser->pED->nameColorEx;
			}
			if(pUser->pED->aioId > 0 && pUser->pSD->nSubClassIndex == 0)
			{
				if(g_AIOSystem.IsEnabled())
				{
					NickNameColor = pUser->pED->aioNicknameColor;
				}
			}
			if(pTerritory)
			{
				if(DWORD Color = pTerritory->GetColor())
					NickNameColor = Color;
			}
	/*d*/ Packet.WriteD(NickNameColor);
		va_end(tag);

		//New C5 Things cdddddd
		Packet.WriteC((BYTE)pUser->pSD->nMoveType);
		Packet.WriteD(pUser->GetPledgeClass());
		Packet.WriteD(pUser->GetPledgeType());
		if(g_PlayerCustomizer.IsEnabled())
		{
			Packet.WriteD(g_PlayerCustomizer.GetPvpTitleColor(pUser->pSD->nPvpCount));
		}else
		{
			Packet.WriteD(0x0ECF9A2);
		}
		Packet.WriteD(pUser->pED->cursedWeaponLevel);
		Packet.WriteD(pUser->GetPledgeReputation());
		
		//SendNewInfo
		pSocket->Send("b", Packet.GetSize(), Packet.GetBuff());
	}
}


void CPacketFix::MyTargetSelected(CSocket *pSocket, const char* format, ...)
{
	va_list va;
	va_start(va, format);
	//cdh -> cdhd
	BYTE opCode = va_arg(va, BYTE);
	DWORD targetId = va_arg(va, DWORD);
	WORD wColor = va_arg(va, WORD);
	va_end(va);
	
	pSocket->Send("cdhd", opCode, targetId, wColor, 0);
}
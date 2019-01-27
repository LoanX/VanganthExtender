#include "StdAfx.h"
#include "SkillDataParser.h"
#include "InstantEffects.h"
#include "PumpEffects.h"
#include "OperateConditions.h"

CPrecompileHeaderDB g_PrecompileHeaderDB;

static int nEffectID;
static bool bChangeID;
static int nSkillCount;
static map<wstring, int> m_SkillEffect;
static map<wstring, int> m_OperateCond;
static map<wstring, int> m_IntParam;

void SkillDataParser::Initialize()
{
	nEffectID = 0;
	nSkillCount = 0;
	bChangeID = false;
	AVESystem::Initialize();
	ATSystem::Initialize();
	
	//Extending Skill Info
	g_HookManager.WriteMemoryDWORD(0x8EF74A, sizeof(CSkillInfo));
	g_HookManager.WriteCall(0x8EF760, SkillInfoConstructor, 0);
	//Int Param
	g_HookManager.WriteJump(0x8EFA3E, CreateIntParamHook, 1);

	g_HookManager.WriteCall( 0x8F4BDF, CreatePumpEffectByWeapon, 0);
	g_HookManager.NopMemory(0x8F4BFF, 10);

	g_HookManager.WriteCall( 0x8F4058, CreatePumpEffectByArmor, 0);
	g_HookManager.NopMemory(0x8F4076, 10);

	g_HookManager.WriteCall( 0x8F4C5C, CreatePumpEffectOneArg, 0);
	g_HookManager.NopMemory(0x8F4C76, 10);

	g_HookManager.WriteCall( 0x8F4798, CreatePumpEffectNoArg, 0);
	g_HookManager.NopMemory(0x8F47A5, 10);


	g_HookManager.WriteCall( 0x8F606E, CreatePumpEffectTwoArgs, 0);
	g_HookManager.NopMemory(0x8F608A, 10);
	g_HookManager.WriteCall( 0x8F7013, CreateInstantEffect, 0);
	g_HookManager.NopMemory(0x8F7020, 10);
	g_HookManager.WriteCall( 0x8F27D4, CreateInstantEffectOneArg, 0);
	g_HookManager.NopMemory(0x8F27EE, 10);
	g_HookManager.WriteCall( 0x8F2828, CreateInstantEffectTwoArg, 0);
	g_HookManager.NopMemory(0x8F2848, 10);
	g_HookManager.WriteCall( 0x8DC0C1, GetName, 0);
	g_HookManager.WriteCall( 0x9090B2, ChangeWordID, 4);
	g_HookManager.WriteCall( 0x8F1C8E, CreateOperateConditionOneVar, 0);
	g_HookManager.NopMemory(0x8F1C9B, 10);
	g_HookManager.WriteCall( 0x79BFCC, LogParsingSkillDataFaild, 0);

	InitMaps();
}

CSkillInfo * SkillDataParser::SkillInfoConstructor(CSkillInfo *pSI)
{
	typedef CSkillInfo * (*f)(CSkillInfo*);

	pSI->nAttachSkillId = 0;
	pSI->nInstantCastTime = 0;
	pSI->nGeodataCheck = 0;

	return f(0x79B800L)(pSI);
}

void SkillDataParser::LogParsingSkillDataFaild(CLog *pLog, CLog::LogType Type, const char* format, wchar_t* File, int Line)
{
	wstring wFile = ReadFileW(L"..//Script//SkillData.txt");
	wstringstream sstr;
	sstr << wFile;
	int nLine = 0;
	int nSkills = 0;
	wstring wLine;
	while(getline(sstr, wLine))
	{
		if(wLine.find(L"//") == 0)
		{
			nLine++;
			continue;
		}else if(wLine.find(L"skill_begin") == 0)
		{
			nLine++;
			nSkills++;
		}else
		{
			nLine++;
		}
		if(nSkills == nSkillCount)
		{
			break;
		}
	}
	g_Log.Add(CLog::Error, "Parsing SkillData file failed! - SkillCount[%d] Line[%d]", nSkillCount, nLine);
}

void SkillDataParser::InitMaps()
{
	//SKILL EFFECTS
	m_SkillEffect[(wstring)L"p_critical_rate_by_weapon"] = P_CRITICAL_RATE_BY_WEAPON;
	m_SkillEffect[(wstring)L"p_critical_damage_by_weapon"] = P_CRITICAL_DAMAGE_BY_WEAPON;
	m_SkillEffect[(wstring)L"p_critical_rate_by_hp2_weapon"] = P_CRITICAL_RATE_BY_HP2_WEAPON;
	m_SkillEffect[(wstring)L"p_critical_rate_by_hp1_weapon"] = P_CRITICAL_RATE_BY_HP1_WEAPON;
	m_SkillEffect[(wstring)L"p_physical_attack_by_hp2_weapon"] = P_PHYSICAL_ATTACK_BY_HP2_WEAPON;
	m_SkillEffect[(wstring)L"p_avoid_melee_skill"] = P_AVOID_MELEE_SKILL;
	m_SkillEffect[(wstring)L"p_cancel_attacker_target"] = P_CANCEL_ATTACKER_TARGET;
	m_SkillEffect[(wstring)L"p_reflect_melee_skill_dmg"] = P_REFLECT_MELEE_SKILL_DMG;
	m_SkillEffect[(wstring)L"p_self_resurrection"] = P_SELF_RESURRECTION;
	m_SkillEffect[(wstring)L"i_summon_erase"] = I_SUMMON_ERASE;
	m_SkillEffect[(wstring)L"i_summon_friend"] = I_SUMMON_FRIEND;
	m_SkillEffect[(wstring)L"i_target_cancel_chance"] = I_TARGET_CANCEL_CHANCE;
	m_SkillEffect[(wstring)L"i_target_me_chance"] = I_TARGET_ME_CHANCE;
	m_SkillEffect[(wstring)L"i_mana_burn"] = I_MANA_BURN;
	m_SkillEffect[(wstring)L"i_lift_death_penalty"] = I_LIFT_DEATH_PENALTY;
	m_SkillEffect[(wstring)L"p_summon_betray"] = P_SUMMON_BETRAY;
	m_SkillEffect[(wstring)L"i_death2"] = I_DEATH2;
	m_SkillEffect[(wstring)L"i_spawn_casting_npc"] = I_SPAWN_CASTING_NPC;
	m_SkillEffect[(wstring)L"p_enlarge_buff_slots"] = P_ENLARGE_BUFF_SLOTS;
	m_SkillEffect[(wstring)L"p_autocast_when_attacked"] = P_AUTOCAST_WHEN_ATTACKED;
	m_SkillEffect[(wstring)L"i_summon_party"] = I_SUMMON_PARTY;
	m_SkillEffect[(wstring)L"p_autocast_on_matk"] = P_AUTOCAST_ON_MATK;
	m_SkillEffect[(wstring)L"p_autocast_on_patk"] = P_AUTOCAST_ON_PATK;
	m_SkillEffect[(wstring)L"p_autocast_on_crit"] = P_AUTOCAST_ON_CRIT;
	m_SkillEffect[(wstring)L"i_teleport_distance"] = I_TELEPORT_DISTANCE;
	m_SkillEffect[(wstring)L"p_reduce_exp_penalty"] = P_REDUCE_EXP_PENALTY;
	m_SkillEffect[(wstring)L"i_charge"] = I_CHARGE;
	m_SkillEffect[(wstring)L"p_block_icon_action"] = P_BLOCK_ICON_ACTION;
	m_SkillEffect[(wstring)L"i_dispel_by_category_count"] = I_DISPEL_BY_CATEGORY_COUNT;
	m_SkillEffect[(wstring)L"i_consume_energy"] = I_CONSUME_ENERGY;
	m_SkillEffect[(wstring)L"p_improve_heal"] = P_IMPROVE_HEAL;
	m_SkillEffect[(wstring)L"i_release_spirit"] = I_RELEASE_SPIRIT;
	m_SkillEffect[(wstring)L"i_hide_abnormal"] = I_HIDE_ABNORMAL;
	m_SkillEffect[(wstring)L"i_unhide_abnormal"] = I_UNHIDE_ABNORMAL;
	m_SkillEffect[(wstring)L"p_critical_damage_position_front_by_weapon"] = P_CRITICAL_DAMAGE_POSITION_FRONT_BY_WEAPON;
	m_SkillEffect[(wstring)L"p_critical_damage_position_side_by_weapon"] = P_CRITICAL_DAMAGE_POSITION_SIDE_BY_WEAPON;
	m_SkillEffect[(wstring)L"p_critical_damage_position_back_by_weapon"] = P_CRITICAL_DAMAGE_POSITION_BACK_BY_WEAPON;
	m_SkillEffect[(wstring)L"i_dispel_npc_buff"] = I_DISPEL_NPC_BUFF;
	m_SkillEffect[L"i_dispel_debuff_with_exception"] = I_DISPEL_DEBUFF_WITH_EXCEPTION;
	m_SkillEffect[(wstring)L"i_kill_myself"] = I_KILL_MYSELF;
	m_SkillEffect[(wstring)L"p_strife_cp"] = P_STRIFE_CP;
	m_SkillEffect[(wstring)L"p_magic_speed_by_weapon"] = P_MAGIC_SPEED_BY_WEAPON;
	m_SkillEffect[(wstring)L"p_anti_buff"] = P_ANTI_BUFF;
	m_SkillEffect[(wstring)L"p_mul_skill_dmg"] = P_MUL_SKILL_DMG;
	m_SkillEffect[(wstring)L"p_digger_chance"] = P_DIGGER_CHANCE;
	m_SkillEffect[(wstring)L"i_dig"] = I_DIG;
	m_SkillEffect[(wstring)L"i_use_skill"] = I_USE_SKILL;
	m_SkillEffect[(wstring)L"i_ride_wyvern"] = I_RIDE_WYVERN;
	m_SkillEffect[(wstring)L"i_dispel_by_chance"] = I_DISPEL_BY_CHANCE;
	m_SkillEffect[(wstring)L"p_defence_attribute_by_armor"] = P_DEFENCE_ATTRIBUTE_BY_ARMOR;

	m_SkillEffect[(wstring)L"p_speed_by_weapon"] = P_SPEED_BY_WEAPON;
	m_SkillEffect[(wstring)L"p_physical_defence_by_weapon"] = P_PHYSICAL_DEFENCE_BY_WEAPON;
	m_SkillEffect[(wstring)L"p_magical_defence_by_weapon"] = P_MAGICAL_DEFENCE_BY_WEAPON;
	m_SkillEffect[(wstring)L"p_avoid_by_weapon"] = P_AVOID_BY_WEAPON;
	m_SkillEffect[(wstring)L"p_semi_invisible"] = P_SEMI_INVISIBLE;

	m_SkillEffect[(wstring)L"p_bonus_exp"] = P_BONUS_EXP;
	m_SkillEffect[(wstring)L"p_bonus_sp"] = P_BONUS_SP;
	m_SkillEffect[(wstring)L"p_bonus_item_drop"] = P_BONUS_ITEM_DROP;
	m_SkillEffect[(wstring)L"p_bonus_adena"] = P_BONUS_ADENA;
	m_SkillEffect[(wstring)L"p_bonus_spoil"] = P_BONUS_SPOIL;
	m_SkillEffect[(wstring)L"p_bonus_harvesting"] = P_BONUS_HARVESTING;
	m_SkillEffect[(wstring)L"p_bonus_rb"] = P_BONUS_RB;
	m_SkillEffect[(wstring)L"p_bonus_epic_rb"] = P_BONUS_EPIC_RB;

	m_SkillEffect[(wstring)L"i_dispel_buff"] = I_DISPEL_BUFF;
	m_SkillEffect[(wstring)L"p_bonus_item_id"] = P_BONUS_ITEM_ID;
	m_SkillEffect[(wstring)L"i_give_item"] = I_GIVE_ITEM;
	m_SkillEffect[(wstring)L"i_acquire_skill"] = I_ACQUIRE_SKILL;
	m_SkillEffect[(wstring)L"i_charm_of_courage"] = I_CHARM_OF_COURAGE;
	m_SkillEffect[(wstring)L"i_additional_box"] = I_ADDITIONAL_BOX;
	m_SkillEffect[L"p_anti_pk"] = P_ANTI_PK;
	m_SkillEffect[L"i_dispel_buff_fixed"] = I_DISPEL_BUFF_FIXED;
	m_SkillEffect[L"p_attack_speed_by_weapon2"] = P_ATTACK_SPEED_BY_WEAPON2;
	m_SkillEffect[L"p_resist_stun"] = P_RESIST_STUN;
	m_SkillEffect[L"i_pledge_name_value"] = I_PLEDGE_NAME_VALUE;
	m_SkillEffect[L"i_update_pledge_skill"] = I_UPDATE_PLEDGE_SKILL;

	m_SkillEffect[L"p_regen_cp"] = P_REGEN_CP;
	m_SkillEffect[L"i_add_enchanted_item"] = I_ADD_ENCHANTED_ITEM;
	m_SkillEffect[L"i_update_eval_score"] = I_UPDATE_EVAL_SCORE;
	m_SkillEffect[L"i_set_aio"] = I_SET_AIO;
	m_SkillEffect[L"i_set_nobless"] = I_SET_NOBLESS;
	m_SkillEffect[L"i_show_menu"] = I_SHOW_MENU;
	
	//OPERATE CONDITIONS
	m_OperateCond[(wstring)L"op_battle_force_count"] = OP_BATTLE_FORCE_COUNT;
	m_OperateCond[(wstring)L"op_spell_force_count"] = OP_SPELL_FORCE_COUNT;
	m_OperateCond[(wstring)L"op_remain_mp_per"] = OP_REMAIN_MP_PER;
	m_OperateCond[(wstring)L"op_max_height_diff"] = OP_MAX_HEIGHT_DIFF;
	m_OperateCond[(wstring)L"op_not_class"] = OP_NOT_CLASS;
	m_OperateCond[(wstring)L"op_not_in_territory"] = OP_NOT_IN_TERRITORY;
	m_OperateCond[(wstring)L"op_is_type"] = OP_IS_TYPE;
	m_OperateCond[(wstring)L"op_strife_not_owned_by"] = OP_STRIFE_NOT_OWNED_BY;
	m_OperateCond[(wstring)L"op_collected_soul_count"] = OP_COLLECTED_SOUL_COUNT;
	m_OperateCond[(wstring)L"op_is_in_siege"] = OP_IS_IN_SIEGE;
	m_OperateCond[(wstring)L"op_hour_lower_than"] = OP_HOUR_LOWER_THAN;
	m_OperateCond[(wstring)L"op_hour_higher_than"] = OP_HOUR_HIGHER_THAN;
	m_OperateCond[(wstring)L"op_can_dig_at"] = OP_CAN_DIG;
	m_OperateCond[(wstring)L"op_can_release_spirit"] = OP_CAN_RELEASE_SPIRIT;
	m_OperateCond[(wstring)L"op_can_acquire_skill"] = OP_CAN_ACQUIRE_SKILL;
	m_OperateCond[L"op_can_get_item"] = OP_CAN_GET_ITEM;
	m_OperateCond[L"op_combat_mode"] = OP_COMBAT_MODE;
	m_OperateCond[L"op_can_update_pledge_skill"] = OP_CAN_UPDATE_PLEDGE_SKILL;
	m_OperateCond[L"op_in_peacezone"] = OP_IN_PEACEZONE;

	//Int params
	m_IntParam[L"attach_skill"] = IP_ATTACH_SKILL;
	m_IntParam[L"instant_activate_time"] = IP_INSTANT_ACTIVATE_TIME;
	m_IntParam[L"resist_cancel"] = IP_RESIST_CANCEL;
	m_IntParam[L"geodata_check"] = IP_GEODATA_CHECK;
}

int SkillDataParser::ChangeWordID(LPVOID Param1)
{
	typedef int (*_getID)(void*);
	_getID getID = (_getID) 0x00909A70;
	int nID = getID(Param1);

	if((*(LPDWORD)Param1) == 0xAE2538)
	{
		if(nEffectID == P_DEFENCE_ATTRIBUTE_BY_ARMOR)
		{
			//change format attribute_name -> diff
			if(nID == 65831)
				nID = 65829;
		}
	}

	if(bChangeID)
	{
		switch(nEffectID)
		{
		case P_CRITICAL_RATE_BY_WEAPON:
			{
				bChangeID = false;
			//	g_Log.Add(CLog::Blue, "[SkillDataParser] Fixing ID[%d] to [%d]" , nID, PUMP_EFFECT_BY_WEAPON);
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_REGEN_CP:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
			}
		case P_CRITICAL_DAMAGE_POSITION_FRONT_BY_WEAPON:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_CRITICAL_DAMAGE_POSITION_SIDE_BY_WEAPON:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_CRITICAL_DAMAGE_POSITION_BACK_BY_WEAPON:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_CRITICAL_DAMAGE_BY_WEAPON:
			{
				bChangeID = false;
			//	g_Log.Add(CLog::Blue, "[SkillDataParser] Fixing ID[%d] to [%d]" , nID, PUMP_EFFECT_BY_WEAPON);
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_CRITICAL_RATE_BY_HP2_WEAPON:
			{
				bChangeID = false;
			//	g_Log.Add(CLog::Blue, "[SkillDataParser] Fixing ID[%d] to [%d]" , nID, PUMP_EFFECT_BY_WEAPON);
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_CRITICAL_RATE_BY_HP1_WEAPON:
			{
				bChangeID = false;
			//	g_Log.Add(CLog::Blue, "[SkillDataParser] Fixing ID[%d] to [%d]" , nID, PUMP_EFFECT_BY_WEAPON);
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_SPEED_BY_WEAPON:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_ATTACK_SPEED_BY_WEAPON2:
			{
				bChangeID = false;
				return P_ATTACK_SPEED_BY_WEAPON2;
				break;
			}
		case P_PHYSICAL_DEFENCE_BY_WEAPON:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_MAGICAL_DEFENCE_BY_WEAPON:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_AVOID_BY_WEAPON:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_PHYSICAL_ATTACK_BY_HP2_WEAPON:
			{
				bChangeID = false;
			//	g_Log.Add(CLog::Blue, "[SkillDataParser] Fixing ID[%d] to [%d]" , nID, PUMP_EFFECT_BY_WEAPON);
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_AVOID_MELEE_SKILL:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_CANCEL_ATTACKER_TARGET:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_REFLECT_MELEE_SKILL_DMG:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_SELF_RESURRECTION:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_MAGIC_SPEED_BY_WEAPON:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_BONUS_EXP:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_BONUS_SP:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_BONUS_ITEM_DROP:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_BONUS_ADENA:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_BONUS_SPOIL:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_BONUS_HARVESTING:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
			
		case P_BONUS_RB:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_BONUS_EPIC_RB:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_WEAPON;
				break;
			}
		case P_ENLARGE_BUFF_SLOTS:
			{
				bChangeID = false;
				return PUMP_EFFECT_ONE_ARG;
				break;
			}
		case P_REDUCE_EXP_PENALTY:
			{
				bChangeID = false;
				return PUMP_EFFECT_ONE_ARG;
				break;
			}
		case P_STRIFE_CP:
			{
				bChangeID = false;
				return PUMP_EFFECT_ONE_ARG;
				break;
			}
		case P_SUMMON_BETRAY:
			{
				bChangeID = false;
				return PUMP_EFFECT_ONE_ARG;
				break;
			}
		case P_DIGGER_CHANCE:
			{
				bChangeID = false;
				return PUMP_EFFECT_ONE_ARG;
			}
		case I_SUMMON_ERASE:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_SUMMON_PARTY:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_DIG:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_SUMMON_FRIEND:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_TARGET_CANCEL_CHANCE:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_SET_AIO:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
			}
		case I_UPDATE_EVAL_SCORE:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
			}
		case I_TARGET_ME_CHANCE:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_DISPEL_NPC_BUFF:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_DISPEL_DEBUFF_WITH_EXCEPTION:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
			}
		case I_ADDITIONAL_BOX:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_MANA_BURN:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case I_DISPEL_BUFF:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case I_LIFT_DEATH_PENALTY:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_USE_SKILL:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_DEATH2:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case I_DISPEL_BUFF_FIXED:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case I_ADD_ENCHANTED_ITEM:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
			}
		case I_ACQUIRE_SKILL:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case I_SPAWN_CASTING_NPC:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case P_AUTOCAST_WHEN_ATTACKED:
			{
				bChangeID = false;
				return PUMP_EFFECT_TWO_ARGS;
				break;
			}
		case P_AUTOCAST_ON_MATK:
			{
				bChangeID = false;
				return PUMP_EFFECT_TWO_ARGS;
				break;
			}
		case P_AUTOCAST_ON_PATK:
			{
				bChangeID = false;
				return PUMP_EFFECT_TWO_ARGS;
				break;
			}
		case P_ANTI_PK:
			{
				bChangeID = false;
				return PUMP_EFFECT_TWO_ARGS;
				break;
			}
		case P_BONUS_ITEM_ID:
			{
				bChangeID = false;
				return PUMP_EFFECT_TWO_ARGS;
				break;
			}
		case P_AUTOCAST_ON_CRIT:
			{
				bChangeID = false;
				return PUMP_EFFECT_TWO_ARGS;
				break;
			}
		case P_MUL_SKILL_DMG:
			{
				bChangeID = false;
				return PUMP_EFFECT_TWO_ARGS;
				break;
			}
		case I_TELEPORT_DISTANCE:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_CHARGE:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case I_KILL_MYSELF:
			{
				bChangeID = false;
				return INSTANT_EFFECT;
				break;
			}
		case I_SET_NOBLESS:
			{
				bChangeID = false;
				return INSTANT_EFFECT;
			}
		case I_CHARM_OF_COURAGE:
			{
				bChangeID = false;
				return INSTANT_EFFECT;
				break;
			}
		case I_SHOW_MENU:
			{
				bChangeID = false;
				return INSTANT_EFFECT;
			}
		case I_RIDE_WYVERN:
			{
				bChangeID = false;
				return INSTANT_EFFECT;
				break;
			}
		case P_BLOCK_ICON_ACTION:
			{
				bChangeID = false;
				return PUMP_EFFECT_ONE_ARG;
				break;
			}
		case I_DISPEL_BY_CATEGORY_COUNT:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case I_DISPEL_BY_CHANCE:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case I_GIVE_ITEM:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case I_CONSUME_ENERGY:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
				break;
			}
		case I_PLEDGE_NAME_VALUE:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
			}
		case I_UPDATE_PLEDGE_SKILL:
			{
				bChangeID = false;
				return INSTANT_EFFECT_ONE_ARG;
			}
		case P_IMPROVE_HEAL:
			{
				bChangeID = false;
				return PUMP_EFFECT_ONE_ARG;
				break;
			}
		case I_RELEASE_SPIRIT:
			{
				bChangeID = false;
				return INSTANT_EFFECT;
				break;
			}
		case I_UNHIDE_ABNORMAL:
			{
				bChangeID = false;
				return INSTANT_EFFECT;
				break;
			}
		case I_HIDE_ABNORMAL:
			{
				bChangeID = false;
				return INSTANT_EFFECT_TWO_ARG;
				break;
			}
		case P_ANTI_BUFF:
			{
				bChangeID = false;
				return PUMP_EFFECT_ONE_ARG;
				break;
			}
		case P_RESIST_STUN:
			{
				bChangeID = false;
				return PUMP_EFFECT_ONE_ARG;
				break;
			}
		case P_DEFENCE_ATTRIBUTE_BY_ARMOR:
			{
				bChangeID = false;
				return PUMP_EFFECT_BY_ARMOR;
				break;
			}
		case P_SEMI_INVISIBLE:
			{
				bChangeID = false;
				return PUMP_EFFECT_NO_ARG;
				break;
			}
		case OP_BATTLE_FORCE_COUNT:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_SPELL_FORCE_COUNT:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_REMAIN_MP_PER:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_MAX_HEIGHT_DIFF:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_IS_IN_SIEGE:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_NOT_CLASS:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_NOT_IN_TERRITORY:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_IS_TYPE:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_STRIFE_NOT_OWNED_BY:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_COLLECTED_SOUL_COUNT:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_HOUR_HIGHER_THAN:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_HOUR_LOWER_THAN:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_CAN_DIG:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_CAN_RELEASE_SPIRIT:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_CAN_ACQUIRE_SKILL:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_CAN_GET_ITEM:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_COMBAT_MODE:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_CAN_UPDATE_PLEDGE_SKILL:
			{
				bChangeID = false;
				return OP_ONE_VAR;
				break;
			}
		case OP_IN_PEACEZONE:
			{
				bChangeID = false;
				return OP_ONE_VAR;
			}
		case IP_ATTACH_SKILL:
			{
				bChangeID = false;
				return INT_PARAM;
				break;
			}
		case IP_INSTANT_ACTIVATE_TIME:
			{
				bChangeID = false;
				return INT_PARAM;
				break;
			}
		case IP_RESIST_CANCEL:
			{
				bChangeID = false;
				return INT_PARAM;
				break;
			}
		case IP_GEODATA_CHECK:
			{
				bChangeID = false;
				return INT_PARAM;
			}
		default:
			{
				bChangeID = false;
				return nID;
				break;
			}
		}	
	}else
		return nID;
}

void CreateIntParam(CSkillInfo *pSI, INT32 param)
{
	switch(nEffectID)
	{
	case IP_ATTACH_SKILL:
		{
			pSI->nAttachSkillId = param;
			nEffectID = 0;
			return;
		}
	case IP_INSTANT_ACTIVATE_TIME:
		{
			pSI->nInstantCastTime = param;
			nEffectID = 0;
			return;
		}
	case IP_RESIST_CANCEL:
		{
			pSI->nResistCancel = param;
			nEffectID = 0;
			return;
		}
	case IP_GEODATA_CHECK:
		{
			pSI->nGeodataCheck = param;
			nEffectID = 0;
			return;
		}
	default:
		{
			pSI->nConsumeHP = param;
			return;
		}
	}
}

LPVOID SkillDataParser::CreatePumpEffectNoArg(int nSize)
{
	switch(nEffectID)
	{
	case P_SEMI_INVISIBLE:
		{
			CSkillEffect_p_semi_invisible *pEffect = new CSkillEffect_p_semi_invisible;
			nEffectID = 0;
			return pEffect;
			break;
		}
	default:
		{
			LPVOID pEffect = new char[nSize];
			memset(pEffect, 0, nSize);
			if(pEffect)
				(*(DWORD*)pEffect) = 0xADA058;
			return pEffect;
			break;
		}
	}
}

LPVOID SkillDataParser::CreatePumpEffectOneArg(int nSize)
{
	switch(nEffectID)
	{
	case P_ENLARGE_BUFF_SLOTS:
		{
			CSkillEffect_p_enlarge_buff_slots *pEffect = new CSkillEffect_p_enlarge_buff_slots;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_IMPROVE_HEAL:
		{
			CSkillEffect_p_improve_heal *pEffect = new CSkillEffect_p_improve_heal;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BLOCK_ICON_ACTION:
		{
			CSkillEffect_p_block_icon_action *pEffect = new CSkillEffect_p_block_icon_action;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_REDUCE_EXP_PENALTY:
		{
			CSkillEffect_p_reduce_exp_penalty *pEffect = new CSkillEffect_p_reduce_exp_penalty;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_SUMMON_BETRAY:
		{
			CSkillEffect_p_summon_betray *pEffect = new CSkillEffect_p_summon_betray;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_STRIFE_CP:
		{
			CSkillEffect_p_strife_cp *pEffect = new CSkillEffect_p_strife_cp;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_ANTI_BUFF:
		{
			CSkillEffect_p_anti_buff* pEffect = new CSkillEffect_p_anti_buff;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_DIGGER_CHANCE:
		{
			CSkillEffect_p_digger_chance* pEffect = new CSkillEffect_p_digger_chance;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_RESIST_STUN:
		{
			CSkillEffect_p_resist_stun* pEffect = new CSkillEffect_p_resist_stun;
			nEffectID = 0;
			return pEffect;
			break;
		}
	default:
		{
			LPVOID pEffect = new char[nSize];
			memset(pEffect, 0, nSize);
			if(pEffect)
				*(DWORD*)pEffect = 0xADA538;
			return pEffect;
			break;
		}
	}
}

LPVOID SkillDataParser::CreatePumpEffectTwoArgs(int nSize)
{
	switch(nEffectID)
	{
	case P_AUTOCAST_WHEN_ATTACKED:
		{
			CSkillEffect_p_autocast_when_attacked *pEffect = new CSkillEffect_p_autocast_when_attacked;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_AUTOCAST_ON_MATK:
		{
			CSkillEffect_p_autocast_on_matk *pEffect = new CSkillEffect_p_autocast_on_matk;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_AUTOCAST_ON_PATK:
		{
			CSkillEffect_p_autocast_on_patk *pEffect = new CSkillEffect_p_autocast_on_patk;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_AUTOCAST_ON_CRIT:
		{
			CSkillEffect_p_autocast_on_crit *pEffect = new CSkillEffect_p_autocast_on_crit;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_MUL_SKILL_DMG:
		{
			CSkillEffect_p_mul_skill_dmg *pEffect = new CSkillEffect_p_mul_skill_dmg;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BONUS_ITEM_ID:
		{
			CSkillEffect_p_bonus_item_id *pEffect = new CSkillEffect_p_bonus_item_id;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_ANTI_PK:
		{
			CSkillEffect_p_anti_pk *pEffect = new CSkillEffect_p_anti_pk;
			nEffectID = 0;
			return pEffect;
			break;
		}
	default:
		{
			LPVOID pEffect = new char[nSize];
			memset(pEffect, 0, nSize);
			if(pEffect)
				*(DWORD*)pEffect = 0xADBA98;
			return pEffect;
			break;
		}
	}
}

LPVOID SkillDataParser::CreatePumpEffectByArmor(int nSize)
{
	switch(nEffectID)
	{
	case P_DEFENCE_ATTRIBUTE_BY_ARMOR:
		{
			CSkillEffect_p_defence_attribute_by_armor *pEffect = new CSkillEffect_p_defence_attribute_by_armor;
			nEffectID = 0;
			return pEffect;
			break;
		}
	default:
		{
			LPVOID pEffect = new char[nSize];
			memset(pEffect, 0, nSize);
			if(pEffect)
				(*(DWORD*)pEffect) = 0xAD9BD8;
			return pEffect;
			break;
		}
	}
}

LPVOID SkillDataParser::CreatePumpEffectByWeapon(int nSize)
{
	switch(nEffectID)
	{
	case P_CRITICAL_RATE_BY_WEAPON:
		{
			CSkillEffect_p_critical_rate_by_weapon *pEffect = new CSkillEffect_p_critical_rate_by_weapon;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating p_critical_rate_by_weapon effect");
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_SPEED_BY_WEAPON:
		{
			CSkillEffect_p_speed_by_weapon *pEffect = new CSkillEffect_p_speed_by_weapon;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_PHYSICAL_DEFENCE_BY_WEAPON:
		{
			CSkillEffect_p_physical_defence_by_weapon *pEffect = new CSkillEffect_p_physical_defence_by_weapon;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_MAGICAL_DEFENCE_BY_WEAPON:
		{
			CSkillEffect_p_magical_defence_by_weapon *pEffect = new CSkillEffect_p_magical_defence_by_weapon;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_AVOID_BY_WEAPON:
		{
			CSkillEffect_p_avoid_by_weapon *pEffect = new CSkillEffect_p_avoid_by_weapon;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_CRITICAL_DAMAGE_BY_WEAPON:
		{
			CSkillEffect_p_critical_damage_by_weapon *pEffect = new CSkillEffect_p_critical_damage_by_weapon;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating p_critical_damage_by_weapon effect");
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_CRITICAL_RATE_BY_HP2_WEAPON:
		{
			CSkillEffect_p_critical_rate_by_hp2_weapon *pEffect = new CSkillEffect_p_critical_rate_by_hp2_weapon;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating p_critical_rate_by_hp2_weapon effect");
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_CRITICAL_RATE_BY_HP1_WEAPON:
		{
			CSkillEffect_p_critical_rate_by_hp1_weapon *pEffect = new CSkillEffect_p_critical_rate_by_hp1_weapon;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating p_critical_rate_by_hp1_weapon effect");
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_PHYSICAL_ATTACK_BY_HP2_WEAPON:
		{
			CSkillEffect_p_physical_attack_by_hp2_weapon *pEffect = new CSkillEffect_p_physical_attack_by_hp2_weapon;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating p_critical_damage_by_weapon effect");
			nEffectID = 0;
			return pEffect;
			break;
		}	
	case P_AVOID_MELEE_SKILL:
		{
			CSkillEffect_p_avoid_melee_skill *pEffect = new CSkillEffect_p_avoid_melee_skill;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating p_critical_damage_by_weapon effect");
			nEffectID = 0;
			return pEffect;
			break;
		}	
	case P_CANCEL_ATTACKER_TARGET:
		{
			CSkillEffect_p_cancel_attacker_target *pEffect = new CSkillEffect_p_cancel_attacker_target;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating p_critical_damage_by_weapon effect");
			nEffectID = 0;
			return pEffect;
			break;
		}	
	case P_REFLECT_MELEE_SKILL_DMG:
		{
			CSkillEffect_p_reflect_melee_skill_dmg *pEffect = new CSkillEffect_p_reflect_melee_skill_dmg;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating p_critical_damage_by_weapon effect");
			nEffectID = 0;
			return pEffect;
			break;
		}	
	case P_SELF_RESURRECTION:
		{
			CSkillEffect_p_self_resurrection *pEffect = new CSkillEffect_p_self_resurrection;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating p_critical_damage_by_weapon effect");
			nEffectID = 0;
			return pEffect;
			break;
		}	
	case P_CRITICAL_DAMAGE_POSITION_FRONT_BY_WEAPON:
		{
			CSkillEffect_p_critical_damage_position_front_by_weapon *pEffect = new CSkillEffect_p_critical_damage_position_front_by_weapon;
			nEffectID = 0;
			return pEffect;
			break;
		}	
	case P_CRITICAL_DAMAGE_POSITION_SIDE_BY_WEAPON:
		{
			CSkillEffect_p_critical_damage_position_side_by_weapon *pEffect = new CSkillEffect_p_critical_damage_position_side_by_weapon;
			nEffectID = 0;
			return pEffect;
			break;
		}	
	case P_CRITICAL_DAMAGE_POSITION_BACK_BY_WEAPON:
		{
			CSkillEffect_p_critical_damage_position_back_by_weapon *pEffect = new CSkillEffect_p_critical_damage_position_back_by_weapon;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_MAGIC_SPEED_BY_WEAPON:
		{
			CSkillEffect_p_magic_speed_by_weapon* pEffect = new CSkillEffect_p_magic_speed_by_weapon;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BONUS_SP:
		{
			CSkillEffect_p_bonus_sp* pEffect = new CSkillEffect_p_bonus_sp;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BONUS_ITEM_DROP:
		{
			CSkillEffect_p_bonus_item_drop* pEffect = new CSkillEffect_p_bonus_item_drop;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BONUS_ADENA:
		{
			CSkillEffect_p_bonus_adena* pEffect = new CSkillEffect_p_bonus_adena;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BONUS_SPOIL:
		{
			CSkillEffect_p_bonus_spoil* pEffect = new CSkillEffect_p_bonus_spoil;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BONUS_HARVESTING:
		{
			CSkillEffect_p_bonus_harvesting* pEffect = new CSkillEffect_p_bonus_harvesting;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BONUS_RB:
		{
			CSkillEffect_p_bonus_rb* pEffect = new CSkillEffect_p_bonus_rb;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BONUS_EPIC_RB:
		{
			CSkillEffect_p_bonus_epic_rb* pEffect = new CSkillEffect_p_bonus_epic_rb;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_BONUS_EXP:
		{
			CSkillEffect_p_bonus_exp* pEffect = new CSkillEffect_p_bonus_exp;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_ATTACK_SPEED_BY_WEAPON2:
		{
			CSkillEffect_p_attack_speed_by_weapon2* pEffect = new CSkillEffect_p_attack_speed_by_weapon2;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case P_REGEN_CP:
		{
			CSkillEffect_p_regen_cp *pEffect = new CSkillEffect_p_regen_cp;
			nEffectID = 0;
			return pEffect;
		}
	default:
		{
			LPVOID pEffect = new char[nSize];
			memset(pEffect, 0, nSize);
			if(pEffect)
				*(DWORD*)pEffect = 0xADA4D8;
			return pEffect;
			break;
		}
	}
}
LPVOID SkillDataParser::CreateInstantEffect(int nSize)
{
	switch(nEffectID)
	{
	case I_RELEASE_SPIRIT:
		{
			CSkillEffect_i_release_soul *pEffect = new CSkillEffect_i_release_soul;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_UNHIDE_ABNORMAL:
		{
			CSkillEffect_i_unhide_abnormal *pEffect = new CSkillEffect_i_unhide_abnormal;
			nEffectID = 0;
			return pEffect;
			break;
		}
		
	case I_KILL_MYSELF:
		{
			CSkillEffect_i_kill_myself *pEffect = new CSkillEffect_i_kill_myself;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_RIDE_WYVERN:
		{
			CSkillEffect_i_ride_wyvern *pEffect = new CSkillEffect_i_ride_wyvern;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_CHARM_OF_COURAGE:
		{
			CSkillEffect_i_charm_of_courage *pEffect = new CSkillEffect_i_charm_of_courage;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_SET_NOBLESS:
		{
			CSkillEffect_i_set_nobless *pEffect = new CSkillEffect_i_set_nobless;
			nEffectID = 0;
			return pEffect;
		}
	case I_SHOW_MENU:
		{
			CSkillEffect_i_show_menu *pEffect = new CSkillEffect_i_show_menu;
			nEffectID = 0;
			return pEffect;
		}
	default:
		{
			LPVOID pEffect = new char[nSize];
			memset(pEffect, 0, nSize);
			if(pEffect)
				*(DWORD*)pEffect = 0xAD96F8;
			return pEffect;
			break;
		}
	}
}
LPVOID SkillDataParser::CreateInstantEffectOneArg(int nSize)
{
	switch(nEffectID)
	{
	case I_SUMMON_ERASE:
		{
			CSkillEffect_i_summon_erase *pEffect = new CSkillEffect_i_summon_erase;
		//	g_Log.Add(CLog::Blue, "[SkillDataParser] Creating i_summon_erase effect");
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_SUMMON_FRIEND:
		{
			CSkillEffect_i_summon_friend *pEffect = new CSkillEffect_i_summon_friend;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_SUMMON_PARTY:
		{
			CSkillEffect_i_summon_party *pEffect = new CSkillEffect_i_summon_party;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_TARGET_CANCEL_CHANCE:
		{
			CSkillEffect_i_target_cancel_chance *pEffect = new CSkillEffect_i_target_cancel_chance;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_TARGET_ME_CHANCE:
		{
			CSkillEffect_i_target_me_chance *pEffect = new CSkillEffect_i_target_me_chance;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_LIFT_DEATH_PENALTY:
		{
			CSkillEffect_i_lift_death_penalty *pEffect = new CSkillEffect_i_lift_death_penalty;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_TELEPORT_DISTANCE:
		{
			CSkillEffect_i_teleport_distance *pEffect = new CSkillEffect_i_teleport_distance;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_CONSUME_ENERGY:
		{
			CSkillEffect_i_consume_energy *pEffect = new CSkillEffect_i_consume_energy;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_DISPEL_NPC_BUFF:
		{
			CSkillEffect_i_dispel_npc_buff *pEffect = new CSkillEffect_i_dispel_npc_buff;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_DISPEL_DEBUFF_WITH_EXCEPTION:
		{
			CSkillEffect_i_dispel_debuff_with_exception *pEffect = new CSkillEffect_i_dispel_debuff_with_exception;
			nEffectID = 0;
			return pEffect;
		}
	case I_DIG:
		{
			CSkillEffect_i_dig* pEffect = new CSkillEffect_i_dig;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_USE_SKILL:
		{
			CSkillEffect_i_use_skill* pEffect = new CSkillEffect_i_use_skill;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_ADDITIONAL_BOX:
		{
			CSkillEffect_i_additional_box* pEffect = new CSkillEffect_i_additional_box;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_PLEDGE_NAME_VALUE:
		{
			CSkillEffect_i_pledge_name_value* pEffect = new CSkillEffect_i_pledge_name_value;
			nEffectID = 0;
			return pEffect;
		}
		
	case I_UPDATE_PLEDGE_SKILL:
		{
			CSkillEffect_i_update_pledge_skill* pEffect = new CSkillEffect_i_update_pledge_skill;
			nEffectID = 0;
			return pEffect;
		}
	case I_UPDATE_EVAL_SCORE:
		{
			CSkillEffect_i_update_eval_score *pEffect = new CSkillEffect_i_update_eval_score();
			nEffectID = 0;
			return pEffect;
		}
	case I_SET_AIO:
		{
			CSkillEffect_i_set_aio *pEffect = new CSkillEffect_i_set_aio;
			nEffectID = 0;
			return pEffect;
		}
	default:
		{
			LPVOID pEffect = new char[nSize];
			memset(pEffect, 0, nSize);
			if(pEffect)
				*(DWORD*)pEffect = 0xAD7BF8;
			return pEffect;
			break;
		}
	}
}
LPVOID SkillDataParser::CreateInstantEffectTwoArg(int nSize)
{
	switch(nEffectID)
	{
	case I_DEATH2:
		{
			CSkillEffect_i_death2 *pEffect = new CSkillEffect_i_death2;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_CHARGE:
		{
			CSkillEffect_i_charge *pEffect = new CSkillEffect_i_charge;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_ADD_ENCHANTED_ITEM:
		{
			CSkillEffect_i_add_enchanted_item *pEffect = new CSkillEffect_i_add_enchanted_item;
			nEffectID = 0;
			return pEffect;
		}
	case I_MANA_BURN:
		{
			CSkillEffect_i_mana_burn *pEffect = new CSkillEffect_i_mana_burn;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_SPAWN_CASTING_NPC:
		{
			CSkillEffect_i_spawn_casting_npc *pEffect = new CSkillEffect_i_spawn_casting_npc;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_DISPEL_BY_CATEGORY_COUNT:
		{
			CSkillEffect_i_dispel_by_category_count *pEffect = new CSkillEffect_i_dispel_by_category_count;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_DISPEL_BY_CHANCE:
		{
			CSkillEffect_i_dispel_by_chance *pEffect = new CSkillEffect_i_dispel_by_chance;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_HIDE_ABNORMAL:
		{
			CSkillEffect_i_hide_abnormal *pEffect = new CSkillEffect_i_hide_abnormal;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_DISPEL_BUFF:
		{
			CSkillEffect_i_dispel_buff *pEffect = new CSkillEffect_i_dispel_buff;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_GIVE_ITEM:
		{
			CSkillEffect_i_give_item *pEffect = new CSkillEffect_i_give_item;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_ACQUIRE_SKILL:
		{
			CSkillEffect_i_acquire_skill *pEffect = new CSkillEffect_i_acquire_skill;
			nEffectID = 0;
			return pEffect;
			break;
		}
	case I_DISPEL_BUFF_FIXED:
		{
			CSkillEffect_i_dispel_buff_fixed *pEffect = new CSkillEffect_i_dispel_buff_fixed;
			nEffectID = 0;
			return pEffect;
			break;
		}
	default:
		{
			LPVOID pEffect = new char[nSize];
			memset(pEffect, 0, nSize);
			if(pEffect)
				*(DWORD*)pEffect = 0xAD7C58;
			return pEffect;
			break;
		}
	}
}
LPVOID SkillDataParser::CreateOperateConditionOneVar(int nSize)
{
	switch(nEffectID)
	{
	case OP_BATTLE_FORCE_COUNT:
		{
			CSkillOperateCondition_op_battle_force_count *pCond = new CSkillOperateCondition_op_battle_force_count;
			nEffectID = 0;
			return pCond;
			break;
		}	
	case OP_SPELL_FORCE_COUNT:
		{
			CSkillOperateCondition_op_spell_force_count *pCond = new CSkillOperateCondition_op_spell_force_count;
			nEffectID = 0;
			return pCond;
			break;
		}	
	case OP_REMAIN_MP_PER:
		{
			CSkillOperateCondition_op_remain_mp_per *pCond = new CSkillOperateCondition_op_remain_mp_per;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_MAX_HEIGHT_DIFF:
		{
			CSkillOperateCondition_op_max_height_diff *pCond = new CSkillOperateCondition_op_max_height_diff;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_NOT_CLASS:
		{
			CSkillOperateCondition_op_not_class *pCond = new CSkillOperateCondition_op_not_class;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_NOT_IN_TERRITORY:
		{
			CSkillOperateCondition_op_not_in_territory *pCond = new CSkillOperateCondition_op_not_in_territory;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_IS_TYPE:
		{
			CSkillOperateCondition_op_is_type *pCond = new CSkillOperateCondition_op_is_type;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_STRIFE_NOT_OWNED_BY:
		{
			CSkillOperateCondition_op_strife_not_owned_by *pCond = new CSkillOperateCondition_op_strife_not_owned_by;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_COLLECTED_SOUL_COUNT:
		{
			CSkillOperateCondition_op_collected_soul_count *pCond = new CSkillOperateCondition_op_collected_soul_count;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_IS_IN_SIEGE:
		{
			CSkillOperateCondition_op_is_in_siege* pCond = new CSkillOperateCondition_op_is_in_siege;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_HOUR_HIGHER_THAN:
		{
			CSkillOperateCondition_op_hour_higher_than* pCond = new CSkillOperateCondition_op_hour_higher_than;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_HOUR_LOWER_THAN:
		{
			CSkillOperateCondition_op_hour_lower_than* pCond = new CSkillOperateCondition_op_hour_lower_than;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_CAN_DIG:
		{
			CSkillOperateCondition_op_can_dig_at* pCond = new CSkillOperateCondition_op_can_dig_at;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_CAN_RELEASE_SPIRIT:
		{
			CSkillOperateCondition_op_can_release_spirit* pCond = new CSkillOperateCondition_op_can_release_spirit;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_CAN_ACQUIRE_SKILL:
		{
			CSkillOperateCondition_op_can_acquire_skill* pCond = new CSkillOperateCondition_op_can_acquire_skill;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_CAN_GET_ITEM:
		{
			CSkillOperateCondition_op_can_get_item* pCond = new CSkillOperateCondition_op_can_get_item;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_COMBAT_MODE:
		{
			CSkillOperateCondition_op_combat_mode* pCond = new CSkillOperateCondition_op_combat_mode;
			nEffectID = 0;
			return pCond;
			break;
		}
	case OP_CAN_UPDATE_PLEDGE_SKILL:
		{
			CSkillOperateCondition_op_can_update_pledge_skill *pCond = new CSkillOperateCondition_op_can_update_pledge_skill;
			nEffectID = 0;
			return pCond;
		}
	case OP_IN_PEACEZONE:
		{
			CSkillOperateCondition_op_peace_zone * pCond = new CSkillOperateCondition_op_peace_zone();
			nEffectID = 0;
			return pCond;
		}
	default:
		{
			LPVOID pEffect = new char[nSize];
			memset(pEffect, 0, nSize);
			if(pEffect)
			{
				(*(DWORD*)pEffect) = 0xADC780;
			}
			return pEffect;
			break;
		}
	}
}
PWCHAR SkillDataParser::GetName(LPVOID param)
{
	typedef PWCHAR (*_getC)(LPVOID);
	_getC getC = (_getC)0x0091CED0;
	PWCHAR wszName = getC(param);
	wstring wStr = wszName;
	
	if(wStr == L"A1" || wStr == L"A2" || wStr == L"A3" || wStr == L"A4" || wStr == L"P" || wStr == L"PP" || wStr == L"T")
	{
		nSkillCount++;
	}

//	g_Log.Add(CLog::Blue, "[%s] word [%S]", __FUNCTION__, wStr.c_str());

	map<wstring, int>::iterator Iter;
	Iter = m_SkillEffect.find(wStr);
	if(Iter != m_SkillEffect.end())
	{
		nEffectID = Iter->second;
		bChangeID = true;
		return wszName;
	}
	Iter = m_OperateCond.find(wStr);
	if(Iter != m_OperateCond.end())
	{
		nEffectID = Iter->second;
		bChangeID = true;
		return wszName;
	}
	Iter = m_IntParam.find(wStr);
	if(Iter != m_IntParam.end())
	{
		nEffectID = Iter->second;
		bChangeID = true;
		return wszName;
	}
//	g_Log.Add(CLog::Blue, "%ls", wszName);
	return wszName;
}
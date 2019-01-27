#pragma once
#include "SkillOperateCondition.h"

class CSkillOperateCondition_op_strife_not_owned_by: public CSkillOperateCondition
{
	int nOwner;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_spell_force_count: public CSkillOperateCondition
{
	int nCount;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_remain_mp_per: public CSkillOperateCondition
{
	int nPer;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_not_in_territory: public CSkillOperateCondition
{
	int nTerritoryType;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_not_class: public CSkillOperateCondition
{
	int nClassID;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_max_height_diff: public CSkillOperateCondition
{
	int nDiff;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_is_type: public CSkillOperateCondition
{
	int nType;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_is_in_siege: public CSkillOperateCondition
{
	int nResidenceID;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_hour_lower_than: public CSkillOperateCondition
{
	int hour;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_hour_higher_than: public CSkillOperateCondition
{
	int hour;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_collected_soul_count: public CSkillOperateCondition
{
	int nCount;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_battle_force_count: public CSkillOperateCondition
{
	int nCount;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_can_dig_at: public CSkillOperateCondition
{
	int type; //0 - none, 1- day, 2- night
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_can_release_spirit: public CSkillOperateCondition
{
	int count; //spirit count
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_can_acquire_skill: public CSkillOperateCondition
{
	int skillpch; //spirit count
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_can_get_item: public CSkillOperateCondition
{
	int amount; //spirit count
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_combat_mode: public CSkillOperateCondition
{
	UINT m_Type;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_can_update_pledge_skill: public CSkillOperateCondition
{
	UINT m_SkillId;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};

class CSkillOperateCondition_op_peace_zone: public CSkillOperateCondition
{
	BOOL m_InPeaceZone;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};
#pragma once
#include "SkillEffect.h"



class CSkillEffect_i_add_enchanted_item : public CSkillEffect
{
	double m_ItemId;
	double m_EnchantLevel;
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_charge : public CSkillEffect
{
	double dType;
	double dRange;
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_update_eval_score : public CSkillEffect
{
	double m_Amount; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_consume_energy : public CSkillEffect
{
	double dCount; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_pledge_name_value : public CSkillEffect
{
	double m_Amount; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_set_aio : public CSkillEffect
{
	double m_Id; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_update_pledge_skill : public CSkillEffect
{
	double m_SkillId; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_show_menu : public CSkillEffect
{
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_set_nobless : public CSkillEffect
{
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_ride_wyvern : public CSkillEffect
{
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_death2 : public CSkillEffect
{
	double effect1;
	double effect2;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_dig : public CSkillEffect
{
	double baseChance;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};


class CSkillEffect_i_dispel_debuff_with_exception : public CSkillEffect
{
	double m_Amount; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pSpeller, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_dispel_npc_buff : public CSkillEffect
{
	double dCount; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_dispel_by_category_count : public CSkillEffect
{
	double dType; // 0x10
	double dCount;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pSpeller, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_dispel_by_chance : public CSkillEffect
{
	double dChance; // 0x10
	double dCount;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pSpeller, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_hide_abnormal : public CSkillEffect
{
	double dType;
	double dCount;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_kill_myself : public CSkillEffect
{
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_lift_death_penalty : public CSkillEffect
{
	double dChance; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_mana_burn : public CSkillEffect
{
	double power; // 0x10
	double chance;
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_release_soul : public CSkillEffect
{
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_spawn_casting_npc : public CSkillEffect
{
	double dNpcID; // 0x10
	double dSkillID;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_summon_erase : public CSkillEffect
{
	double dChance; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_summon_friend : public CSkillEffect
{
	double dItemID; // 0x10 item id consumed
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_summon_party : public CSkillEffect
{
	double dItemID;
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_target_cancel_chance : public CSkillEffect
{
public:
	double dChance; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_target_me_chance : public CSkillEffect
{
	double dChance; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_teleport_distance : public CSkillEffect
{
	double dDistance; // 0x10 item id consumed
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_unhide_abnormal : public CSkillEffect
{
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_use_skill : public CSkillEffect
{
	double skillClassId; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_dispel_buff : public CSkillEffect
{
	double baseChance; // 0x10
	double maxCount;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_give_item : public CSkillEffect
{
	double itemId; // 0x10
	double count;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_acquire_skill : public CSkillEffect
{
	double skillId;
	double level;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_charm_of_courage : public CSkillEffect
{
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_additional_box : public CSkillEffect
{
	double duration; // 0x10
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_i_dispel_buff_fixed : public CSkillEffect
{
	double buffChance; // 0x10
	double maxCount;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget){ return false;};
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo){};
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

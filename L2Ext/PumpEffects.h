#pragma once
#include "SkillEffect.h"

class CSkillEffect_p_summon_betray : public CSkillEffect
{
	double dTick;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction);
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_strife_cp : public CSkillEffect
{
	double Bonus;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};


class CSkillEffect_p_resist_stun : public CSkillEffect
{
	double value;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_self_resurrection : public CSkillEffect
{
	bool baWeaponType[16]; //0x10h 
	double RecoveryExp; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature *pCreature);
	virtual void Pump(CCreature *pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_reflect_melee_skill_dmg : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double Chance; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature *pCreature);
	virtual void Pump(CCreature *pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_reduce_exp_penalty : public CSkillEffect
{
	double dPercent;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_physical_attack_by_hp2_weapon : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double PAtkAmount; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; };
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_mul_skill_dmg : public CSkillEffect
{
	DWORD nClassID;
	DWORD nValue;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_magic_speed_by_weapon : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double Speed; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_improve_heal : public CSkillEffect
{
	double dStat;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_enlarge_buff_slots : public CSkillEffect
{
	double dSlots;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_digger_chance : public CSkillEffect
{
	double baseChance;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_critical_rate_by_weapon : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double CriticalAmount; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_critical_rate_by_hp2_weapon : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double CriticalAmount; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_critical_rate_by_hp1_weapon : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double CriticalAmount; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_critical_damage_position_front_by_weapon : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double CriticalAmount; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_critical_damage_position_side_by_weapon : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double CriticalAmount; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_critical_damage_position_back_by_weapon : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double CriticalAmount; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_critical_damage_by_weapon : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double DamageAmount; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_cancel_attacker_target : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double Chance; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature *pCreature);
	virtual void Pump(CCreature *pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_block_icon_action : public CSkillEffect
{
	double dActionID;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_avoid_melee_skill : public CSkillEffect
{
	bool baWeaponType[11]; //0x10h 
	unsigned char ucaUnkn[5];
	double Chance; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature *pCreature);
	virtual void Pump(CCreature *pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_autocast_when_attacked : public CSkillEffect
{
	DWORD nClassID;
	DWORD nChance;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_autocast_on_patk : public CSkillEffect
{
	DWORD nClassID;
	DWORD nChance;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_autocast_on_matk : public CSkillEffect
{
	DWORD nClassID;
	DWORD nChance;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_autocast_on_crit : public CSkillEffect
{
	DWORD nClassID;
	DWORD nChance;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_anti_buff : public CSkillEffect
{
	double dSelf;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_defence_attribute_by_armor : public CSkillEffect
{
	double value;	//10
	int attribute; //18h
	bool armorType[4]; //1Ch
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_speed_by_weapon : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};


class CSkillEffect_p_attack_speed_by_weapon2 : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_physical_defence_by_weapon : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_magical_defence_by_weapon : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_avoid_by_weapon : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_semi_invisible : public CSkillEffect
{
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSI);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_bonus_exp : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_bonus_sp : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_bonus_item_drop : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_bonus_adena : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_bonus_spoil : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_bonus_harvesting : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_bonus_rb : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_bonus_epic_rb : public CSkillEffect
{
	bool weaponType[16]; //0x10h
	double value; //0x20h
	int ChangeType; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false;};
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pTarget);
	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};

class CSkillEffect_p_bonus_item_id : public CSkillEffect
{
	DWORD nClassID;
	DWORD nValue;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};


class CSkillEffect_p_anti_pk : public CSkillEffect
{
	DWORD nMaxPlayerLevel;
	DWORD nLevelRange;
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};


class CSkillEffect_p_regen_cp : public CSkillEffect
{
	bool m_WeaponType[16]; //0x10h 
	double m_Amount; //0x20h
	UINT m_Type; // 0x28h per- 0 or diff-1
public:
	virtual bool CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction) { return false; };
	virtual void Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction* pAction, double distance) {};
	virtual void Unk() {};
	virtual void Tick() {};
	virtual double Unk2() { return 0.0;};
	virtual bool CheckPumpCondition(CCreature* pCreature);
	virtual void Pump(CCreature* pCreature, CSkillInfo* pSkillInfo);
	virtual bool Consume(CCreature *pCreature, double param2) { return false; }; 
	virtual bool CheckResisted()
	{ 
		typedef bool (*f)(CSkillEffect*);
		return f(0x7985D0L)(this);
	};
};
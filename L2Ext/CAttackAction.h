#pragma once

class AttackTargetDataType
{
public:
	INT64 nUnkn000;
	DWORD dTargetSID;
	BYTE bUnkn00C;
	BYTE bCritical;
	BYTE bUnkn00E;
	BYTE bUnkn00F;
	INT64 nUnkn010;
};

class CAttackAction
{
	/* 00 */ LPVOID v_Table;
	/* 08 */ UINT attackerId;
	/* 0C */ int nUnkn00C;
	/* 10 */ UINT targetId;
	/* 14 */ UINT _unkn14[3];
	/* 20 */ UINT _unkn20[4];
	/* 30 */ UINT _unkn30;
	/* 3C */ UINT weaponType;

//	/* 70 */ int multi_count;
//	/* 74 */ int unkn_74h;
//	/* 78 */ AttackTargetDataType arr_multi_target; //size 480
public:
	void CheckItemSkill(AttackTargetDataType *pData);
	CCreature* GetAttacker();
	CObject* GetTarget();
	inline UINT GetWeaponType() { return weaponType; };
	inline void SetTarget(UINT id) { targetId = id; };
};
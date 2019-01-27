#pragma once

class CSkillAction
{
	/* 8h */ UINT casterId;
	/* 0Ch */ int unkn_0Ch;
	/* 10h */ UINT targetId;
	/* 14h */ int skillId;
	/* 18h */ int skillLevel;
	/* 1Ch */ UINT isMagic; //0-physical 1-magical 2-other
	/* 20h */ bool force;
	/* 21h */ bool hasTarget;
	/* 22h */ char unkn22h[6];
	/* 28h */ double mpConsumeMultipler;	//for song/ dances
	/* 30h */ FVector pos;
	/* 48h */ char unkn48h[32];
	/* 68h */ CSkillInfo *pSI;
	/* 70h */ UINT state; //0-HIT CANCEL TIME 1-SKILL HIT TIME 2- end
	/* 74h */ UINT timerVal1; //hit cancel time
	/* 78h */ UINT timerVal2; //skill hit time
	/* 7Ch */ UINT timerVal3; //3rd time cool time
public:
	inline INT32 GetSkillId() { return skillId; };
	inline INT32 GetSkillLevel() { return skillLevel; };
	inline UINT GetIsMagic() { return isMagic; };
	inline UINT GetState() { return state; };
	inline UINT GetTimerVal1() { return timerVal1; };
	inline UINT GetTimerVal2() { return timerVal2; };
	inline UINT GetTimerVal3() { return timerVal3; };
	inline double GetMpConsumeMultipler() { return mpConsumeMultipler; };
	inline void SetMpConsumeMultipler(double value) { mpConsumeMultipler = value; };
	inline const CSkillInfo* GetSkillInfo() { return pSI; };
	CCreature* GetCaster()
	{
		typedef CCreature*(*f)(PUINT);
		return f(0x07F07F0L)(&casterId);
	}

	CObject* GetTarget()
	{
		if(targetId)
		{
			typedef CObject*(*f)(PUINT);
			return f(0x07F07F0L)(&targetId);
		}
		return 0;
	}
	virtual bool FalseSub() { return false; };
	virtual bool OnStart()
	{
		typedef bool (*f)(CSkillAction*);
		return f(0x6B28F0L)(this);
	}
	virtual bool OnTick()
	{
		typedef bool (*f)(CSkillAction*);
		return f(0x6B70F0L)(this);
	}
	virtual bool OnLostTarget(int id, bool ByUser)
	{
		typedef bool (*f)(CSkillAction*, int, bool);
		return f(0x6B06F0L)(this, id, ByUser);
	}
	virtual bool OnDisappear()
	{
		typedef bool (*f)(CSkillAction*);
		return f(0x6B0940L)(this);
	}
	virtual bool TrueSub() { return true; };
	virtual bool OnDamaged(LPVOID spAttacker, int nDamage)
	{
		typedef bool (*f)(CSkillAction*, LPVOID, int);
		return f(0x6B0AA0L)(this, spAttacker, nDamage);
	}
	virtual bool OnEnd(bool bFinishedWell)
	{
		typedef bool (*f)(CSkillAction*, bool);
		return f(0x6B1830L)(this, bFinishedWell);
	}
	virtual bool TrueSub2() { return true; };
};
#include "StdAfx.h"
#include "CCreature.h"
#include "SkillOperateCondition.h"
#include "CreatureAction.h"

bool CCreature::ValidCreature()
{
	if(this)
	{
		try
		{
			DWORD dwVTable = (*(LPDWORD)this);
			switch(dwVTable)
			{
			case VT_CBOSS:
			case VT_CMERCHANT:
			case VT_CCREATURE:
			case VT_CNPC:
			case VT_CPET:
			case VT_CSUMMON:
			case VT_CZZOLDAGU:
			case VT_USER:
				{
					if(this->pSD)
						return true;
					else
						return false;
					break;
				}
			default:
				{
					return false;
					break;
				}
			}
		}
		catch(...)
		{
			return false;
		}
	}
	return false;
}

bool CCreature::ValidUser()
{
	if(this)
	{
		return IsValid(VT_USER);
	}
	return false;
}
bool CCreature::IsNPC()
{
	if(this)
	{
		return IsValid(VT_CNPC);
	}
	return false;
}
bool CCreature::IsSummon()
{
	if(this)
	{
		return IsValid(VT_CSUMMON);
	}
	return false;
}
bool CCreature::IsPet()
{
	if(this)
	{
		return IsValid(VT_CPET);
	}
	return false;
}
bool CCreature::IsBoss()
{ 
	if(this)
		return IsValid(VT_CBOSS); 
	else
		return false;
}
bool CCreature::IsZZoldagu()
{ 
	if(this)
		return IsValid(VT_CZZOLDAGU); 
	else
		return false;
}

bool CCreature::IsMob()
{
	if(this)
	{
		if(IsValid(VT_CNPC) || IsValid(VT_CBOSS) || IsValid(VT_CZZOLDAGU))
		{
			return true;
		}
	}
	return false;
}

CInventory * CCreature::GetInventory()
{
	if(this)
	{
		return &inventory;
	}else
		return NULL;
}

void CCreature::GetBuffDebuffAmount(UINT& buffAmount, UINT& debuffAmount, UINT& selfBuffAmount)
{
	buffAmount = debuffAmount = 0;

	for(UINT n=0;n<vAbnormalStatus.size();n++)
	{
		AbnormalData& ad = vAbnormalStatus[n];
		if(ad.pSI)
		{
			if(ad.pSI->nDebuff == 0 || ad.pSI->nDebuff == 2)
			{
				if(ad.pSI->IsSelfBuff())
				{
					selfBuffAmount++;
				}else
				{
					buffAmount++;
				}
			}else if(ad.pSI->nDebuff == 1)
			{
				debuffAmount++;
			}
		}
	}
}

void CCreature::GetBuffDebuffAmount(UINT& buffAmount, UINT& debuffAmount)
{
	buffAmount = debuffAmount = 0;

	for(UINT n=0;n<vAbnormalStatus.size();n++)
	{
		AbnormalData& ad = vAbnormalStatus[n];
		if(ad.pSI)
		{
			if(ad.pSI->nDebuff == 0 || ad.pSI->nDebuff == 2)
			{
				buffAmount++;
			}else if(ad.pSI->nDebuff == 1)
			{
				debuffAmount++;
			}
		}
	}
}

UINT CCreature::GetBuffAmount()
{
	UINT nBuff = 0;
	for(UINT n=0;n<vAbnormalStatus.size();n++)
	{
		AbnormalData& ad = vAbnormalStatus[n];
		if(ad.pSI)
		{
			if(ad.pSI->nDebuff == 0 || ad.pSI->nDebuff == 2)
			{
				nBuff++;
			}
		}
	}
	return nBuff;
}

double CCreature::GetAttribute(UINT attributeType)
{
	typedef double(*f)(CCreature*, UINT);
	return f(0x4D8F10L)(this, attributeType);
}

double CCreature::GetAttributeBonus(CCreature *pOpponent)
{
	typedef double (*f)(CCreature*, CCreature*);
	return f(0x4E01F0L)(this, pOpponent);
}

int CCreature::GetDebuffAmount()
{
	int nDeBuff = 0;
	AbnormalDataVector::iterator Iter;
	for(Iter = vAbnormalStatus.begin(); Iter!=vAbnormalStatus.end(); Iter++)
	{
		CSkillInfo *pSI = Iter->pSI;
		if(pSI)
		{
			if(pSI->nDebuff == 1)
			{
				nDeBuff++;
			}
		}
	}
	return nDeBuff;
}

UINT CCreature::GetAbnormalAmount()
{
	return static_cast<UINT>(vAbnormalStatus.size());
}

double CCreature::Distance(CCreature *pCreature)
{
	if(pCreature->ValidCreature() && this->ValidCreature())
	{
		return sqrt(pow((this->pSD->Pos.x - pCreature->pSD->Pos.x),2)+pow((this->pSD->Pos.y - pCreature->pSD->Pos.y),2));
	}else
		return 0;
}
int CCreature::GetAcquiredSkillLevel(int nSkillID)
{
	if(!this->ValidCreature()) return -1;
	typedef int (__fastcall *__GASL)(CCreature*, int);
	__GASL _GASL = (__GASL) 0x004F05E0;
	return _GASL(this, nSkillID);
}
bool CCreature::HaveSkill(int nSkillID, int nLevel)
{
	typedef bool (* _f)(CCreature*, int, int);
	_f f = (_f) 0x004F0830;
	return f(this, nSkillID, nLevel);
}

void CCreature::ValidateHP()
{
	typedef void (__fastcall * __vhp)(CCreature *pCreature);
	__vhp _vhp = (__vhp) 0x004DB600;
	_vhp(this);
}

void CCreature::ValidateMP()
{
	typedef void (__fastcall * __vmp)(CCreature *pCreature);
	__vmp _vmp = (__vmp) 0x004DB850;
	_vmp(this);
}

void CCreature::ValidateParameters()
{
	typedef void (*f) (CCreature*);
	f(0x517ED0L)(this);
}

void CCreature::BroadcastFlyToLocation(int x, int y, int z)
{
	typedef void (*_fly)(CCreature*, int, int, int);
	_fly fly = (_fly) 0x004EAFF0;
	if(this)
		fly(this, x, y, z);
}

void CCreature::CheckAbnormalVisualEffect()
{
	typedef void (__fastcall *__DABGM)(CCreature*);
	__DABGM _DABGM = (__DABGM) 0x004E52B0;
	_DABGM(this);
}

void CCreature::Action(CCreature *pCreature, DWORD dOption1, DWORD dOption2)
{
	typedef void(__fastcall *__ct)(CCreature*, CCreature *, DWORD, DWORD);
	__ct _ct = (__ct) 0x004E46C0;
	if(this->ValidCreature())
	{
		if(pCreature->ValidCreature())
		{
			_ct(this, pCreature, dOption1, dOption2);
		}
	}
}

void CCreature::DeleteSkill(int nSkillID)
{
	typedef void (__fastcall * DeleteSkillS) (CCreature*, int nSkillID);
	DeleteSkillS _DeleteSkill = (DeleteSkillS) 0x004F8A70;
	_DeleteSkill(this, nSkillID);
}

void CCreature::AcquireSkill(int nSkillID, int nLevel, int Replace, int UseLock)
{
	typedef void (*f)(CCreature*, int, int, int, int); //Creature, SkillID, SkillLevel, Overwrite, UseLock
	f(0x4F8760)(this, nSkillID, nLevel, Replace, UseLock);
}

void CCreature::AttachSkill(int ID, int Level, int OperateType)
{
	typedef void (*f)(CCreature*, int, int, int);
	f(0x500000)(this, ID, Level, OperateType);
}

bool CCreature::ValidateBaseCP()
{
	typedef bool (__fastcall * ValidateBCPS) (CCreature*);
	ValidateBCPS _ValidateBCPS = (ValidateBCPS) 0x004DBA50;
	return _ValidateBCPS(this);
}
bool CCreature::ValidateBaseData()
{
	typedef bool (*_vbd)(CCreature*);
	_vbd vbd = (_vbd) 0x004DACC0;
	return vbd(this);
}
bool CCreature::ValidateAllOnWeapon()
{
	typedef bool (__fastcall * __ValidateAllOnWeapon) (CCreature*);
	__ValidateAllOnWeapon _ValidateAllOnWeapon = (__ValidateAllOnWeapon) 0x00516FE0;
	return _ValidateAllOnWeapon(this);
}
bool CCreature::ValidateSkillMod()
{
	typedef bool (*_vsm) (CCreature*);
	_vsm vsm = (_vsm) 0x004FEA80;
	return vsm(this);
}
void CCreature::ValidateLocation()
{
	typedef void(*f)(CCreature*);
	f(0x4F00D0L)(this);
}

void CCreature::InitializeSkillMod()
{
	guard;

	typedef void (*_ism) (CCreature*);
	_ism ism = (_ism) 0x004DDC70;
	
	ism(this);

	unguard;
}

void CCreature::AddTimer(int timeout, int id)
{
	typedef void(*f)(CCreature*, int, int);
	f(0x6261D0L)(this, timeout, id);
}

void CCreature::AddContribution(CCreature *pEnemy, double value)
{
	typedef void(*f)(CCreature*, CCreature*, double);
	f(0x4F8510L)(this, pEnemy, value);
}

bool CCreature::ValidateAllOnSIM()
{
	typedef bool (__fastcall * __ValidateAll) (CCreature*);
	__ValidateAll _ValidateAll = (__ValidateAll) 0x004FFDC0;
	return _ValidateAll(this);
}
bool CCreature::InfectedSkillBy(CCreature *pBuffer, CSkillInfo *pSkillInfo)
{
	typedef bool (*f)(CCreature*, CCreature*, CSkillInfo*);
	return f(0x504FD0L)(this, pBuffer, pSkillInfo);
}

void CCreature::WriteLock(PWCHAR wszFile, int nLine)
{
	typedef void(__fastcall *__ct)(CCreature*, PWCHAR, int);
	__ct _ct = (__ct) 0x004292F0;
	_ct(this, wszFile, nLine);
}

void CCreature::WriteUnlock()
{
	typedef void(__fastcall *__ct)(CCreature*);
	__ct _ct = (__ct) 0x00429F10;
	_ct(this);
}

bool CCreature::DeleteItemInInventory(int nItemID, int nAmount)
{
	typedef bool (*f)(CCreature*, int, int);
	return f(0x004EF8C0L)(this, nItemID, nAmount);
}

void CCreature::AddItemToInventory(int itemId, int amount, bool isPick, int enchanted, int blessed, int eroded)
{
	typedef void (__fastcall * f)(CCreature*, int, int, bool, int, int, int);
	f(0x004EF130L)(this, itemId, amount, isPick, enchanted, blessed, eroded);
}

void CCreature::ValidateSkillList()
{
	typedef void (*f) (CCreature*);
	f(0x509B80L)(this);
}
bool CCreature::ValidateSetItem(int nSlotType)
{
	typedef bool(*f)(CCreature*, int);
	return f(0x516940L)(this, nSlotType);
}

bool CCreature::Die(CCreature *pEnemy)
{
	if(this)
	{
		UINT64 addr = (*(PUINT64)this);	//vt
		addr += 0x5A8;
		addr = (*(PUINT64)addr);
		typedef bool (*f)(CCreature*, CCreature*);
		return f(reinterpret_cast<LPVOID>(addr))(this, pEnemy);
	}
	return false;
}

void CCreature::SetMagicLevel()
{
	typedef void (*f)(CCreature*);
	f(0x004EACB0L)(this);
}
bool CCreature::IsShieldDefend(CCreature *pAttacker, double dCriticalBonus)
{
	typedef bool (*_defend)(CCreature*, CCreature*, double);
	_defend defend = (_defend) 0x004DFD80;
	return defend(this, pAttacker, dCriticalBonus);
}

void CCreature::DispelAllByGM()
{
	typedef void (*f)(CCreature*);
	f(0x00502A00L)(this);
}

void CCreature::UpdateStatus(int nType)
{
	typedef void (*US)(CCreature*, int);
	US us = (US) 0x004F0CF0;
	us(this, nType);
}
CSharedItemData *CCreature::GetEquipedWeapon()
{
	typedef CSharedItemData*(*gew)(CCreature*);
	gew _gew = (gew) 0x004E4560;
	return _gew(this);
}

bool CCreature::IsInRange(CCreature *pTarget, int range)
{
	if(this && pTarget)
	{
		double x1 = this->pSD->Pos.x;
		double y1 = this->pSD->Pos.y;
		double z1 = this->pSD->Pos.z;
		double x2 = pTarget->pSD->Pos.x;
		double y2 = pTarget->pSD->Pos.y;
		double z2 = pTarget->pSD->Pos.z;

		int dist = static_cast<int>(sqrt(pow((x2-x1), 2) + pow((y2-y1), 2) + pow((z2-z1), 2)));
		if(dist < range)
		{
			return true;
		}

	}
	return false;
}

bool CCreature::CheckDistance(CCreature *pTarget, int Distance)
{
	if(this->ValidCreature() && pTarget->ValidCreature())
	{
		try 
		{
			double x1 = this->pSD->Pos.x;
			double y1 = this->pSD->Pos.y;
			double x2 = pTarget->pSD->Pos.x;
			double y2 = pTarget->pSD->Pos.y;
			int dist = (int)sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
			if(dist < Distance)
				return true;
			else
				return false;
		}
		catch(...)
		{
			return false;
		}
	}else
		return false;
}

bool CCreature::IsValidTalkTarget()
{
	if(this)
	{
		UINT64 nAddr = (UINT64)this;
		nAddr += 0xE0;
		nAddr = (*(UINT64*)nAddr);
		typedef bool (*f)(CCreature*);
		return f(nAddr)(this);
	}
	return false;
}
CCreature *CCreature::GetTarget()
{
	if(this)
	{
		return GetCreatureBySID(&targetId);
	}
	return NULL;
}
CCreature *CCreature::GetCreatureBySID(UINT *pSID)
{
	typedef CCreature* (*f)(UINT*);
	return f(0x007F0410L)(pSID);
}
bool CCreature::IsValidServerId(PUINT pServerId)
{
	typedef bool (*f)(LPUINT);
	return f(0x7F06D0L)(pServerId);
}

CCreature *CCreature::GetTalkTarget(PUINT pServerId)
{
	if(pServerId && (*pServerId))
	{
		typedef CCreature*(*f)(LPUINT);
		if(CCreature *pCreature = f(0x7F07F0L)(pServerId))
		{
			UINT64 nAddr = (UINT64)pCreature;
			nAddr = (*(UINT64*)nAddr);
			nAddr += 0xE0;
			nAddr = (*(UINT64*)nAddr);
			typedef bool (*f2)(CCreature*);
			if(f2(nAddr)(pCreature))	//is npc
			{
				nAddr = (UINT64)pCreature;
				nAddr = (*(UINT64*)nAddr);
				nAddr += 0x120;
				nAddr = (*(UINT64*)nAddr);
				typedef CCreature* (*f3)(CCreature*);
				pCreature = f3(nAddr)(pCreature);
				if(pCreature && pCreature->pSD)
				{
					return pCreature;
				}
			}
		}
	}

	return 0;
}

void CCreature::SetSkillUsableTime(int nSkillID, int nLevel, DWORD *stamp, DWORD ReuseDelay)
{
	typedef void (*_ssut)(CCreature*, int, int, DWORD*, DWORD);
	_ssut ssut = (_ssut) 0x004F8D10;
	ssut(this, nSkillID, nLevel, stamp, ReuseDelay);
}
DWORD* CCreature::GetSkillUsedTime(int nSkillID, int nLevel)
{
	typedef DWORD* (*_f)(CCreature*, int, int);
	_f f = (_f) 0x004F1D40;
	return f(this, nSkillID, nLevel);
}
DWORD* CCreature::GetItemUsedTime(int itemClassID)
{
	typedef DWORD* (*_f)(CCreature*, int);
	_f f = (_f) 0x004F1F90;
	return f(this, itemClassID);
}

void CCreature::SetItemUsableTime(int itemClassID, DWORD* stamp, DWORD reusedelay)
{
	typedef void (*_f)(CCreature*, int, DWORD*, DWORD);
	_f f = (_f) 0x004F90E0;
	f(this, itemClassID, stamp, reusedelay);
}

void CCreature::SkillAction(int nSkillID, int nControl, bool bShift)
{
	typedef void (*_csa)(CCreature*, int, int, bool, INT64);
	_csa csa = (_csa) 0x004F1070;
	csa(this, nSkillID, nControl, bShift, 0);
}

void CCreature::BuffCreature(int nSkillClassID)
{
	if(ValidCreature())
	{
		int nSkillID = nSkillClassID >> 8;
		int nLevel = nSkillClassID - (nSkillID << 8);
		if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(nSkillID, nLevel))
		{
			pSD->buffedByBuffer = 1;
			pSI->ActivateSkill(this, this, 0, 0);
			pSD->buffedByBuffer = 0;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find skill info - skillId[%d] level[%d]", __FUNCTION__, nSkillID, nLevel);
		}
	}
}

void CCreature::DispelByAbnormal(int nAbnormalType, bool useSDLock)
{
	guard;

	if(ValidCreature())
	{
		bool bHasBuff = false;
		abnormalStatusLock.Lock(__FILEW__, __LINE__);
		for(AbnormalDataVector::iterator Iter = vAbnormalStatus.begin();Iter!=vAbnormalStatus.end();Iter++)
		{
			if(Iter->pSI)
			{
				if(Iter->pSI->nAbnormalType == nAbnormalType)
				{
					if(Iter->pSI->nAbnormalType > 63 &&  Iter->pSI->nAbnormalType < 78)
					{
						//song
						if(songSlotsUsed > 0)
							songSlotsUsed--;

					}else if(Iter->pSI->nAbnormalType > 77 && Iter->pSI->nAbnormalType < 91)
					{
						//dance
						if(danceSlotsUsed > 0)
							danceSlotsUsed--;
					}
					vAbnormalStatus.erase(Iter);
					pSD->DeleteAbnormalStatus(nAbnormalType);
					bHasBuff = true;
					break;
				}
			}
		}
		abnormalStatusLock.Unlock();
		if(bHasBuff)
		{
			if(useSDLock)
				lpSharedDataLock->Lock(__FILEW__, __LINE__);

			ValidateAllOnSIM();

			if(useSDLock)
				lpSharedDataLock->Unlock();

			broadcastInfoPacket = true;
			
			CheckAbnormalVisualEffect();

			if(User *pUser = SafeCastUser())
			{
				pUser->RequestUpdateSpelledInfo();
				pUser->SendAbnormalStatusInfo();
				pUser->SetUserInfoChanged();
				pUser->SetCharInfoChanged();
			}
		}
	}
	unguard;
}

void CCreature::TurnOffSpiritshot()
{
	typedef void (*_toss)(CCreature*);
	_toss toss = (_toss) 0x004E7420;
	toss(this);
}

void CCreature::TurnOffSoulshot()
{
	typedef void(*_toss)(CCreature*);
	_toss toss = (_toss) 0x004E6F30;
	toss(this);
}

User* CCreature::GetResponsibleUser()
{
	CTL;

	if(ValidUser())
	{
		return (User*)this;
	}else if(IsPet())
	{
		return CastPet()->GetMaster();
	}else if(IsSummon())
	{
		return CastSummon()->GetMaster();
	}

	return 0;
}

FVector* CCreature::GetPos(FVector* Pos)
{
	typedef FVector* (*_f)(CCreature*, FVector*);
	_f f = (_f) 0x00442A10;
	return f(this, Pos);
}

int CCreature::HasAbnormal(int abnormalType)
{
	guard;
	INT32 abnormalLevel = 0;
	if(this)
	{
		for(UINT n=0;n<40;n++)
		{
			if(pSD->AbnormalTable[n][0] == abnormalType)
			{
				abnormalLevel = pSD->AbnormalTable[n][1];
				break;
			}
		}
	}

	unguard;
	return abnormalLevel;
}

bool CCreature::UpdateAbnormalLevel(int skillId, int newLevel, bool mayDown)
{
	guard;
	bool ret = false;

	if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(skillId, newLevel))
	{
		bool validate = false;
		abnormalStatusLock.Lock(__FILEW__, __LINE__);
		for(AbnormalDataVector::iterator it = vAbnormalStatus.begin();it!=vAbnormalStatus.end();it++)
		{
			if(it->pSI)
			{
				if(it->pSI->nSkillID == skillId)
				{
					if(it->pSI->nLevel < newLevel || mayDown)
					{
						it->pSI = pSI;
						//update abnormal level in shared data
						for(UINT n=0;n<40;n++)
						{
							if(pSD->AbnormalTable[n][0] == pSI->nAbnormalType)
							{
								pSD->AbnormalTable[n][1] = pSI->nAbnormalLevel;
								break;
							}
						}
						validate = true;
						break;
					}
				}
			}
		}
		abnormalStatusLock.Unlock();

		if(validate)
		{
			lpSharedDataLock->Lock(__FILEW__, __LINE__);
			ValidateAllOnSIM();
			lpSharedDataLock->Unlock();
			broadcastInfoPacket = true;

			if(User *pUser = GetUser())
			{
				pUser->SendAbnormalStatusInfo();
			}
			ret = true;
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid skill[%d][%d] - cannot get skill info!", __FUNCTION__, skillId, newLevel);
	}

	unguard;
	return ret;
}

void CCreature::ResetSkillReuse()
{
	//static CSharedGuard SharedGuard(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	DWORD nTickCount = GetTickCount();
	skillReuseMapLock.Lock(__FILEW__, __LINE__);
	for(SkillReuseMap::iterator Iter=this->mSkillReuse.begin();Iter!=this->mSkillReuse.end();Iter++)
	{
		switch(Iter->first.skill_id)
		{
		case 395:	//HeroSkill
		case 396:	//HeroSkill
		case 1374:	//HeroSkill
		case 1375:	//HeroSkill
		case 1376:	//HeroSkill
			{
				//dont reset reuse for that skill
				break;
			}
		default:
			{
				if(Iter->second > nTickCount)
				{
					Iter->second = nTickCount;					
				}
				break;
			}
		};
	}
	skillReuseMapLock.Unlock();
	if(ValidUser())
	{
		GetUser()->GetSocket()->Send("cd", SKILL_COOL_TIME, 0);
	}
}

void CCreature::SocialAction(SocialActionType ActionID)
{
	typedef void (*_f)(CCreature*, SocialActionType);
	_f f = (_f) 0x004ED090;
	f(this, ActionID);
};
DWORD CCreature::GetSkillReuse(int nSkillID)
{
	if(this)
	{
		DWORD nReuse = NULL;
		skillReuseMapLock.Lock(__FILEW__, __LINE__);
		SkillReuseMap::const_iterator Iter = mSkillReuse.begin();
		for(;Iter!=mSkillReuse.end();Iter++)
		{
			if(Iter->first.skill_id == nSkillID)
			{
				nReuse = Iter->second;
				break;
			}
		}
		skillReuseMapLock.Unlock();
		return nReuse;
	}
	return 0;
}
bool CCreature::IsInAlliance(CCreature *pCreature)
{
	User *p1 = NULL;
	User *p2 = NULL;
	if(pCreature->ValidUser())
	{
		p1 = pCreature->GetUser();
	}else if(pCreature->IsSummon() || pCreature->IsPet())
	{
		p1 = pCreature->CastSummon()->GetMaster();
	}
	if(this->ValidUser())
	{
		p2 = this->GetUser();
	}else if(this->IsSummon() || this->IsPet())
	{
		p2 = this->CastSummon()->GetMaster();
	}
	if(p1 && p2)
	{
		if(p1->pSD->nPledgeID != 0)
		{
			if(p2->pSD->nPledgeID != 0)
			{
				if(p1->pSD->nPledgeID == p2->pSD->nPledgeID)
					return true;

				if(p1->pSD->nAllianceID == p2->pSD->nAllianceID && p1->pSD->nAllianceID != 0)
					return true;
			}else
				return false;
		}else
			return false;
	}
	return false;
}

bool CCreature::IsMyParty(CCreature *pCreature)
{
	typedef bool (*_f)(CCreature*, CCreature*);
	_f f = (_f) 0x00802DD0;

	return f(this, pCreature);
}

void CCreature::BroadcastCombatModeStart()
{
	typedef void (*f)(CCreature*);
	f(0x4EBA50L)(this);
}

void CCreature::DispelNpcBuff()
{
	if( ValidCreature() )
	{
		abnormalStatusLock.Lock(__FILEW__, __LINE__);

		int nCount = (int)vAbnormalStatus.size();

		for(int n = 0; n < nCount; n++)
		{
			AbnormalDataVector::iterator Iter = vAbnormalStatus.begin();
			for(;Iter!=vAbnormalStatus.end();Iter++)
			{
				try
				{
					if( !(Iter->selfBuff & 1) && Iter->pSI->nDebuff != 1 )
					{
						if(Iter->pSI->nAbnormalType > 63 &&  Iter->pSI->nAbnormalType < 78)
						{
							//song
							if(songSlotsUsed > 0)
								songSlotsUsed--;

						}else if(Iter->pSI->nAbnormalType > 77 && Iter->pSI->nAbnormalType < 91)
						{
							//dance
							if(danceSlotsUsed > 0)
								danceSlotsUsed--;
						}

						//Deleting from abnormal table
						CreatureAction::OnDeleteAbnormalStatus(pSD, Iter->pSI->nAbnormalType);
						//Send msg to user (if target is user)
						if(User *pUser = GetUser())
							pUser->GetSocket()->Send("cddddd", 0x64, 749, 1, 4, Iter->pSI->nSkillID, Iter->pSI->nLevel);

						vAbnormalStatus.erase(Iter);
						break;
					}
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
				}
			}
		}
		abnormalStatusLock.Unlock();

		lpSharedDataLock->Lock(__FILEW__, __LINE__);
		ValidateAllOnSIM();
		lpSharedDataLock->Unlock();
		broadcastInfoPacket = true;

		CheckAbnormalVisualEffect();

		if(User *pUser = GetUser())
		{
			pUser->RequestUpdateSpelledInfo();
			pUser->SendAbnormalStatusInfo();
			pUser->SetUserInfoChanged();
			pUser->SetCharInfoChanged();
		}
	}
}

double CCreature::GetDefAttribute(CSkillInfo::ATTRIBUTE_TYPE AttributeType)
{
	typedef double (*f)(CCreature*, int);
	return f(0x4D8F10L)(this, AttributeType);
}

CTimeData CCreature::GetSkillReuseEx(int SkillID, int Level)
{
	CTimeData reuse;
	DWORD CurrentTick = GetTickCount();
	skillReuseMapLock.Lock(__FILEW__, __LINE__);
	if(Level > 0)
	{
		CSkillKey key(SkillID, Level);
		SkillReuseMap::iterator Iter = mSkillReuse.find(key);
		if(Iter!=mSkillReuse.end())
		{
			if(Iter->second > CurrentTick)
				reuse.Set((Iter->second - CurrentTick));
		}
	}else
	{
		for(SkillReuseMap::iterator Iter = mSkillReuse.begin();Iter!=mSkillReuse.end();Iter++)
		{
			if(Iter->first.skill_id == SkillID)
			{
				if(Iter->second > CurrentTick)
				{
					reuse.Set((Iter->second - CurrentTick));
				}
				break;
			}
		}
	}
	skillReuseMapLock.Unlock();
	return reuse;
}

bool CCreature::TurnOffToggleSkills()
{
	bool result = false;
	if(ValidCreature())
	{
		try
		{
			ToggleSkillLock.Lock(__FILEW__, __LINE__);
			ToggleSkillMap::iterator Iter = mToggleSkill.begin();
			for(;Iter!=mToggleSkill.end();Iter++)
			{
				if(Iter->second.InUse)
				{
					Iter->second.InUse = false;
					result = true;
				}
			}
			ToggleSkillLock.Unlock();
			if(result)
			{
				ValidateParameters();
				CheckAbnormalVisualEffect();
				if(User *pUser = SafeCastUser())
					pUser->SetAbnormalStatusChanged();
			}
		}catch(...)
		{
			EXCEPTION_LOG;
		}
	}
	return result;
}

bool CCreature::ToggleSkillAction(int nSkillID, int nLevel)
{
	try
	{
		if(User *pUser = GetUser())
		{
			if(pSD->nAlive && canUseItem)
			{
				CSkillInfo *pSI = g_SkillDB.GetSkillInfo(nSkillID, nLevel);
				if(pSI)
				{
					if(pSI->nNextAction == 0)
					{
						if(pSI->CanUsedBy(this))
						{
							switch(pSI->nIsMagic)
							{
							case 0:
								//physical
								if(!canCastPhysical)
									return false;
								break;
							case 1:
								//magical
								if(!canCastMagical)
									return false;
								break;
							case 2:
								if(!canCastOther)
									return false;
								break;
							default:
								return false;
								break;
							};
							
							bool result = false;

							ToggleSkillLock.Lock(__FILEW__, __LINE__);
							ToggleSkillMap::iterator Iter = mToggleSkill.find(nSkillID);
							if(Iter!=mToggleSkill.end())
							{
								if(Iter->second.InUse)
								{
									Iter->second.InUse = false;
								}else
								{
									result = true;
								}
							}
							ToggleSkillLock.Unlock();

							if(result)
							{
								//check for operate cond
								if(pSI->vOperateCondition.size() > 0)
								{
									for(vector<CSkillOperateCondition*>::iterator Iter = pSI->vOperateCondition.begin();Iter!=pSI->vOperateCondition.end();Iter++)
									{
										if(CSkillOperateCondition *pCond = (*Iter))
										{
											if(!pCond->CanUse(this, this))
											{
												CSystemMessage msg(113);
												msg.AddSkill(nSkillID, nLevel);
												pUser->SendSystemMessage(&msg);
												return false;
											}
										}
									}
								}

								//check for mp/ hp requirements
								bool hasRequiredHPMP = false;
								lpSharedDataLock->Lock(__FILEW__, __LINE__);
								if(pSD->fHP > (double)pSI->nConsumeHP && pSD->fMP > (double)pSI->nConsumeMP1)
								{
									pSD->fHP = pSD->fHP - (double)pSI->nConsumeHP;
									pSD->fMP = pSD->fMP - (double)pSI->nConsumeMP1;
									hasRequiredHPMP = true;
								}
								lpSharedDataLock->Unlock();

								if(!hasRequiredHPMP)
									return false;

								ToggleSkillLock.Lock(__FILEW__, __LINE__);
								ToggleSkillMap::iterator Iter = mToggleSkill.find(nSkillID);
								if(Iter!=mToggleSkill.end())
								{
									Iter->second.InUse = true;
								}
								ToggleSkillLock.Unlock();

								CSystemMessage msg(46); //46	1	You use $s1.	0	79	9B	B0	FF			0	0	0	0	0		none

								msg.AddSkill(nSkillID, nLevel);
								pUser->SendSystemMessage(&msg);
							}else
							{
								CSystemMessage msg(335); //335	1	$s1 has been aborted.	0	79	9B	B0	FF			0	0	0	0	0		none
								msg.AddSkill(nSkillID, nLevel);
								pUser->SendSystemMessage(&msg);
							}
							ValidateParameters();
							CheckAbnormalVisualEffect();
							pUser->SendAbnormalStatusInfo();
						}
					}else
					{
						SkillAction(nSkillID, 0, 0);
					}
				}
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	return false;
}

void CCreature::InsertAbnormalData(AbnormalData abnormal)
{
	try
	{
		if(abnormal.pSI->nDebuff > 1)
			vAbnormalStatus.push_back(abnormal);
		else if(abnormal.pSI->nDebuff == 0)
		{
			AbnormalDataVector::iterator Iter = vAbnormalStatus.begin();
			for(;Iter!=vAbnormalStatus.end();Iter++)
			{
				if(Iter->pSI->nDebuff > 0)
					break;
			}
			vAbnormalStatus.insert(Iter, abnormal);
		}else if(abnormal.pSI->nDebuff == 1)
		{
			AbnormalDataVector::iterator Iter = vAbnormalStatus.begin();
			for(;Iter!=vAbnormalStatus.end();Iter++)
			{
				if(Iter->pSI->nDebuff > 1)
					break;
			}
			vAbnormalStatus.insert(Iter, abnormal);
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
}

void CCreature::MakeCombat()
{
	if(ValidCreature())
	{
		if(inCombatMode)
		{
			combatModeTimeStamp = time(NULL);
		}else
		{
			inCombatMode = true;
			combatModeTimeStamp = time(NULL);
			BroadcastCombatModeStart();
		}
	}
}

void CCreature::StopMove()
{
	typedef void(*f)(CCreature*);
	f(0x4E4CB0L)(this);
}

bool CCreature::ResurrectedBy(CCreature *pCreature, INT32 percent)
{
	if(pCreature == 0)
		pCreature = this;

	typedef bool (*f)(CCreature*, CCreature*, INT32);
	return f(0x518070L)(this, pCreature, percent);
}

bool CCreature::IsSameAbnormalTeam(CCreature *pOther)
{
	if(ValidCreature() && pOther->ValidCreature())
	{
		INT32 teamCreature = 0;
		INT32 teamAttacker = 0;
		for(INT32 n=0;n<40;n++)
		{
			if(pSD->AbnormalTable[n][0] == 189)
			{
				teamCreature = pSD->AbnormalTable[n][1];
				break;
			}
		}
		if(teamCreature > 0)
		{
			for(INT32 n=0;n<40;n++)
			{
				if(pOther->pSD->AbnormalTable[n][0] == 189)
				{
					teamAttacker = pOther->pSD->AbnormalTable[n][1];
					break;
				}
			}

			if(teamCreature == teamAttacker)
			{
				return true;
			}
		}
	}
	return false;
}

bool CCreature::ValidateAttackSpeed()
{
	UINT64 addr = (*reinterpret_cast<PUINT64>(this));
	addr += 0x480;
	addr = (*reinterpret_cast<PUINT64>(addr));
	typedef bool (*f)(CCreature*);
	return f(addr)(this);
}

bool CCreature::ValidateSpeed()
{
	UINT64 addr = (*reinterpret_cast<PUINT64>(this));
	addr += 0x488;
	addr = (*reinterpret_cast<PUINT64>(addr));
	typedef bool (*f)(CCreature*);
	return f(addr)(this);
}


double CCreature::EquipedArmorSpeedByCrystal()
{
	typedef double(*f)(CCreature*);
	return f(0x4E3250L)(this);
}

double CCreature::WeaponHitCorrectionByCrystal()
{
	typedef double(*f)(CCreature*);
	return f(0x4E3A90L)(this);
}

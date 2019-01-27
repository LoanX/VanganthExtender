#include "stdafx.h"
#include "GradePenalty.h"

CGradePenalty g_GradePenalty;

void CGradePenalty::Init()
{
	m_Enabled = false;
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled, skill[%d][%d]", __FUNCTION__, m_PenaltySkill.skillId, m_PenaltySkill.level);
	}
}

void CGradePenalty::LoadINI()
{
	const TCHAR* section = _T("GradePenalty");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile) ? true : false;
	m_PenaltySkill.skillId = GetPrivateProfileInt(section, _T("SkillId"), 0, g_ConfigFile);
	m_PenaltySkill.level = GetPrivateProfileInt(section, _T("SkillLevel"), 0, g_ConfigFile);
}

void CGradePenalty::CheckState(User *pUser, bool gradePenalty)
{
	CTL;
	if(m_Enabled)
	{
		if(gradePenalty)
		{
			if(!pUser->pED->gradePenaltySet)
			{
				pUser->pED->gradePenaltySet = true;
				if(m_PenaltySkill.skillId && m_PenaltySkill.level)
				{
					pUser->AcquireSkill(m_PenaltySkill.skillId, m_PenaltySkill.level);
					pUser->SetValidateParametersRequest();
				}
			}
		}else
		{
			if(pUser->pED->gradePenaltySet)
			{
				if(m_PenaltySkill.skillId && m_PenaltySkill.level)
				{
					pUser->DeleteSkill(m_PenaltySkill.skillId);
					pUser->SetValidateParametersRequest();
				}
				pUser->pED->gradePenaltySet = false;
			}
		}
	}
}

void CGradePenalty::Validate(User *pUser)
{
	CTL;

	if(m_Enabled)
	{
		if(pUser->EquipedArmorSpeedByCrystal() != 1.0 || pUser->WeaponHitCorrectionByCrystal() != 0.0 )
		{
			//apply penalty
			pUser->pED->gradePenaltySet = true;
			if(m_PenaltySkill.skillId && m_PenaltySkill.level)
			{
				pUser->AcquireSkill(m_PenaltySkill.skillId, m_PenaltySkill.level);
				pUser->SetValidateParametersRequest();
			}
		}else
		{
			if(m_PenaltySkill.skillId && m_PenaltySkill.level)
			{
				if(pUser->HaveSkill(m_PenaltySkill.skillId, m_PenaltySkill.level))
				{
					pUser->DeleteSkill(m_PenaltySkill.skillId);
					pUser->SetValidateParametersRequest();
				}
			}
			pUser->pED->gradePenaltySet = false;
		}
	}
}
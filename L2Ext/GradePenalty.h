#pragma once


class CGradePenalty
{
	bool m_Enabled;
	SkillPchId m_PenaltySkill;
	void LoadINI();
public:
	void Init();
	inline bool Enabled() { return m_Enabled; }
	void CheckState(User *pUser, bool gradePenalty);
	void Validate(User *pUser);

public:
};

extern CGradePenalty g_GradePenalty;
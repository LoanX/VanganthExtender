#pragma once

class CSpawnProtection
{
	bool m_Enabled;
	bool m_DeactivateOnMoving;
	UINT m_TimeoutTime;
	UINT m_SkillId;
	void LoadINI();
public:
	void Initialize();
	inline bool IsEnabled() { return m_Enabled; };
	inline bool IsDeactivateOnMoving() { return m_DeactivateOnMoving; };
	void ActivateProtection(User *pUser);
	void DeactivateProtection(User *pUser);
};

extern CSpawnProtection g_SpawnProtection;
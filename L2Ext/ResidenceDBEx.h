#pragma once

struct ResidenceDataEx
{
	INT32 residence_id;
	INT32 reward_id;
	INT32 reward_count;
	BOOL castle;
};

typedef map<int, ResidenceDataEx> ResidenceExMap;

/* Since it's readonly so CRITICAL_SECTION isn't required. */

class CResidenceDBEx
{
	ResidenceExMap mData;
public:
	CResidenceDBEx();
	~CResidenceDBEx();
	void Load();
	const ResidenceDataEx& GetData(int residence_id);
};

extern CResidenceDBEx g_ResidenceDBEx;
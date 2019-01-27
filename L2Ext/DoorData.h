#pragma once
#include "DoorGeo.h"

struct DoorInfo
{
	UINT objectId;	//static object id
	CDoorGeo doorGeo;
	CDoor *pObject;
	DoorInfo() : pObject(0), objectId(0) {};
};

class CDoorData
{
	BOOL m_Enabled;
	vector<DoorInfo*> m_Doors;
	double m_SearchRange;
	double m_MissingHpPercent;
	double m_BlockRideRange;
	BOOL m_AffectCloseCombat;
	BOOL m_Disable2ndSkillGeoCheck;
	void LoadData();
public:
	void Init();
	void GatherNearestDoors(FVector l1, vector<DoorInfo*>& doors, double range);
	bool CanSee(FVector l1, FVector l2);
	static bool OnGeoCanSee(PVOID pInstance, FVector pos1, FVector pos2);
	static bool ReturnTrue();
	bool IsIntersection(FVector l1, FVector l2, FVector b1, FVector b2);
	inline BOOL IsEnabled() { return m_Enabled; };
	inline BOOL IsAffectCloseCombat() { return m_AffectCloseCombat; };
	inline double GetBlockRideRange() { return m_BlockRideRange; };
};

extern CDoorData g_DoorData;
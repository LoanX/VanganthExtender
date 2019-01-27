#pragma once
#include "TerritoryEx.h"

class CTerritoryChecker
{
	map<int, CTerritoryEx*> mData;
public:
	CTerritoryChecker();
	~CTerritoryChecker();
	void Initialize();
	void ReadData(string sFile);
	void GetTerritories(CCreature *pCreature, TerritoryData& td);
	int IsInTerritory(CCreature *pCreature, TerritoryType type);
	int IsInTerritory(FVector& Pos, TerritoryType type);
	void ValidateRideTerritory(CCreature *pCreature);
	CTerritoryEx* GetTerritory(int ID);
};

extern CTerritoryChecker g_TerritoryChecker;